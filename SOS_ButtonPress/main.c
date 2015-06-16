/**********************************************************
 * Pushbutton SOS Blinky
 *********************************************************/

/*********************************************************
 * File:  		main.c
 * Author: 		Matt Casari
 * Description: This file contains code to blink the green
 * 				LED in a Morse Code SOS after the button is pushed
 * 				on the MSP430 Launchpad with no interrupts driven
 * 				directly from main loop	with while() countdown.
 *
 * History: 	MJC - 6/12/2015
 *
 * Notes:
 *********************************************************/

#include <msp430.h>
#include <intrinsics.h>
#include <stdint.h>
#include <stdio.h>


#define RED_LED 	BIT0
#define GREEN_LED	BIT6
#define PUSH_BUTTON	BIT3
/*
 * main.c
 */


/***************** Function Prototypes ********************/
void initGPIO(void);
void sosUpdate(void);


/********************** Globals ***************************/
volatile int8_t ButtonFlag = 0;

/************************ Main ****************************/
int main(void) {

	volatile int8_t i = 0;

    WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer

    // Initialize Peripherals
    initGPIO();

    // Enable Global Interrupts
    _BIS_SR(GIE);

    // Main loop
    for(;;) {
    		volatile unsigned int i;	// volatile to prevent optimization

    		// Update the LED
    		if(ButtonFlag == 1) {
    			sosUpdate();
    		}
    		// Countdown
    		i = 25000;					// SW Delay
    		do i--;
    		while(i != 0);
    	}

	
	return 0;
}

/*---------------------------------------------------------------------
  Function Name: initGPIO
  Description:   Initialize RED & GREEN LEDs and the Push Button
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void initGPIO(void) {

	// Init Green & Red LEDs
	P1DIR |= RED_LED + GREEN_LED;
	// Set as outputs & Inputs
	P1OUT |= PUSH_BUTTON;

	P1REN |= PUSH_BUTTON;

	// Set Intterupt on Push Button
	P1IE |= PUSH_BUTTON;

	// Clear Interrupt Flag
	P1IFG &= ~PUSH_BUTTON;

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

		// Reset the ButtonFlag
		ButtonFlag = 0x00;

	}

	// Write to LED
	if(sosvals[j] == 1){
		P1OUT |= GREEN_LED;
	} else {
		P1OUT &= ~GREEN_LED;
	}
}


/*---------------------------------------------------------------------
  Interrupt Name: PORT1_ISR
  Description:   Interrupt on Push-Button Press
-----------------------------------------------------------------------*/
__attribute__((interrupt (PORT1_VECTOR)))
void PORT1_ISR() {

	P1OUT ^= RED_LED;  // Toggle the RED LED

	// Set the Flag
	ButtonFlag = 0x01;

	// Clear Interrupt
	P1IFG &= ~PUSH_BUTTON;

	//
	P1IES ^= PUSH_BUTTON;

	// Exit isr
}
