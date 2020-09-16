/*
 * Autor: xbilyd01, Daniel Bílý
 * Originál, inspirace z democvièení a jednotlivých cvièení
 * Datum poslední zmìny: 21.12 2019
 *
 */
#include "MK60D10.h"

// Mapping to specific port pins
#define BTN_SW3 0x1000
#define SPK 0x10 //speaker

#define DASH_DELAY 0x200 //delay for dash
#define END_DELAY 0x600 //delay for end of the letter

int button_up = 1; //flag, button pushed
int timer_running = 0; //flag, timer running
int dash_read = 0; //flag, dash has been read, stop beeping

char input[5] = {0}; //input array
int input_index = 0; //input index

// kody morseovky
static const char *morse_code[26] =
{
	".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
	"....", "..", ".---", "-.-", ".-..", "--",
	"-.", "---", ".--.", "--.-", ".-.", "...", "-",
	"..-", "...-", ".--", "-..-", "-.--", "--.."
};
// delay function
void delay(long long bound)
{
  long long i;
  for(i=0;i<bound;i++);
}
//send char via uart
void SendCh(char ch)
{
    while(!(UART5->S1 & UART_S1_TDRE_MASK) && !(UART5->S1 & UART_S1_TC_MASK));
    UART5->D = ch;
}
//adds to input array
void input_add(char ch)
{
	if(input_index <=3)
	{
			input[input_index] = ch;
			input_index++;
			/*
			//prining out input, for debug
			SendCh('/');
			for (int i=0; i<input_index;i++)
			{

				SendCh(input[i]);
			}
			SendCh('/');
			*/
	}

}
// initialize the MCU
void MCUInit(void)
{
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}
// initialize the pins
void PinInit(void)
{
	//clock for uart5
	SIM->SCGC1 = SIM_SCGC1_UART5_MASK;
	//clocks for port A and E
	SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;
	//port A
	PORTA->PCR[4] = PORT_PCR_MUX(0x01); //speaker

	// PORT E
	PORTE->PCR[8]  = PORT_PCR_MUX(0x03); // UART0_TX
	PORTE->PCR[9]  = PORT_PCR_MUX(0x03); // UART0_RX
	PORTE->PCR[10] = PORT_PCR_MUX(0x01); // SW2
	PORTE->PCR[12] = PORT_PCR_MUX(0x01); // SW3
	PORTE->PCR[27] = PORT_PCR_MUX(0x01); // SW4
	PORTE->PCR[26] = PORT_PCR_MUX(0x01); // SW5
	PORTE->PCR[11] = PORT_PCR_MUX(0x01); // SW6

	// set ports as output
	PTA->PDDR =  GPIO_PDDR_PDD(0x0010);
	PTB->PDDR =  GPIO_PDDR_PDD(0x3C);
	PTB->PDOR |= GPIO_PDOR_PDO(0x3C); // turn all LEDs OFF

	//button
	PORTE->PCR[12] = ( PORT_PCR_ISF(0x01)
			| PORT_PCR_IRQC(0x0B)
			| PORT_PCR_MUX(0x01)
			| PORT_PCR_PE(0x01)
			| PORT_PCR_PS(0x01));

	NVIC_ClearPendingIRQ(PORTE_IRQn);
	NVIC_EnableIRQ(PORTE_IRQn);
}
// init uart
void UART5Init(void)
{
	UART5->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

	UART5->BDH = 0x00;
	UART5->BDL = 0x1A;
	UART5->C4 = 0x0F;
	UART5->C1 = 0x00;
	UART5->C3 = 0x00;
	UART5->MA1 = 0x00;
	UART5->MA2 = 0x00;

	UART5->S2 |= 0xC0;

	UART5->C2 |= ( UART_C2_TE_MASK );
}
// init lptmr0
void LPTMR0Init()
{
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK; // Enable clock
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;   // Turn OFF LPTMR
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) // div 2
                 | LPTMR_PSR_PBYP_MASK
                 | LPTMR_PSR_PCS(1)) ;   // use the choice of clock

    LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK    // Clear any pending interrupt
    		| LPTMR_CSR_TIE_MASK  // LPT interrupt enabled
                );
    NVIC_EnableIRQ(LPTMR0_IRQn);         // enable interrupts from LPTMR0
}
// set value to timer and enable it
void set_timer(int value)
{
	LPTMR0_CMR = value;
	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;
}
//stops timer, clears its value
void stop_timer()
{
	LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //use ten, not tie LPTMR_CSR_TIE_MASK;
}
// decode input array
char decode_input()
{
	for (int i = 0; i < 26; i++)
	{
		if(!strcmp(input, morse_code[i]))
		{

			//SendCh(i+65);
			return i + 65; // using inddex value + 65 we get ascii value of given character
		}
	}
	return -1;
}
// clears input array
void clear_input()
{
	for(int i = 0; i<5;i++)
	{
		input[i] = '\0';

	}
	input_index = 0;
}
//handles interupt from lptmr
void LPTMR0_IRQHandler(void)
{
	if(!button_up)
	{
		//dash
		input_add('-');
		dash_read = 1;
		timer_running = 0;
		LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   // writing 1 to TCF tclear the flag
		stop_timer();
		set_timer(END_DELAY);
	}
	else
	{
		//end of char
		char result = decode_input();
		clear_input();
		if(result != -1)
			SendCh(result);

		SendCh(' ');
		LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;
		stop_timer();
	}

}
// handles interupt from the button
void PORTE_IRQHandler(void)
{
    delay(10000);
	if(PORTE_ISFR & BTN_SW3)
	{
		if(!(GPIOE_PDIR & BTN_SW3) && button_up)
		{

			button_up = 0;
			dash_read = 0;
			stop_timer();
			set_timer(DASH_DELAY);
			timer_running = 1;
		}
		else if((GPIOE_PDIR & BTN_SW3) && !button_up)
		{
			if(timer_running)
			{
				input_add('.');
			}

			button_up = 1;
			stop_timer();
			set_timer(END_DELAY);
		}
		PORTE_ISFR = BTN_SW3;
	}
}
int main(void)
{
	MCUInit();
	PinInit();
	UART5Init();
    LPTMR0Init();

    while (1)
    {
    	if(!(GPIOE_PDIR & BTN_SW3) && !dash_read)
        {
        	GPIOA_PDOR ^= SPK;   // invert speaker state
        	delay(1000);
        }
        else if((GPIOE_PDIR & BTN_SW3))
        {
        	GPIOA_PDOR &= ~SPK;
        }

	}

    return 0;
}
