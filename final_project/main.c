#include "tm4c123gh6pm.h"
#include "PLL.h"

void DisableInterrupts(void);
void EnableInterrupts(void);

void ADC_Init(void){
	SYSCTL_RCGCADC_R 		|= 0x1;
	SYSCTL_RCGCGPIO_R 	|= 0x10;
	while((SYSCTL_PRGPIO_R&0x10) != 0x10){};
	
	GPIO_PORTE_DIR_R 		&= ~0x10;
	GPIO_PORTE_AFSEL_R	|= 0x10;
	GPIO_PORTE_DEN_R 		&= ~0x10;
	GPIO_PORTE_AMSEL_R 	|= 0x10;
		
	ADC0_PC_R						&= ~0xF;
	ADC0_PC_R						|= 0x1;
	ADC0_SSPRI_R				 = 0x0123;
	ADC0_ACTSS_R				&= ~0x0008;
	ADC0_EMUX_R 				&= ~0xF000;
	ADC0_SSMUX3_R 			&= ~0x000F;
	ADC0_SSMUX3_R				+= 9;
	ADC0_SSCTL3_R				= 0x0006;
	ADC0_IM_R						&= ~0x0008;
	ADC0_ACTSS_R 				|= 0x0008;
}

uint32_t ADC0_InSeq3(void){
	uint32_t result;
	ADC0_PSSI_R 				= 0x0008;
	while((ADC0_RIS_R & 0x08) == 0){};
	result = ADC0_SSFIFO3_R & 0xFFF;
	ADC0_ISC_R 					= 0x0008;
	return result;
}



#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))

// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
	volatile int delay __attribute__((unused));
  SYSCTL_RCGCGPIO_R 	|= 0x20;
  delay 							 = SYSCTL_RCGCGPIO_R;
  GPIO_PORTF_DIR_R 		|= 0x0E;
  GPIO_PORTF_DEN_R 		|= 0x0E;
}

// ***************** Timer3A_Init ****************
// Activate Timer3 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
//          priority 0 (highest) to 7 (lowest)
// Outputs: none
void Timer3A_Init(uint32_t period, uint32_t priority){
  volatile uint32_t delay __attribute__((unused));
  SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
  delay = SYSCTL_RCGCTIMER_R;         // user function
  TIMER3_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER3_TAILR_R = period-1;    // 4) reload value
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|(priority<<29); // priority  
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN1_R = 1<<(35-32);      // 9) enable IRQ 35 in NVIC
  TIMER3_CTL_R = 0x00000001;    // 10) enable timer3A
}

void Timer3A_Stop(void){
  NVIC_DIS1_R = 1<<(35-32);   // 9) disable interrupt 35 in NVIC
  TIMER3_CTL_R = 0x00000000;  // 10) disable timer3
}
uint32_t Data;
uint32_t Flag;
void Timer3A_Handler(void){
  TIMER3_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER3A timeout
	if(Flag != 0){
		return;
	}
//	GPIO_PORTF_DATA_R ^= 0x02;
	Data = ADC0_InSeq3();
	Flag = 1;
}

//Assumes 80 MHz bus clock
void UART_Init(void){
	SYSCTL_RCGCUART_R 	|= 0x0001; 																			// activate UART0
	SYSCTL_RCGCGPIO_R 	|= 0x0001; 																			// activate port A
	while((SYSCTL_PRGPIO_R&0x01) == 0){};
	UART0_CTL_R 				&= ~0x0001; 																		// disable UART
	UART0_IBRD_R 				 = 43;																					// IBRD = int(80,000,000 / (16 * 115,200)) = int(43.40278)
	UART0_FBRD_R				 = 26;																					// FBRD = int(0.40278 * 64) = 26
	UART0_LCRH_R				 = 0x0070;																			// 8-bit word length, enable FIFO
	UART0_CTL_R					 = 0x0301;																			//enable RXE, TXE, and UART
	GPIO_PORTA_PCTL_R		 = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;		//Use pins as UART
	GPIO_PORTA_AMSEL_R	&= ~0x03;																				//TM4C123GH6PM, disable analog on PA1-0
	GPIO_PORTA_AFSEL_R	|= 0x03;																				//enable alt functionality on PA1-0
	GPIO_PORTA_DEN_R		|= 0x03;																				//enable digital I/O on PA1-0
	
}

void UART_OutChar(char data){
	while((UART0_FR_R&UART_FR_TXFF) != 0){
	};
  UART0_DR_R = data;
	
//	while((UART0_FR_R&0x0020) != 0);
//	UART0_DR_R = data;
}

#define CR   0x0D
#define LF   0x0A
void UART_OutputNewLine(void){
//	UART_OutChar(CR);
	UART_OutChar(LF);
}

void UART_SendMessage(char * bit, char * buffer){
	int i = 0;
	while(bit[i] != '\0'){
		UART_OutChar(bit[i]);
		i++;
	}
	
	UART_OutChar(',');
	
	i = 0;
	while(buffer[i] != '\0'){
		UART_OutChar(buffer[i]);
		i++;
	}
	UART_OutputNewLine();
}

void itoa(uint32_t n, char * buffer){
	uint32_t num = n;
	int count = 0;
	while(num != 0){
		count++;
		num /= 10;
	}
	
	num = n;
	buffer[count] = '\0';
	count--;
	
	for(int i = count; i >= 0; i--){
		uint8_t digit = num % 10;
		buffer[i] = (char)(digit + 0x30);
		num /= 10;
	}
}

void extract_lsb(uint32_t n, char * buffer){
	uint8_t bit = (uint8_t)(n & 0x1);
	buffer[0] = (char)(bit + 0x30);
	buffer[1] = '\0';
}

int main(void){
	DisableInterrupts();
	PLL_Init(Bus80MHz);
	ADC_Init();
	PortF_Init();
	//Assuming 80 MHz bus clock, achieves 100 kHz sampling frequency on ADC
	uint32_t period 		= 800;
	uint32_t priority 	= 6;
	Timer3A_Init(period, priority);
	UART_Init();
	EnableInterrupts();
	
	char buffer[32];
	char bit[2];
	
	while(1){
		if(Flag == 1){
			itoa(Data, buffer);
			extract_lsb(Data, bit);
			UART_SendMessage(bit, buffer);
		}
		Flag = 0;
	}
	
	return 0;
}