all:
	gcc -g spectre.c -o spectre -O1

asm:
	gcc -S spectre.c -o spectre.s -O1
	
mac:
	clang spectre.c -o spectre

watch:
	watch -n 0.2 ./spectre

gcc_a:
	gcc attacker.c -o attack

mwatch:
	watch -n 0.2 LD_PRELOAD=$(realpath ./mimalloc-main/out/libmimalloc.so) ./mspectre

compileMI:
	cd mimalloc-main && mkdir -p out && cd out && cmake .. && make -j4

mspectre:
	gcc -g mspectre.c -o mspectre

masm:
	gcc -S mspectre.c -o mspectre.s

clean:
	rm spectre
