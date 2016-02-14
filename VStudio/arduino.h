//-------------------------------------------------------------------
#ifndef __arduino_H__
#define __arduino_H__
//-------------------------------------------------------------------

typedef signed char		int8_t;
typedef unsigned char	uint8_t;
typedef unsigned short     uint16_t;
#define boolean		bool
#define byte		unsigned char
#define strcpy_P(d, s)	strcpy_s(d, this->sizex + 1, s)

#define HIGH	1
#define LOW		0
#define INPUT	1
#define INPUT_PULLUP	3
#define OUTPUT	2
#define DEC		1
#define BIN		2
#define HEX		3
#define NULL	0
#define PROGMEM
#define PGM_P   const char *

#define NUM_DIGITAL_PINS            70
#define NUM_ANALOG_INPUTS           16

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define F(string)	string
#define __FlashStringHelper	char
#define prog_char	char
byte pgm_read_byte(const char *);
int pgm_read_word(const char *);

void pinMode(int, int);
void digitalWrite(int, int);
int digitalRead(int);
void analogWrite(int, int);
int analogRead(int);
unsigned long millis();
unsigned long micros();
void delay(int);

int map(int, int, int, int, int);

void noInterrupts();
void interrupts();
void attachInterrupt(uint8_t, void(*)(void), int mode);
void detachInterrupt(uint8_t);

int freeMemory();

double power(double val, int exp);

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
