/*
    TasksRedirections.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_TASKSREDIRECTIONS_H
#define NEUROFUZZYS22_TASKSREDIRECTIONS_H

#include "FreeRTOS.h"
#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif

void MX_FREERTOS_Init(void);

//----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------Interrupts redirections------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

//----------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------Tasks---------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void RTOS_DefaultTask(void *arguments);
void RTOS_MonitoringTask(void *arguments);
void RTOS_UartRxTask(void *arguments);
void RTOS_UartTxTask(void *arguments);
void RTOS_LoadTask(void *arguments);
void RTOS_MotorControlBEMFMonitoringTask(void *arguments);

#ifdef __cplusplus
}
#endif

#endif