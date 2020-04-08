/***********************************************************************************************//**
 * \file   peripherals.c
 * \brief  All the Hardware Peripheral Functions
 * \author Claudio Filho
 ***************************************************************************************************
 * <b> (C) Copyright 2017 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/*Application Specific Code*/

#include "em_chip.h"
#include "em_timer.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "native_gecko.h"
#include "peripherals.h"
#include "em_usart.h"
#include "em_iadc.h"
#include "em_ldma.h"
#include <string.h>
#include <stdio.h>

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif


OBJFLAGS PWMObj;
OBJFLAGS UARTObj;
OBJFLAGS ADCObj;
GPIOS UserGPIOs;




/**************************************************************************//**
 * @brief GPIO Related Functions  and variables
 *
 *****************************************************************************/
//uint8_t test[256];

void InitGPIO(void) {

	// $[Port A Configuration]

	/*UART Function Related GPIOs*/

	CMU_ClockEnable(cmuClock_GPIO, true);

	/*PWM Function Related GPIOs*/

	//PWM Output - PIN P9 and EXP_HEADER12 on WSTK with BRD4182 Radio Board
	GPIO_PinModeSet(PWM_PORT, PWM_PIN, gpioModePushPull, 0);

	/*ADC Function Related GPIOs*/

	//ADC Input - PIN P11 and EXP_HEADER14 on WSTK with BRD4182 Radio Board - PD12

	//GPIO_PinModeSet(gpioPortA, 6, gpioModeInput, 0);

	/*General Purpose Function Related GPIOs*/

	//UIF LED0 - PIN P8, UIF_LED0, EXP_HEADER11 on WSTK with BRD4182 Radio Board
	/* Pin PF4 is configured to Push-pull */
	GPIO_PinModeSet(BSP_LED0_PORT, BSP_LED0_PIN, gpioModePushPull, 0);
	UserGPIOs.bits.LED0 = 0;	//Logic Definition

	//UIF LED1 - PIN P10, UIF_LED1, EXP_HEADER13 on WSTK with BRD4182 Radio Board
	/* Pin PF5 is configured to Push-pull */
	GPIO_PinModeSet(BSP_LED1_PORT, BSP_LED1_PIN, gpioModePushPull, 0);
	UserGPIOs.bits.LED1 = 0;	//Logic Definition

	//UIF PB0 - PIN P4, EXP_HEADER7, UIF_BUTTON0 on WSTK with BRD4182 Radio Board
	/* Pin PF6 is configured to Input enabled with filter */

	GPIO_PinModeSet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, gpioModeInput, 0);
	UserGPIOs.bits.PB0 = 0;	//Logic Definition

	//UIF PB1 - PIN P6, EXP_HEADER9, UIF_BUTTON1 on WSTK with BRD4182 Radio Board
	/* Pin PF7 is configured to Input enabled with filter */
	GPIO_PinModeSet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN, gpioModeInput, 0);
	UserGPIOs.bits.PB1 = 0;	//Logic Definition

	// [Port F Configuration]$

	/*Other GPIOs*/


	/* Test GPIO */
//	GPIO_PinModeSet(gpioPortD, 10, gpioModePushPull, 1);

}

void SetLED0(void)
{
	UserGPIOs.bits.LED0=1;
}
void ClearLED0(void)
{
	UserGPIOs.bits.LED0=0;
}
void ToogleLED0(void)
{
	UserGPIOs.bits.LED0^=1;
}
uint8_t GetLED0()
{
	return UserGPIOs.bits.LED0;
}


void SetLED1(void)
{
	UserGPIOs.bits.LED1=1;
}
void ClearLED1(void)
{
	UserGPIOs.bits.LED1=0;
}
void ToogleLED1(void)
{
	UserGPIOs.bits.LED1^=1;
}

uint8_t GetLED1()
{
	return UserGPIOs.bits.LED1;
}

uint8_t GetPB0()
{
	return UserGPIOs.bits.PB0;
}

uint8_t GetPB1()
{
	return UserGPIOs.bits.PB1;
}

void GPIOHandler(void)
{
	if (UserGPIOs.bits.LED0 != GPIO_PinOutGet(BSP_LED0_PORT, BSP_LED0_PIN))
		{
			if (UserGPIOs.bits.LED0==1)  GPIO_PinOutSet(BSP_LED0_PORT, BSP_LED0_PIN);
			else  GPIO_PinOutClear(BSP_LED0_PORT, BSP_LED0_PIN);
		}

	if (UserGPIOs.bits.LED1 != GPIO_PinOutGet(BSP_LED1_PORT, BSP_LED1_PIN))
		{
			if (UserGPIOs.bits.LED1==1)  GPIO_PinOutSet(BSP_LED1_PORT, BSP_LED1_PIN);
			else  GPIO_PinOutClear(BSP_LED1_PORT, BSP_LED1_PIN);
		}

	if(GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN)==0) UserGPIOs.bits.PB0=1;
	else UserGPIOs.bits.PB0=0;

	if(GPIO_PinInGet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN)==0) UserGPIOs.bits.PB1=1;
	else UserGPIOs.bits.PB1=0;

}

/**************************************************************************//**
 * @brief PWM Related Functions  and variables
 *
 *****************************************************************************/
static uint8_t desiredDutyCycle;


void InitPWM1()
{

		  // Enable clock to GPIO and TIMER0

		  CMU_ClockEnable(cmuClock_TIMER1, true);


	// $[TIMER1 I/O setup]
	/* Set up CC0 */
		  // Route TIMER1 CC0 output to PA6
		  GPIO->TIMERROUTE[1].ROUTEEN  = GPIO_TIMER_ROUTEEN_CC0PEN;
		  GPIO->TIMERROUTE[1].CC0ROUTE = (PWM_PORT << _GPIO_TIMER_CC0ROUTE_PORT_SHIFT)
		                    | (PWM_PIN << _GPIO_TIMER_CC0ROUTE_PIN_SHIFT);

	TIMER_Init_TypeDef init = TIMER_INIT_DEFAULT;

	init.enable = 0;
	init.debugRun = 1;
	init.dmaClrAct = 0;
	init.sync = 0;
	init.clkSel = timerClkSelHFPerClk;
	init.prescale = timerPrescale1;
	init.fallAction = timerInputActionNone;
	init.riseAction = timerInputActionNone;
	init.mode = timerModeUp;
	init.quadModeX4 = 0;
	init.oneShot = 0;
	init.count2x = 0;
	init.ati = 0;
	TIMER_Init(TIMER1, &init);
	// [TIMER1 initialization]$

	// $[TIMER1 CC0 init]
	TIMER_InitCC_TypeDef initCC0 = TIMER_INITCC_DEFAULT;

	initCC0.prsInput = false;
	initCC0.prsSel = timerPRSSELCh0;
	initCC0.edge = timerEdgeRising;
	initCC0.mode = timerCCModePWM;
	initCC0.eventCtrl = timerEventEveryEdge;
	initCC0.filter = 0;
	initCC0.cofoa = timerOutputActionNone;
	initCC0.cufoa = timerOutputActionNone;
	initCC0.cmoa = timerOutputActionNone;
	initCC0.coist = 0;
	initCC0.outInvert = 0;
	TIMER_InitCC(TIMER1, 0, &initCC0);
	// [TIMER1 CC0 init]$0

	//PWM Output - PIN P7 on WSTK

	GPIO_PinModeSet(PWM_PORT, PWM_PIN, gpioModePushPull, 0);

	PWMObj.all=0;
	PWMObj.bits.Enabled=1;

}


void UpdatePWM1(uint8_t DutyCycle) //desiredDutyCycle varies from 0-100;
{
	desiredDutyCycle = DutyCycle;
	PWMObj.bits.Status=1;
}

void ChangePWMoutput() //desiredDutyCycle varies from 0-100;
{
	uint32_t PWMFrequency;
	uint32_t CC1DutyCycle;


	PWMFrequency = BSP_CLK_HFXO_FREQ / PWM_FREQ;

	if (desiredDutyCycle>0)
	{

		CC1DutyCycle = (desiredDutyCycle*PWMFrequency)/100;
		CC1DutyCycle =CC1DutyCycle ;

	}
	else  {CC1DutyCycle=0;}
	if (desiredDutyCycle>100) CC1DutyCycle=99;


	TIMER_Enable(TIMER1,0);	//Disables Timer

	/* set PWM period */
	TIMER_TopSet (TIMER1, PWMFrequency);

	/* Set PWM duty cycle to 50% */
	TIMER_CompareSet (TIMER1, 0, CC1DutyCycle);

	CC1DutyCycle = CC1DutyCycle;

	TIMER_Enable(TIMER1,1);

}

void PWMHandler(void)
{
	if( (PWMObj.bits.Enabled==1) && (PWMObj.bits.Status==1))
	{
		ChangePWMoutput();
		PWMObj.bits.Status=0;
	}

}

uint8_t GetPWM1()
{
	return desiredDutyCycle;
}


/****************************************************************************
 * @brief ADC0 Related Functions and variables
 *
 *****************************************************************************/
static IADC_Result_t ADCresult;

/**************************************************************************//**
 * @brief  IADC Initializer
 *****************************************************************************/
void initIADC (void)
{
  // Declare init structs
  IADC_Init_t init = IADC_INIT_DEFAULT;
  IADC_AllConfigs_t initAllConfigs = IADC_ALLCONFIGS_DEFAULT;
  IADC_InitSingle_t initSingle = IADC_INITSINGLE_DEFAULT;
  IADC_SingleInput_t initSingleInput = IADC_SINGLEINPUT_DEFAULT;

  // Enable IADC clock
  CMU_ClockEnable(cmuClock_IADC0, true);

  // Reset IADC to reset configuration in case it has been modified
  IADC_reset(IADC0);

  // Configure IADC clock source for use while in EM2
  CMU_ClockSelectSet(cmuClock_IADCCLK, cmuSelect_FSRCO);

  // Modify init structs and initialize
  init.warmup = iadcWarmupKeepWarm;

  // Set the HFSCLK prescale value here
  init.srcClkPrescale = IADC_calcSrcClkPrescale(IADC0, CLK_SRC_ADC_FREQ, 0);

  // Configuration 0 is used by both scan and single conversions by default
  // Use unbuffered AVDD as reference
  initAllConfigs.configs[0].reference = iadcCfgReferenceVddx;

  // Divides CLK_SRC_ADC to set the CLK_ADC frequency for desired sample rate
  initAllConfigs.configs[0].adcClkPrescale = IADC_calcAdcClkPrescale(IADC0,
                                                                    CLK_ADC_FREQ,
                                                                    0,
                                                                    iadcCfgModeNormal,
                                                                    init.srcClkPrescale);

  // Single initialization
  initSingle.dataValidLevel = _IADC_SINGLEFIFOCFG_DVL_VALID1;

  // Set conversions to run once
  initSingle.triggerAction = iadcTriggerActionOnce;

  // Configure Input sources for single ended conversion GPIO port C pin 2

  initSingleInput.posInput = iadcPosInputPortCPin2;
  initSingleInput.negInput = iadcNegInputGnd;

  // Initialize IADC
  IADC_init(IADC0, &init, &initAllConfigs);

  // Initialize Scan
  IADC_initSingle(IADC0, &initSingle, &initSingleInput);

  // Allocate the analog bus for ADC0 inputs
  GPIO->IADC_INPUT_BUS |= IADC_INPUT_BUSALLOC;

#if ADCINTENABLED
  // Enable interrupts on data valid level
  IADC_enableInt(IADC0, IADC_IF_SINGLEFIFODVL);

  // Enable ADC interrupts
  NVIC_ClearPendingIRQ(IADC_IRQn);
  NVIC_EnableIRQ(IADC_IRQn);

#endif

  ADCObj.bits.ADCSample=0;
}


/**************************************************************************//**
 * @brief  ADC Handler
 *****************************************************************************/
void IADC_IRQHandler(void)
{
	gecko_external_signal(IADC_IRQFlag);
}

void softIADC_IRQHandler(void)
{

  // Read data from the FIFO
	ADCresult = IADC_pullSingleFifoResult(IADC0);

  // For single-ended the result range is 0 to +Vref, i.e., 12 bits for the
  // conversion value.
  //singleResult = sample.data * 3.3 / 0xFFF;

  ADCObj.bits.ADCSample=1;

  IADC_clearInt(IADC0, IADC_IF_SINGLEFIFODVL);

}

void ADCSampleReady()
{
	ADCObj.bits.ADCSample=1;
}

void StartADC0Sample()
{
	IADC_command(IADC0, iadcCmdStartSingle);
}

uint16_t GetADC0()
{
	ADCObj.bits.ADCSample=0;

	ADCresult = IADC_pullSingleFifoResult(IADC0);
	return ADCresult.data;
}

void Test_GPIO()
{
    /* Enable GPIO clock */
      CMU_ClockEnable(cmuClock_GPIO, true);

      /* Configure Push Button 0 as input*/
      GPIO_PinModeSet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, gpioModeInput, 0);

      /* Configure LED0 as a push pull for LED drive */
      GPIO_PinModeSet(BSP_LED0_PORT, BSP_LED0_PIN, gpioModePushPull, 1);

      while (1)
      {
        /* Check if button is pressed - when pressed, value will be 0 */
        if (!GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN))
        {
          GPIO_PinOutSet(BSP_LED0_PORT, BSP_LED0_PIN);
        }
        else
        {
          GPIO_PinOutClear(BSP_LED0_PORT, BSP_LED0_PIN);
        }
      }
}



void InitPeripherals()
{

#if 1
	  // Initialize the IADC
	    initIADC();
	    // Start single
	    StartADC0Sample();

		InitGPIO();
	    InitPWM1();
	    UpdatePWM1(70);
	    ChangePWMoutput();
	    /* Enable GPIO clock */


#else

	    Test_GPIO();
#endif

}





/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
