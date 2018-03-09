/*
 * Hoike.c
 *
 * Created: 3/1/2018 3:40:20 PM
 *  Author: John Pham
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
	unsigned short microphone = 0x0000;
// ambient noise level of microphone

	
void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

#ifndef BIT_H
#define BIT_H

////////////////////////////////////////////////////////////////////////////////
//Functionality - Sets bit on a PORTx
//Parameter: Takes in a uChar for a PORTx, the pin number and the binary value
//Returns: The new value of the PORTx
unsigned char SetBit(unsigned char pin, unsigned char number, unsigned char bin_value)
{
	return (bin_value ? pin | (0x01 << number) : pin & ~(0x01 << number));
}

////////////////////////////////////////////////////////////////////////////////
//Functionality - Gets bit from a PINx
//Parameter: Takes in a uChar for a PINx and the pin number
//Returns: The value of the PINx
unsigned char GetBit(unsigned char port, unsigned char number)
{
	return ( port & (0x01 << number) );
}

#endif //BIT_H
enum States { Init, UserInput, Visualize, Learn } State;

unsigned char button = 0x00;

unsigned char learnPosition = 0;
unsigned char song[] = { 0x01, 0x02, 0x03, 0x04, 0xFF };
unsigned char songNotes[] = { 'F', '\0', 'C', '\0', 'D', '\0', 'G', '\0', 'E', '\0' };
unsigned char notePosition = 0;
unsigned char notePrompt;
unsigned char songLength = 5;
unsigned char i = 0;

#define BLUEBTN 0
#define REDBTN 3
#define GREENBTN 2

void Tick() {
	microphone = ADC;

	button = ~PIND;
	
	switch (State) {
		case Init:
			learnPosition = 0;
			i = 0;
			State = UserInput;
			PORTB = 0x00;
			break;
		case UserInput:
			if (i <= 10) {
				LCD_DisplayString(1, "Hoike [Home]");
			}
			if (i > 10 && i <= 20) {
				LCD_DisplayString(1, "(G) Learn       (B) Visualize");
				if (i >= 20) {
					i = 0;
				}
			}
			i++;
			if (GetBit(button, BLUEBTN) == 0) {
				State = Visualize;
				i = 0;
			}
			if (GetBit(button, REDBTN) == 0) {
				State = UserInput;
				i = 0;
			}
			if (GetBit(button, GREENBTN) == 0) {
				State = Learn;
				i = 0;
			}
			break;
		case Visualize:
			LCD_DisplayString(1, "Hoike [Visual]  (R) Home");
			PORTB = (char)(microphone);
			// TODO: hash the microphone to RGBA values
			// Send to Uno via USART
			if (GetBit(button, 3) == 0) {
				State = Init;
			}
			break;
		case Learn:
			if (i <= 20) {
				unsigned char* str[80];
				strcpy(str, "Hoike [Learn] ");
				strcat(str, &songNotes[notePosition]);
				LCD_DisplayString(1, str);
			}
			if (i > 20 && i <= 30) {
				LCD_DisplayString(1, "(G) Back (R) Home (B) Next");
				if (i >= 30) {
					i = 0;
				}
			}
			i++;
			PORTB = song[learnPosition];
			if (GetBit(button, BLUEBTN) == 0) {
				if (learnPosition > 0) {
					learnPosition--;
					notePosition -= 2;
				}
				State = Learn;
			}
			if (GetBit(button, REDBTN) == 0) {
				State = Init;
			}
			if (GetBit(button, GREENBTN) == 0) {
				if (learnPosition < songLength) {
					learnPosition++;
					notePosition += 2;
				}
				State = Learn;
			}
			
			break;
	}
}

int main(void)
{
	// PORT A used for microphone input
	DDRA = 0x00;
	PORTA = 0xFF;
	
	// Port C used for button input
	// C0 is the back button
	// C1 is the restart button
	// C2 is the forward button
	DDRD = 0xFF;
	PORTD = 0x00;
	
	DDRC = 0xFF;
	PORTC = 0x00;
	
	DDRB = 0xFF;
	PORTB = 0xF3;
	
	ADC_init();
	TimerSet(100);
	TimerOn();
	
	LCD_init();
	
	State = Init;

	
    while(1)                                                                                                                                                                                          
    {
		Tick();
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;

    }
}