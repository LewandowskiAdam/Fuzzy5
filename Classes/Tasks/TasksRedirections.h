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

static int adcIterator;
#ifdef __cplusplus
extern "C" {
#endif

void MX_FREERTOS_Init(void);

//----------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------Tasks---------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void RTOS_DefaultTask(void *arguments);
void RTOS_MonitoringTask(void *arguments);
void RTOS_UartRxTask(void *arguments);
void RTOS_UartTxTask(void *arguments);
void RTOS_LoadTask(void *arguments);
void RTOS_MotorTask(void *arguments);

#ifdef __cplusplus
}
#endif

#endif