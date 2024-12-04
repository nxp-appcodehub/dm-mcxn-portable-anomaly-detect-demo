/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include <stdint.h>
#include <stdbool.h>

#include "pin_mux.h"

#include "fsl_port.h"
#include "fsl_device_registers.h"

#include "clock_config.h"
#include "board.h"

#include "FreeRTOS.h"
#include "task.h"

typedef enum _demo_mode {
  demo_idle = 0,
  demo_stop,
  demo_train,
  demo_run
} demo_mode_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void motor_control_init();
void motor_control_task();
void flexPWM_init(void);
void ESC_Motor_Init(void);
void PWMX_DRV_Init(void);
void PWM_pulse_ms(float duty_cycle_ms);
void PWM_pulse_start(void);
void PWM_pulse_stop(void);
void BalancedMotor_ON(void);
void BalancedMotor_OFF(void);

void UnBalancedMotor_ON(void);
void UnBalancedMotor_OFF(void);

void All_Motors_OFF(void);

//void delay_ms(uint16_t time_ms);

void get_demo_mode(demo_mode_t *mode);
void set_demo_mode(demo_mode_t mode);

void SysTick_Handler(void);
void SysTick_DelayTicks(uint32_t n);
void Run_DemoSeq(void);
void Stop_DemoSeq(void);



#endif /* MOTOR_CONTROL_H_ */
