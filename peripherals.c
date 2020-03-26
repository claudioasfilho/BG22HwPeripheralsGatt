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
	GPIO_PinModeSet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, gpioModeInput, 1);
	UserGPIOs.bits.PB0 = 0;	//Logic Definition

	//UIF PB1 - PIN P6, EXP_HEADER9, UIF_BUTTON1 on WSTK with BRD4182 Radio Board
	/* Pin PF7 is configured to Input enabled with filter */
	GPIO_PinModeSet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN, gpioModeInput, 1);
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

#if 0
void initTimer(void)
{

	  GPIO_PinModeSet(gpioPortA, 6, gpioModePushPull, 0);
	  // Enable clock to GPIO and TIMER0
	  CMU_ClockEnable(cmuClock_GPIO, true);
	  CMU_ClockEnable(cmuClock_TIMER0, true);

	  // Initialize and start timer with no prescaling
	  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
	  // Configure TIMER0 Compare/Capture for output compare
	  TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;

	  // Use PWM mode, which sets output on overflow and clears on compare events
	  timerInit.enable = false;
	  timerCCInit.mode = timerCCModePWM;

	  // Configure but do not start the timer
	  TIMER_Init(TIMER0, &timerInit);

	  // Route Timer0 CC0 output to PA6
	  GPIO->TIMERROUTE[0].ROUTEEN  = GPIO_TIMER_ROUTEEN_CC0PEN;
	  GPIO->TIMERROUTE[0].CC0ROUTE = (gpioPortA << _GPIO_TIMER_CC0ROUTE_PORT_SHIFT)
	                    | (6 << _GPIO_TIMER_CC0ROUTE_PIN_SHIFT);

	  TIMER_InitCC(TIMER0, 0, &timerCCInit);

	  // Set top value to overflow at the desired PWM_FREQ frequency
	  TIMER_TopSet(TIMER0, CMU_ClockFreqGet(cmuClock_TIMER0) / PWM_FREQ);

	  // Start the timer
	  TIMER_Enable(TIMER0, true);

	  // Trigger DMA on compare event to set CCVB to update duty cycle on next period
	 // TIMER_IntEnable(TIMER0, TIMER_IF_CC0);

	PWMObj.all=0;
	PWMObj.bits.Enabled=1;

}

/**************************************************************************//**
* @brief
*    Initialize the LDMA module
*
* @details
*    Configure the channel descriptor to use the default peripheral to
*    memory transfer descriptor. Modify it to not generate an interrupt
*    upon transfer completion (we don't need it). Additionally, the transfer
*    configuration selects the TIMER0_CC0 signal as the trigger for the LDMA
*    transaction to occur.
*
* @note
*    The descriptor object needs to at least have static scope persistence so
*    that the reference to the object is valid beyond its first use in
*    initialization
*****************************************************************************/
void initLdma(void)
{
  // Start the transfer
  uint32_t channelNum = 0;

  // LDMA initialization
  LDMA_Init_t init = LDMA_INIT_DEFAULT;
  LDMA_Init(&init);

  // Transfer configuration and trigger selection
  LDMA_TransferCfg_t transferConfig =
    LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_TIMER0_CC0);

  // Channel descriptor configuration
  static LDMA_Descriptor_t descriptor =
    LDMA_DESCRIPTOR_LINKREL_M2P_BYTE(&buffer,            // Memory source address
                                    &TIMER0->CC[0].OCB, // Peripheral destination address
                                    BUFFER_SIZE,         // Number of bytes per transfer
                                    0);                  // Link to same descriptor
  descriptor.xfer.size     = ldmaCtrlSizeHalf; // Unit transfer size
  descriptor.xfer.doneIfs  = 0;                // Don't trigger interrupt when done

  LDMA_StartTransfer(channelNum, &transferConfig, &descriptor);
}

/**************************************************************************//**
 * @brief
 *    Populate buffer with timer duty cycle values
 *****************************************************************************/
void populateBuffer(void)
{
  for (uint32_t i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = (uint16_t) (TIMER_TopGet(TIMER0) * dutyCyclePercentages[i] / 100);
  }
}
#endif

void InitPWM1()
{

		  // Enable clock to GPIO and TIMER0
		  CMU_ClockEnable(cmuClock_GPIO, true);
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


#if 0

/**************************************************************************//**
 * @brief LETIMER Related Functions and variables
 *
 *****************************************************************************/

void LETIMER0_IRQHandler(void)
{
    LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF);

	LETIMER0->CNT = LETIMERReload;	//Reloads the Timer counter

    gecko_external_signal(LETIMER0INT);

}

void InitLETIMER0(void)
{

		CMU_ClockEnable(cmuClock_LETIMER0, true);


		LETIMER_Init_TypeDef letimerInit = LETIMER_INIT_DEFAULT;


		LETIMER0->CNT= LETIMERReload; //Timer In 16bit Count Down Mode. Reload value represents (32768 Khz time base)

		LETIMER_Init(LETIMER0, &letimerInit);

		LETIMER_IntEnable	(LETIMER0, LETIMER_IEN_UF); //LETIMER Underflow Interrupt enabled

		LETIMER_Enable(LETIMER0, true);

		NVIC_ClearPendingIRQ(LETIMER0_IRQn);
		NVIC_EnableIRQ(LETIMER0_IRQn);

}
#endif

/****************************************************************************
 * @brief ADC0 Related Functions and variables
 *
 *****************************************************************************/
static uint32_t ADCresult=0;


#ifdef SERIES01
void InitADC0(void) {

	// $[ADC0_Init]
//	ADC_Init_TypeDef ADC0_init = ADC_INIT_DEFAULT;

	/* Enable clock for ADC0 */
	CMU_ClockEnable(cmuClock_ADC0, true);


	  /* Base the ADC configuration on the default setup. */
	  ADC_Init_TypeDef       init  = ADC_INIT_DEFAULT;
	  ADC_InitSingle_TypeDef sInit = ADC_INITSINGLE_DEFAULT;

	  /* Initialize timebases */
	  init.timebase = ADC_TimebaseCalc(0);
	  init.prescale = ADC_PrescaleCalc(380000, 0);
	  ADC_Init(ADC0, &init);

	  /* Set input to temperature sensor. Reference must be 1.25V */
	  sInit.reference   = adcRefVDD;
	  sInit.acqTime     = adcAcqTime8; /* Minimum time for temperature sensor */
	  //sInit.posSel      = adcPosSelTEMP;
	  sInit.posSel = adcPosSelAPORT3XCH4;
	  sInit.negSel = adcNegSelVSS;
	  ADC_InitSingle(ADC0, &sInit);

	ADCresult=0;
	ADCObj.all =0;
	ADCObj.bits.Enabled=1;

}

#endif

#if ADCTimed
void ADC_Handler(void)
{
	if(ADCObj.bits.Enabled==1)
	{
		if (ADCObj.bits.ADCSample==1)
		{
			ADCresult = ADC_DataSingleGet(ADC0);
			ADCObj.bits.ADCSample=0;
		}
	}
}

void StartADC0Sample()
{
	ADCObj.bits.ADCSample=1;
}

uint32_t GetADC0()
{
	return ADCresult;
}
#endif


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

  // Set conversions to run continuously
  initSingle.triggerAction = iadcTriggerActionContinuous;

  // Configure Input sources for single ended conversion
  initSingleInput.posInput = iadcPosInputPortCPin4;
  initSingleInput.negInput = iadcNegInputGnd;

  // Initialize IADC
  IADC_init(IADC0, &init, &initAllConfigs);

  // Initialize Scan
  IADC_initSingle(IADC0, &initSingle, &initSingleInput);

  // Allocate the analog bus for ADC0 inputs
  GPIO->IADC_INPUT_BUS |= IADC_INPUT_BUSALLOC;

  // Enable interrupts on data valid level
  IADC_enableInt(IADC0, IADC_IF_SINGLEFIFODVL);

  // Enable ADC interrupts
  NVIC_ClearPendingIRQ(IADC_IRQn);
  NVIC_EnableIRQ(IADC_IRQn);
}

/**************************************************************************//**
 * @brief  ADC Handler
 *****************************************************************************/
void IADC_IRQHandler(void)
{
  GPIO_PinOutToggle(BSP_LED0_PORT, BSP_LED0_PIN);

  // Read data from the FIFO
  sample = IADC_pullSingleFifoResult(IADC0);

  // For single-ended the result range is 0 to +Vref, i.e., 12 bits for the
  // conversion value.
  singleResult = sample.data * 3.3 / 0xFFF;

  IADC_clearInt(IADC0, IADC_IF_SINGLEFIFODVL);
}

uint32_t GetADC0()
{
#ifdef SERIES01
	  ADC_Start(ADC0, adcStartSingle);
	  while ( ( ADC0->STATUS & ADC_STATUS_SINGLEDV ) == 0 ){}
	  return ADC_DataSingleGet(ADC0);
#else
	  return sample.data;
#endif

}

void InitPeripherals()
{
	  // Initialize the IADC
	//    initIADC();

	    // Start single
	  //  IADC_command(IADC0, iadcCmdStartSingle);

		InitGPIO();
	    InitPWM1();
	    UpdatePWM1(70);
	    ChangePWMoutput();
	    // Initialize DMA only after buffer is populated
	   // populateBuffer();
	    //initLdma();

	    while (1) {
	      EMU_EnterEM1(); // Enter EM1 (won't exit)
	    }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
