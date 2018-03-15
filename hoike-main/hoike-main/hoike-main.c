/*
 * Hoike.c
 *
 * Created: 3/1/2018 3:40:20 PM
 *  Author: John Pham
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "io.c"
#include "Timer.c"
#include "adc.c"
#include "GetSetBit.c"
#include "usart_ATmega1284.h"

enum States { Init, UserInput, Visualize, Learn } State;

unsigned char button = 0x00;

unsigned char learnPosition = 0;
unsigned char song[] = { 0x04, 0x14, 0x30, 0xD0, 0x08 };
unsigned char songLength = 4;
unsigned char songNotes[] = { 'A', '7', '\0', 'A', ' ', '\0', 'F', ' ', '\0', 'D', ' ', '\0', 'C', '7', '\0' };
unsigned char notePosition = 0;
unsigned char notePrompt;

unsigned char i = 0;
unsigned char update = 1;
unsigned short microphone = 0x0000;
unsigned char redVal = 0x05;
unsigned char greenVal = 0x00;
unsigned char blueVal = 0x00;

#define BLUEBTN 4
#define REDBTN 3
#define GREENBTN 2

unsigned char getRed(unsigned short input) {
	return input % 257;
}

unsigned char getBlue(unsigned short input) {
	return (input >> 1) % 257;
}

unsigned char getGreen(unsigned short input) {
	return (input >> 2) % 257;
}

unsigned char reverse(unsigned char b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

void Tick() {
	microphone = ADC;

	button = ~PIND;
	
	switch (State) {
		case Init:
			i = 0;
			update = 1;
			State = UserInput;
			PORTA = 0x00;
			PORTB = 0x00;
			while (!USART_IsSendReady(0)) {};
			USART_Send(0, 0);
			while (!USART_IsSendReady(0)) {};
			USART_Send(0, 0);
			while (!USART_IsSendReady(0)) {};
			USART_Send(0, 0);
			learnPosition = eeprom_read_byte((uint8_t*) 10);
			notePosition = eeprom_read_byte(20);
			if (learnPosition < 0 || learnPosition > songLength) {
				learnPosition = 0;
				notePosition = 0;
			}
			
			break;
		case UserInput:
			
			if (i <= 10 && update) {
				LCD_DisplayString(1, "Hoike [Home]");
				update = 0;
			}
			if (i >= 40 && update) {
				LCD_DisplayString(1, "(G) Learn       (B) Visualize");
				if (i >= 20) {
					i = 0;
				}
				update = 0;
				i = 0;
			}
			i++;
			if (i >= 10) {
				update = 1;
			}
			if (GetBit(button, BLUEBTN) == 0) {
				State = Visualize;
				i = 0;
				update = 1;
			}
			if (GetBit(button, REDBTN) == 0) {
				State = UserInput;
				i = 0;
				update = 1;
			}
			if (GetBit(button, GREENBTN) == 0) {
				State = Learn;
				i = 0;
				update = 1;
			}
			break;
		case Visualize:
			if (update) {
				LCD_DisplayString(1, "Hoike [Visual]  (R) Home");
				update = 0;
			}
			
			PORTA = (char)(microphone);
			PORTB = reverse((char)(microphone));
			redVal = getRed(microphone);
			greenVal = getGreen(microphone);
			blueVal = getBlue(microphone);
			while (!USART_IsSendReady(0)) {};
			USART_Send(redVal, 0);
			while (!USART_IsSendReady(0)) {};
			USART_Send(greenVal, 0);
			while (!USART_IsSendReady(0)) {};
			USART_Send(blueVal, 0);
			if (GetBit(button, 3) == 0) {
				State = Init;
			}
			break;
		case Learn:
			{}
			unsigned char* str[80];
			strcpy(str, "Hoike [Learn] ");
			strcat(str, &songNotes[notePosition]);
			if (update) {
				LCD_DisplayString(1, str);
				update = 0;
			}
	
			PORTA = song[learnPosition];
			if (GetBit(button, BLUEBTN) == 0) {

				if (learnPosition > 0) {
					learnPosition--;
					eeprom_write_byte( (uint8_t *) 10, learnPosition);
					notePosition -= 3;
					eeprom_write_byte( (uint8_t *) 20, notePosition);
				}
				State = Learn;
				update = 1;
			}
			if (GetBit(button, REDBTN) == 0) {
				State = Init;
			}
			if (GetBit(button, GREENBTN) == 0) {
				if (learnPosition < songLength) {
					learnPosition++;
					eeprom_write_byte( (uint8_t *) 10, learnPosition);
					notePosition += 3;
					eeprom_write_byte( (uint8_t *) 20, notePosition);
				}
				State = Learn;
				update = 1;
			}
			
			break;
	}
}

int main(void)
{
	DDRA = 0xFE;
	PORTA = 0x01;
	
	DDRB = 0xFF;
	PORTB = 0x00;
	
	DDRC = 0xFF;
	PORTC = 0x00;
	
	DDRD = 0xFF;
	PORTD = 0x00;
	
	ADC_init();
	TimerSet(100);
	TimerOn();
	LCD_init();
	initUSART(0);
	USART_Flush(0);
	
	State = Init;
	
    while(1)                                                                                                                                                                                          
    {
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
    }
	
}
