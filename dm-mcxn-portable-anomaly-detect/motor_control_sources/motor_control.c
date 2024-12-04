/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <state_machine.h>
#include "motor_control.h"
#include "events_init.h"
#include "fsl_lptmr.h"
#include "fsl_clock.h"
#include "board.h"
#include "stdbool.h"
#include "gui_guider.h"
#include "fsl_pwm.h"

//#include "portmacrocommon.h"


#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }

#define PCR_IBE_ibe1 0x01u

#ifndef pdMSTO_TICKS
#define pdMSTO_TICKS(xTimeInMs)   ((TickType_t)(((TickType_t)\
								  (xTimeInMs) * (TickType_t) configTICK_RATE_HZ) / (TickType_t)1000))
#endif

#define DEMO_LPTMR_BASE      LPTMR0
#define LPTMR_USEC_COUNT     1000000
#define LPTMR_50_USEC_COUNT  50
#define DEMO_LPTMR_IRQn      LPTMR0_IRQn
#define LPTMR_LED_HANDLER    LPTMR0_IRQHandler
#define LPTMR_SOURCE_CLOCK   (12000000)
#define LED_INIT()           LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE()         LED_RED_TOGGLE()
#define PWM_SIGNAL_ON()      GPIO_PortSet(BOARD_MIKROE_NATIVE_PWM_GPIO, 1U << BOARD_MIKROE_NATIVE_PWM_PIN)  /*!< Turn on ME PWM signal */
#define PWM_SIGNAL_OFF()     GPIO_PortClear(BOARD_MIKROE_NATIVE_PWM_GPIO, 1U << BOARD_MIKROE_NATIVE_PWM_PIN)    /*!< Turn off ME PWM signal */
/*!< Turn on MIKROE CS pin - High */
#define MIKROE_CS_ON()       GPIO_PortSet(BOARD_MIKROE_NATIVE_CS_GPIO, 1U << BOARD_MIKROE_NATIVE_CS_PIN)
#define TURN_OFF_ESC_RELAY() MIKROE_CS_ON()
/*!< Turn off MIKROE CS pin - Low */
#define MIKROE_CS_OFF()      GPIO_PortClear(BOARD_MIKROE_NATIVE_CS_GPIO, 1U << BOARD_MIKROE_NATIVE_CS_PIN)
#define TURN_ON_ESC_RELAY()  MIKROE_CS_OFF()
#define PWM_INIT()           MIKROE_NATIVE_PWM_INIT(0)  //Zero by default - Un-balanced motor
#define MIKROE_CS_INIT()     MIKROE_NATIVE_CS_INIT(0)  //Zero by default - Balanced motor
#define PWM_SIGNAL_FREQ      50U

/* PWM Configuration */
/* The PWM base address */
#define BOARD_PWM_BASEADDR        PWM1
#define PWM_SRC_CLK_FREQ          CLOCK_GetFreq(kCLOCK_BusClk)
#define DEMO_PWM_FAULT_LEVEL      true
#define APP_DEFAULT_PWM_FREQUENCE (50U)   /* Set the PWM Frequency */
#define APP_DEFAULT_PWM_PERIOD    (20U)   /* Set the PWM Period */
#define DEMO_PWM_CLOCK_DEVIDER    kPWM_Prescale_Divide_128


/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile static uint32_t g_rpm = 0;

demo_mode_t demo_mode = demo_idle; //by default
sm_app_state_t motor_sm;

/* LPTMR Variables */
volatile uint32_t lptmrCounter = 0U;
lptmr_config_t lptmrConfig;
volatile uint32_t g_systickCounter;
TickType_t currentTick;

extern lv_ui guider_ui; /* declared in the main file. */

bool state_started = false; /* use only if one PWM pulse is needed per speed. */
bool stop_manual;
bool stop_flow_control = false;

void ESC_Motor_Init(void)
{
	BOARD_Init_MotorPins();

    motor_sm = kSM_App_Idle;  /* Motor is Idle */

	MIKROE_CS_INIT();  // High by default de-energize the ESC
	MIKROE_CS_OFF(); /* Confirm Low Level on CS pin */

}

void BalancedMotor_ON(void)
{
	MIKROE_CS_ON();
}

void BalancedMotor_OFF(void)
{
	MIKROE_CS_OFF();
}

void UnBalancedMotor_ON(void)
{
	//When PIO3_13 is configured as PIO and not as PWMX
	PWM_SIGNAL_ON();
}

void UnBalancedMotor_OFF(void)
{
	//When PIO3_13 is configured as PIO and not as PWMX
	PWM_SIGNAL_OFF();

}

void All_Motors_OFF(void)
{
	MIKROE_CS_OFF();
	PWM_SIGNAL_OFF();

}

void PWMX_DRV_Init(void)
{
    uint16_t deadTimeVal;
    pwm_signal_param_t pwmSignal[2];
    uint32_t pwmSourceClockInHz;
    uint32_t pwmFrequencyInHz = APP_DEFAULT_PWM_FREQUENCE;

    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

    /* Set deadtime count, we set this to about 650ns */
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;

    pwmSignal[0].pwmChannel       = kPWM_PwmX;
    pwmSignal[0].level            = kPWM_HighTrue;
    pwmSignal[0].dutyCyclePercent = 5.5f; /* 5.5 percent duty cycle initial - 1.1 ms */
    pwmSignal[0].deadtimeValue    = deadTimeVal;
    pwmSignal[0].faultState       = kPWM_PwmFaultState0;
    pwmSignal[0].pwmchannelenable = true;

    // Setup PWM1 Submodule 1, PWM_X signal
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_1, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

}

void LPTMR_LED_HANDLER(void)
{
    LPTMR_ClearStatusFlags(DEMO_LPTMR_BASE, kLPTMR_TimerCompareFlag);
    lptmrCounter++;
    LED_TOGGLE();
    /*
     * Workaround for TWR-KV58: because write buffer is enabled, adding
     * memory barrier instructions to make sure clearing interrupt flag completed
     * before go out ISR
     */
    __DSB();
    __ISB();
}

void motor_control_init()
{
	uint32_t ui32PrimaskReg = DisableGlobalIRQ();

    //Initialize pins for Relay and Fan.
	BOARD_Init_MotorPins();

    motor_sm = kSM_App_Idle;  /* Fan is Idle */

    // Configure LPTMR
    // ADDD LPTM TIMER FUNCTION AND CONFIG ******************************
   // PWM_INIT(); //Starts at zero by default
   // PWM_SIGNAL_OFF();

	MIKROE_CS_INIT();  // High by default de-energize the ESC
	MIKROE_CS_ON(); /* Confirm High Level on CS pin */

	/* 12MHZ clock enabled for LPTMR */
	SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO12MHZ_ENA(1);

	/* Configure LPTMR */
	/*
	 * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
	 * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
	 * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
	 * lptmrConfig.enableFreeRunning = false;
	 * lptmrConfig.bypassPrescaler = true;
	 * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
	 * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
	 */
	LPTMR_GetDefaultConfig(&lptmrConfig);
	lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_0;  //FRO_12MHz

	/* Initialize the LPTMR */
	LPTMR_Init(DEMO_LPTMR_BASE, &lptmrConfig);

	/*
	 * Set timer period.
	 * 50us resolution timer ---> using the FRO12MHZ clock
	 */
	LPTMR_SetTimerPeriod(DEMO_LPTMR_BASE, USEC_TO_COUNT(LPTMR_50_USEC_COUNT, LPTMR_SOURCE_CLOCK));

	/* Enable timer interrupt */
	LPTMR_EnableInterrupts(DEMO_LPTMR_BASE, kLPTMR_TimerInterruptEnable);

	/* Enable at the NVIC */
	EnableIRQ(DEMO_LPTMR_IRQn);

	/* For delay */
	//SysTick_Config(SystemCoreClock / 1000U);

    EnableGlobalIRQ(ui32PrimaskReg);
}

void flexPWM_init()
{
	/* Structure of initialize PWM */
	pwm_config_t pwmConfig;
	pwm_fault_param_t faultConfig;
	uint32_t pwmVal = 10;

    /* Enable PWM1 SUB Clockn */
    /* PWM_X1 is localized on Sub-Module1, so it requires CLK0_EN = 1 */
    SYSCON->PWM1SUBCTL |=
        (SYSCON_PWM1SUBCTL_CLK0_EN_MASK | SYSCON_PWM1SUBCTL_CLK1_EN_MASK );

    /*
     * pwmConfig.enableDebugMode = false;
     * pwmConfig.enableWait = false;
     * pwmConfig.reloadSelect = kPWM_LocalReload;
     * pwmConfig.clockSource = kPWM_BusClock;
     * pwmConfig.prescale = kPWM_Prescale_Divide_1;
     * pwmConfig.initializationControl = kPWM_Initialize_LocalSync;
     * pwmConfig.forceTrigger = kPWM_Force_Local;
     * pwmConfig.reloadFrequency = kPWM_LoadEveryOportunity;
     * pwmConfig.reloadLogic = kPWM_ReloadImmediate;
     * pwmConfig.pairOperation = kPWM_Independent;
     */
    PWM_GetDefaultConfig(&pwmConfig);

    /* Set prescaler = 1:128*/
#ifdef DEMO_PWM_CLOCK_DEVIDER
    pwmConfig.prescale = DEMO_PWM_CLOCK_DEVIDER;
#endif

    /* PWM X Submodule 1 Independent - PIO3_19 */
    pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
    pwmConfig.pairOperation   = kPWM_Independent;
    pwmConfig.enableDebugMode = true; /* PWM running */
    pwmConfig.clockSource           = kPWM_BusClock;    //kPWM_BusClock

    /* Set localsync for submodule 1*/
    pwmConfig.initializationControl = kPWM_Initialize_LocalSync;

    /* Initialize submodule 1, make it use same counter clock as submodule 0. */
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_1, &pwmConfig) == kStatus_Fail)
    {
        //PRINTF("PWM initialization failed\n");
        //return 1;
    }

    /*
     *   config->faultClearingMode = kPWM_Automatic;
     *   config->faultLevel = false;
     *   config->enableCombinationalPath = true;
     *   config->recoverMode = kPWM_NoRecovery;
     */
    PWM_FaultDefaultConfig(&faultConfig);

#ifdef DEMO_PWM_FAULT_LEVEL
    faultConfig.faultLevel = DEMO_PWM_FAULT_LEVEL;
#endif

    /* Sets up the PWM fault protection */
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_0, &faultConfig);
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_1, &faultConfig);
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_2, &faultConfig);
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_3, &faultConfig);

    /* Set PWM fault disable mapping for submodule 1 */
    PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmX, kPWM_faultchannel_0,
                             kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);

    /* Call the init function with demo configuration */
    PWMX_DRV_Init();

    /* Set the load okay bit for submodule 1 to load registers from their buffer */
    PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_1 , true);

    /* Start the PWM generation from Submodule 1 */
  //  PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_1 );

    /* Update duty cycles for all 3 PWM signals */
    PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmX, kPWM_SignedCenterAligned, pwmVal); //>> 1));


}

void motor_control_task()
{
	switch (demo_mode)
	{
	case demo_stop: /* Demo stop sequence */
		Stop_DemoSeq();

		break;

	case demo_train: /* Demo train sequence */
		BalancedMotor_ON();

		break;

	case demo_run:  /* Demo run sequence */
		/* runs demo sequence forever, until stop or manual functions are called. */
		Run_DemoSeq();

		break;

	default:
		;  /* demo_idle */
		break;

	}

}

void PWM_pulse_start(void)
{
	/* Start the PWM generation from Submodule 1 */
	PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_1 );
}

void PWM_pulse_stop(void)
{
	/* Stop the PWM generation from Submodule 1 */
	PWM_StopTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_1 );

	/* Make sure the GPIO output is driven to Low when the PWM is in STOP mode */
	BOARD_PWMpin_as_GPIO();
	UnBalancedMotor_OFF();  //PWM signal goes to Low

}


void PWM_pulse_ms(float duty_cycle_ms)
{
	float d_percent;

	d_percent = (duty_cycle_ms * 100)/ APP_DEFAULT_PWM_PERIOD;
	PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmX, kPWM_SignedCenterAligned, d_percent);

	/* Below not supported on SDK v2.14 yet. */
	PWM_SetOutputToIdle(BOARD_PWM_BASEADDR, kPWM_PwmX, kPWM_Module_1, false);  //Low at idle


	/* Set the load okay bit for all submodules to load registers from their buffer */
	PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_1 , true);

}

/* Get the demo mode from LVGL GUI
   Returns: temp_type */
void get_demo_mode(demo_mode_t *mode)
{
   *mode = demo_mode;
}

/* Get the demo mode from LVGL GUI
   Returns: temp_type */
void set_demo_mode(demo_mode_t mode){

   demo_mode = mode;

}

void Run_DemoSeq(void)
{
	switch (motor_sm)
	{
		/* Fan is Idle */
		case kSM_App_Idle: /* CS pin goes to High, de-energizes relay. Reaffirm Level on CS pin */
			All_Motors_OFF(); /* Ensure all motors are OFF at this point */
			motor_sm = kSM_App_Demo_IdleTime;

			currentTick = xTaskGetTickCount(); /* Get the tick before jumping cases */

			break;

		case kSM_App_Demo_IdleTime: /* keep last known state */

			if (!state_started)
			{ /* Entry one time only */
				state_started = true;

			}

			if(xTaskGetTickCount() - currentTick > pdMSTO_TICKS(8000))
			{
				motor_sm = kSM_App_BalancedOnly_ON1;
				state_started = false;
				currentTick = xTaskGetTickCount(); /* Get the tick before jumping cases */
			}
			break;

		case kSM_App_BalancedOnly_ON1:

			if (!state_started) {
				BalancedMotor_ON();
				state_started = true;

			}

			if(xTaskGetTickCount() - currentTick > pdMSTO_TICKS(8000))
			{
				motor_sm = kSM_App_BalancedOnly_ON_LowPWM;
				state_started = false;  /* restarted to False for next state */
				currentTick = xTaskGetTickCount(); /* Get the tick before jumping cases */
			}
			break;

		case kSM_App_BalancedOnly_ON_LowPWM:

			if (!state_started) {
				/* Entry only one time */
				// Balanced motor is running in this step and continue to run
				BOARD_PWMpin_as_PWM1_X1();
				PWM_pulse_start();
				PWM_pulse_ms(14); // 14ms Pulse Width
				state_started = true;
			}

			if(xTaskGetTickCount() - currentTick > pdMSTO_TICKS(3000))
			{
				motor_sm = kSM_App_BalancedOnly_ON2;
				state_started = false;  /* restarted to False for next state */
				currentTick = xTaskGetTickCount(); /* Get the tick before jumping cases */
			}
			break;

		case kSM_App_BalancedOnly_ON2:
			if (!state_started) {
				/* Entry only one time */
				// Balanced motor is running in this step and continue to run
				PWM_pulse_stop();
				state_started = true;
			}

			if(xTaskGetTickCount() - currentTick > pdMSTO_TICKS(5000))
			{
				motor_sm = kSM_App_BalancedOnly_ON_MediumPWM;
				state_started = false;  /* restarted to False for next state */
				currentTick = xTaskGetTickCount(); /* Get the tick before jumping cases */
			}
			break;

		case kSM_App_BalancedOnly_ON_MediumPWM:

			if (!state_started) {
				/* Entry only one time */
				// Balanced motor is running in this step and continue to run
				BOARD_PWMpin_as_PWM1_X1();
				PWM_pulse_start();
				PWM_pulse_ms(17); // 17ms Pulse Width
				state_started = true;

			}

			if(xTaskGetTickCount() - currentTick > pdMSTO_TICKS(3000))
			{
				motor_sm = kSM_App_BalancedOnly_ON3;
				state_started = false;  /* restarted to False for next state */
				currentTick = xTaskGetTickCount(); /* Get the tick before jumping cases */
			}
			break;

		case kSM_App_BalancedOnly_ON3:
			if (!state_started) {
				/* Entry only one time */
				// Balanced motor is running in this step and continue to run
				PWM_pulse_stop();
				state_started = true;

			}

			if(xTaskGetTickCount() - currentTick > pdMSTO_TICKS(5000))
			{
				motor_sm = kSM_App_BalancedOnly_ON_HighPWM;
				state_started = false;  /* restarted to False for next state */
				currentTick = xTaskGetTickCount(); /* Get the tick before jumping cases */
			}
			break;

		case kSM_App_BalancedOnly_ON_HighPWM:
			if (!state_started) {
				/* Entry only one time */
				// Balanced motor is running in this step and continue to run
				BOARD_PWMpin_as_PWM1_X1();
				PWM_pulse_start();
				PWM_pulse_ms(19.5); // 19.5ms Pulse Width
				state_started = true;

			}

			if(xTaskGetTickCount() - currentTick > pdMSTO_TICKS(3000))
			{
				motor_sm = kSM_App_BalancedOnly_ON4;
				state_started = false;  /* restarted to False for next state */
				currentTick = xTaskGetTickCount(); /* Get the tick before jumping cases */
			}
			break;

		case kSM_App_BalancedOnly_ON4:
			if (!state_started) {
				/* Entry only one time */
				// Balanced motor is running in this step and continue to run
				PWM_pulse_stop();
				state_started = true;

			}

			if(xTaskGetTickCount() - currentTick > pdMSTO_TICKS(3000))
			{
				motor_sm = kSM_App_Repeat;
				state_started = false;  /* restarted to False for next state */
				currentTick = xTaskGetTickCount(); /* Get the tick before jumping cases */
			}
			break;

		case kSM_App_Repeat:
			if (!state_started) {
				state_started = true;
			}

			/* quick transition 200ms */
			if(xTaskGetTickCount() - currentTick > pdMSTO_TICKS(200))
			{
				motor_sm = kSM_App_Idle;
				state_started = false;  /* restarted to False for next state */
				PWM_pulse_stop();
			}
			break;

		default:
			/* If a non-defined case happens, reset the SM */
			motor_sm = kSM_App_Idle;

			break;
	}
}

void Stop_DemoSeq(void)
{
	if (!stop_flow_control)
	{
		/* Stop motors */
		MIKROE_CS_OFF(); /* Stop Balanced motor */

		/* Disable PWM Pulse. */
		PWM_pulse_stop();
		BOARD_PWMpin_as_GPIO();
		UnBalancedMotor_OFF();  //PWM signal goes to Low

		/* Allow for 1 second */
		currentTick = xTaskGetTickCount(); /* Get the tick before jumping to cases */

		stop_flow_control = true;
	}

	if(xTaskGetTickCount() - currentTick > pdMSTO_TICKS(1000))
	{
		/* restart all controllers, variables */
		motor_sm = kSM_App_Stop;
		state_started = false;  /* restarted to False for next state */
		demo_mode = demo_stop; /* The demo must go into idle state after stop.
										  In idle state the PWM Pulse and Relay are DISABLED */

		stop_flow_control = false;
	}
}
