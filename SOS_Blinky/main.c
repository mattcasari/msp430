/**********************************************************
 * SOS Blinky
 *********************************************************/

/*********************************************************
 * File:  		main.c
 * Author: 		Matt Casari
 * Description: This file contains code to blink the green
 * 				LED on the MSP430 Launchpad with no interrupts
 * 				driven directly from main loop with while()
 * 				countdown.
 *
 * History: 	MJC - 6/12/2015
 *
 * Notes:
 *********************************************************/

#include <msp430.h>
#include <intrinsics.h>
#include <stdint.h>
#include <stdio.h>
/*
 * main.c
 */


/***************** Function Prototypes ********************/
void initLEDs(void);
void sosUpdate(void);


/************************ Main ****************************/
int main(void) {

	volatile int8_t i = 0;

    WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer

    // Initialize Peripherals
    initLEDs();

    // Main loop
    for(;;) {
    		volatile unsigned int i;	// volatile to prevent optimization

    		// Update the LED
    		sosUpdate();

    		// Countdown
    		i = 50000;					// SW Delay
    		do i--;
    		while(i != 0);
    	}

	
	return 0;
}




/*---------------------------------------------------------------------
  Function Name: initLEDs
  Description:   Initialize RED & GREEN LEDs
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void initLEDs(void) {

	// Init the GREEN LED
	P1DIR |= 0x01;
	// INIT the RED LED
	P1DIR |= 0x40;

	// Set as outputs
	P1OUT = 0x00;

	return;
}


/*---------------------------------------------------------------------
  Function Name: sosUpdate
  Description:   Increment through Morse Code SOS
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void sosUpdate(void){

	const int8_t sosvals[] = {0,0,0,0,1,0,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0};
	static int8_t i = 0;
	static int8_t j = 0;

	// Increment the value to be transmitted, wrap at max value
	if(++j == sizeof(sosvals)){
		j = 0;
		P1OUT ^= 0x01;
	}

	// Write to LED
	if(sosvals[j] == 1){
		P1OUT |= 0x40;
	} else {
		P1OUT &= 0xBF;
	}

}
