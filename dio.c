/*
 * dio.c
 *
 *  Created on: Feb 3, 2016
 *      Author: Jason
 */
#include <msp430.h>

void (*DIO_p10FunctPtr)();
void (*DIO_p11FunctPtr)();
void (*DIO_p12FunctPtr)();
void (*DIO_p13FunctPtr)();
void (*DIO_p14FunctPtr)();
void (*DIO_p15FunctPtr)();
void (*DIO_p16FunctPtr)();
void (*DIO_p17FunctPtr)();

void (*DIO_p26FunctPtr)();
void (*DIO_p27FunctPtr)();

__interrupt void DIO_port1IntFunct();
__interrupt void DIO_port2IntFunct();

#pragma vector=PORT1_VECTOR
__interrupt void DIO_port1IntFunct(){
	/* find out which pin triggered the interrupt */
	if(P1IFG & BIT0){
		/* if the function pointer points to an address,
		 * then execute that function call */
		if(DIO_p10FunctPtr != 0){
			(*DIO_p10FunctPtr)();
		}

		P1IFG &= ~BIT0;
	}else if(P1IFG & BIT1){
		if(DIO_p11FunctPtr != 0){
			(*DIO_p11FunctPtr)();
		}

		P1IFG &= ~BIT1;
	}else if(P1IFG & BIT2){
		if(DIO_p12FunctPtr != 0){
			(*DIO_p12FunctPtr)();
		}

		P1IFG &= ~BIT2;
	}else if(P1IFG & BIT3){
		if(DIO_p13FunctPtr != 0){
			(*DIO_p13FunctPtr)();
		}

		P1IFG &= ~BIT3;
	}else if(P1IFG & BIT4){
		if(DIO_p14FunctPtr != 0){
			(*DIO_p14FunctPtr)();
		}

		P1IFG &= ~BIT4;
	}else if(P1IFG & BIT5){
		if(DIO_p15FunctPtr != 0){
			(*DIO_p15FunctPtr)();
		}

		P1IFG &= ~BIT5;
	}else if(P1IFG & BIT6){
		if(DIO_p16FunctPtr != 0){
			(*DIO_p16FunctPtr)();
		}

		P1IFG &= ~BIT6;
	}else if(P1IFG & BIT7){
		if(DIO_p17FunctPtr != 0){
			(*DIO_p17FunctPtr)();
		}

		P1IFG &= ~BIT7;
	}
}

#pragma vector=PORT2_VECTOR
__interrupt void DIO_port2IntFunct(){
	/* find out which pin triggered the interrupt */
if(P2IFG & BIT6){
	if(DIO_p26FunctPtr != 0){
			(*DIO_p26FunctPtr)();
		}

		P2IFG &= ~BIT6;
	}else if(P2IFG & BIT7){
		if(DIO_p27FunctPtr != 0){
			(*DIO_p27FunctPtr)();
		}

		P2IFG &= ~BIT7;
	}else{
	    while(1); // programmer's trap
	}
}

void DIO_makeOutput(int port, int pin){
	/* use a mask */
	unsigned char  mask = (1 << pin);

	switch(port){
		case 1:
		{
			P1DIR |= mask;
			break;
		}
		case 2:
		{
			P2DIR |= mask;
			break;
		}

		default:
		{
			while(1); // programmer's trap
		}
	}
}

void DIO_makeInput(int port, int pin){
	/* use a mask */
	unsigned char  mask = ~(1 << pin);

	switch(port){
		case 1:
		{
			P1DIR &= mask;
			break;
		}

		case 2:
		{
			P2DIR &= mask;
			break;
		}

		default:
		{
			while(1); // programmer's trap
		}
	}
}

void DIO_enableInterrupt(int port, int pin, void (*functPtr)()){
	/* pins that interrupt should generally be inputs */
	DIO_makeInput(port, pin);

	switch(port){
		case 1:
		{
			unsigned char mask = 1 << pin;

			P1IFG &= ~mask;			// clear the interrupt flag
			P1IE |= mask;			// enable interupts on this pin
			__bis_SR_register(GIE);	// enable global interrupts

			/* associate the function pointer */
			switch(pin){
			    case 0: DIO_p10FunctPtr = functPtr; break;
			    case 1: DIO_p11FunctPtr = functPtr; break;
			    case 2: DIO_p12FunctPtr = functPtr; break;
			    case 3: DIO_p13FunctPtr = functPtr; break;
			    case 4: DIO_p14FunctPtr = functPtr; break;
			    case 5: DIO_p15FunctPtr = functPtr; break;
			    case 6: DIO_p16FunctPtr = functPtr; break;
			    case 7: DIO_p17FunctPtr = functPtr; break;
			    default: while(1);
			}

			break;
		}

		case 2:
		{
			unsigned char mask = 1 << pin;

			P2IFG &= ~mask;			// clear the interrupt flag
			P2IE |= mask;			// enable interupts on this pin
			__bis_SR_register(GIE);	// enable global interrupts

            /* associate the function pointer */
            switch(pin){
                case 6: DIO_p26FunctPtr = functPtr; break;
                case 7: DIO_p27FunctPtr = functPtr; break;
                default: while(1);
            }

			break;
		}

		default:
		{
			while(1); // programmer's trap
		}
	}
}

void DIO_disableInterrupt(int port, int pin){
	switch(port){
		case 1:
		{
			unsigned char mask = 1 << pin;
			P1IE &= ~mask;

			break;
		}

		case 2:
		{
			unsigned char mask = 1 << pin;
			P2IE &= ~mask;

			break;
		}

		default:
		{
			while(1); // programmer's trap
		}
	}
}
