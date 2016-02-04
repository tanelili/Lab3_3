/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */
#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif
#include "lcd_port.h"
#include <cr_section_macros.h>
#include "LiquidCrystal.h"
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <iostream>

#define TICKRATE_HZ1 (1000) // Ticks per seconds

volatile int counter = 0;
volatile int h = 0;
volatile int m = 0;
volatile int s = 0;
volatile int ms = 0;

class Timer {

public:
	Timer(int min = 1, int sec = 0, int ms =0){
		tickit = (sec * 100) + (min * 60 * 100) + (ms / 10);
		tickeja = 0;
	};
	virtual ~Timer();
	bool Tick(); // make timer tick once, returns true if timer expired,

private:
	int tickit;
	int tickeja;
};

bool Timer::Tick() {
	// Tikataan kerran

	if (tickit != tickeja) {
		tickeja++;
		return false;
	}

	if ( tickit <= tickeja) {
		tickeja = 0;
		return true;
	}

}

Timer::~Timer() {

}

#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Handler(void)
{
	if ( counter <= 0) {
		counter = 1000;
	}
	if(counter > 0){
		counter--;
	}


/*
	// RTC
	ms++;
	//flag = true;

	if ( h == 24) {
		h = 0;
	}

	if ( ms == 100) {
		s++;
		ms = 0;

		if (s == 60) {
			m++;
			s = 0;

			if ( m == 60) {
				h++;
				m = 0;
			}
		}
	}*/
}

#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
	counter = ms;
	while (counter > 0) {
		__WFI();
	}
}

int main(void) {

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(3, true);
#endif
#endif

	Timer aika(0, 30, 0);

	Board_Init();
	Chip_Clock_SetSysTickClockDiv(1);
	uint32_t sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / TICKRATE_HZ1);

	LiquidCrystal meth(8, 9, 10, 11, 12, 13);
	meth.begin(16,2);
	meth.setCursor(0,0);
	string teksti = "Testi teksti 010"; //Hiukan liian raskas vakiomerkkitulostukseen
	meth.print(teksti);
	while(8) {
	}
	return 0;
}
