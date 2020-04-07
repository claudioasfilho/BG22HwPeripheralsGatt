/***********************************************************************************************//**
 * \file   peripherals.h
 * \brief  All the Hardware Peripheral Functions
 * \author Claudio Filho
 ***************************************************************************************************
 * <b> (C) Copyright 2017 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include "em_timer.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "native_gecko.h"
#include "peripherals.h"
#include "em_usart.h"
#include "em_iadc.h"

/***********************************************************************************************//**
 * @addtogroup Peripheral Definitions
 * @{
 **************************************************************************************************/


#define UARTBUFFERSIZE 64
#define UART_SOF '$'
#define UART_EOF '\r'

#define LETIMERReload 1475 // (1/32768)*45ms




#define LEUSART0INT 0xf0
#define LETIMER0INT 0xf1

typedef union
        {

	struct{
		uint8_t			TXready:1;		//For UART Only - It indicates when to Transmit
	  	uint8_t			RXready:1;		//For UART Only - It indicates when data is available
	  	uint8_t			ADCSample:1;	//For ADC Only - It indicates there is a request to sample ADC
	  	uint8_t			Unused3:1;
	  	uint8_t			Unused4:1;
	  	uint8_t			Enabled:1;       // Is the task enabled or not?
	  	uint8_t			Finished:1;
	  	uint8_t			Status:1;		//Does it need attention?
	}bits;

	uint8_t all;
    } OBJFLAGS;


typedef union
		{

	struct{
		uint8_t			LED0:1;
		uint8_t			LED1:1;
		uint8_t			PB0:1;
		uint8_t			PB1:1;
		uint8_t			Unused1:1;
		uint8_t			Unused2:1;
		uint8_t			Unused3:1;
		uint8_t			Unused4:1;
		uint8_t			Unused5:1;
		uint8_t			Unused6:1;
		uint8_t			Unused7:1;
		uint8_t			Unused8:1;
		uint8_t			Unused9:1;
		uint8_t			Unused10:1;
		uint8_t			Unused11:1;
		uint8_t			Unused12:1;
	}bits;

	uint16_t all;
	} GPIOS;


typedef union
		{
	uint32_t value;
	uint8_t array[4];
	} ADCRESULT;


extern uint8_t UARTbuffer[UARTBUFFERSIZE];

enum {
		IADC_IRQFlag,

}softIRQ;

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/*******************************************************************************
 *******************************   PWM Definitions   ***********************************
 ******************************************************************************/

// Note: change this to set the desired Output frequency in Hz
#define PWM_FREQ 1000

// Buffer size
#define BUFFER_SIZE 11

// Note: change this to change the duty cycles used in this example
static const uint16_t dutyCyclePercentages[BUFFER_SIZE] =
    {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

// Buffer of duty cycle values for DMA transfer to CCVB
// Buffer is populated after TIMER is initialized and Top value is set
static uint16_t buffer[BUFFER_SIZE];


/*******************************************************************************
 *******************************  ADC Definitions   ***********************************
 ******************************************************************************/

// Set CLK_ADC to 10kHz (this corresponds to a sample rate of 1ksps)
#define CLK_SRC_ADC_FREQ        5000000  // CLK_SRC_ADC; largest division is by 4
#define CLK_ADC_FREQ            10000    // CLK_ADC; IADC_SCHEDx PRESCALE has 10 valid bits

// When changing GPIO port/pins above, make sure to change xBUSALLOC macro's
// accordingly.
#define IADC_INPUT_BUS          CDBUSALLOC
#define IADC_INPUT_BUSALLOC     GPIO_CDBUSALLOC_CDEVEN0_ADC0

/*******************************************************************************
 ***************************   GLOBAL VARIABLES   ******************************
 ******************************************************************************/

// Stores latest ADC sample and converts to volts
static volatile IADC_Result_t sample;
static volatile double singleResult;


/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/


void InitGPIO(void);
void GPIOHandler(void);



void SetLED0(void);
void ClearLED0(void);
void ToogleLED0(void);
uint8_t GetLED0(void);

void SetLED1(void);
void ClearLED1(void);
void ToogleLED1(void);
uint8_t GetLED1(void);

uint8_t GetPB0();
uint8_t GetPB1();

void InitPWM1(void);
void UpdatePWM1(uint8_t DutyCycle);
void PWMHandler(void);
uint8_t GetPWM1();


void InitLEUART0(void);
void UART_Tx(uint8_t *buffer, uint16_t size);
void UART_RXHandler(void);
void UART_TXHandler(void);
void ClearSOFReceived();

/*UART Messages*/

void LED0onUARTmessage(void);
void LED0offUARTmessage(void);
void LED1onUARTmessage(void);
void LED1offUARTmessage(void);


/*LETIMER0 */

void InitLETIMER0(void);

/*ADC0*/

void InitADC0(void);
void StartADC0Sample();
uint16_t GetADC0();
void softIADC_IRQHandler(void);

void InitPeripherals();


void Test_GPIO();

#endif /* PERIPHERALS_H_ */

