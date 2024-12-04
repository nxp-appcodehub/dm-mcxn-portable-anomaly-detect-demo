/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "stdint.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* State machine control command flags */
#define SM_CTRL_NONE 0x0U
#define SM_CTRL_FAULT 0x1U
#define SM_CTRL_FAULT_CLEAR 0x2U
#define SM_CTRL_INIT_DONE 0x4U
#define SM_CTRL_STOP 0x8U
#define SM_CTRL_START 0x10U
#define SM_CTRL_STOP_ACK 0x20U
#define SM_CTRL_RUN_ACK 0x40U

typedef uint16_t sm_app_ctrl;
typedef uint32_t sm_app_fault;

/*! @brief Pointer to function */
typedef void (*pfcn_void_void)(void);

/*! @brief Application state identification enum */
typedef enum _sm_app_state
{
    kSM_App_Idle = 0,
    kSM_App_Demo_IdleTime  = 1,
	kSM_App_BalancedOnly_ON1  = 2,
	kSM_App_BalancedOnly_ON_LowPWM  = 3,
	kSM_App_BalancedOnly_ON2  = 4,
	kSM_App_BalancedOnly_ON_MediumPWM  = 5,
	kSM_App_BalancedOnly_ON3  = 6,
	kSM_App_BalancedOnly_ON_HighPWM  = 7,
	kSM_App_BalancedOnly_ON4  = 8,
	kSM_App_Repeat  = 9,
	kSM_App_Stop  = 10
} sm_app_state_t;


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif //_ST_MACHINE_H_
