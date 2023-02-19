/*
    TasksRedirections.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "TasksRedirections.h"
#include "System.h"
#include "Configuration.h"
#include "tim.h"

#include "Led.h"

extern NeuroFuzzy::System neuroFuzzySystem;

typedef StaticTask_t osStaticThreadDef_t;

osThreadId_t defaultTaskHande RTOS_MEMORY_ALLOCATION;
uint32_t defaultTaskBuffer[DEFAULT_TASK_BUFFER_SIZE] RTOS_MEMORY_ALLOCATION;
osStaticThreadDef_t defaultTaskControlBlock RTOS_MEMORY_ALLOCATION;
const osThreadAttr_t defaultTask_attributes = {
        .name = DEFAULT_TASK_NAME,
        .cb_mem = &defaultTaskControlBlock,
        .cb_size = sizeof(defaultTaskControlBlock),
        .stack_mem = &defaultTaskBuffer[0],
        .stack_size = sizeof(defaultTaskBuffer),
        .priority = (osPriority_t) DEFAULT_TASK_PRIORITY,
};

osThreadId_t monitoringTaskHande RTOS_MEMORY_ALLOCATION;
uint32_t monitoringTaskBuffer[MONITORING_TASK_BUFFER_SIZE] RTOS_MEMORY_ALLOCATION;
osStaticThreadDef_t monitoringTaskControlBlock RTOS_MEMORY_ALLOCATION;
const osThreadAttr_t monitoringTask_attributes = {
        .name = MONITORING_TASK_NAME,
        .cb_mem = &monitoringTaskControlBlock,
        .cb_size = sizeof(monitoringTaskControlBlock),
        .stack_mem = &monitoringTaskBuffer[0],
        .stack_size = sizeof(monitoringTaskBuffer),
        .priority = (osPriority_t) MONITORING_TASK_PRIORITY,
};

osThreadId_t uartRxTaskHandle RTOS_MEMORY_ALLOCATION;
uint32_t uartRxTaskBuffer[UART_RX_TASK_BUFFER_SIZE] RTOS_MEMORY_ALLOCATION;
osStaticThreadDef_t uartRxTaskControlBlock RTOS_MEMORY_ALLOCATION;
const osThreadAttr_t uartRxTask_attributes = {
        .name = UART_RX_TASK_NAME,
        .cb_mem = &uartRxTaskControlBlock,
        .cb_size = sizeof(uartRxTaskControlBlock),
        .stack_mem = &uartRxTaskBuffer[0],
        .stack_size = sizeof(uartRxTaskBuffer),
        .priority = (osPriority_t) UART_RX_PRIORITY,
};

osThreadId_t uartTxTaskHandle RTOS_MEMORY_ALLOCATION;
uint32_t uartTxTaskBuffer[UART_TX_TASK_BUFFER_SIZE] RTOS_MEMORY_ALLOCATION;
osStaticThreadDef_t uartTxTaskControlBlock RTOS_MEMORY_ALLOCATION;
const osThreadAttr_t uartTxTask_attributes = {
        .name = UART_TX_TASK_NAME,
        .cb_mem = &uartTxTaskControlBlock,
        .cb_size = sizeof(uartTxTaskControlBlock),
        .stack_mem = &uartTxTaskBuffer[0],
        .stack_size = sizeof(uartTxTaskBuffer),
        .priority = (osPriority_t) UART_TX_PRIORITY,
};

osThreadId_t loadTaskHandle RTOS_MEMORY_ALLOCATION;
uint32_t loadTaskBuffer[LOAD_TASK_BUFFER_SIZE] RTOS_MEMORY_ALLOCATION;
osStaticThreadDef_t loadTaskControlBlock RTOS_MEMORY_ALLOCATION;
const osThreadAttr_t loadTask_attributes = {
        .name = LOAD_TASK_NAME,
        .cb_mem = &loadTaskControlBlock,
        .cb_size = sizeof(loadTaskControlBlock),
        .stack_mem = &loadTaskBuffer[0],
        .stack_size = sizeof(loadTaskBuffer),
        .priority = (osPriority_t) LOAD_TASK_PRIORITY,
};


unsigned long SystemTicksCounter;
extern TIM_HandleTypeDef htim16;

void configureTimerForRunTimeStats(void) {
    SystemTicksCounter = 0;
    HAL_TIM_Base_Start_IT(&htim16);
}

unsigned long getRunTimeCounterValue(void) {
    /*!
    * To be modified to return directly value of timer counter register
    * this is rtos overrided statistics function
    */
    return SystemTicksCounter;
}

void RTOS_DefaultTask(void *arguments) {
    neuroFuzzySystem.defaultTask.task();
}

void RTOS_MonitoringTask(void *arguments) {
    neuroFuzzySystem.monitorTask.task();
}

void RTOS_UartRxTask(void *arguments) {
    neuroFuzzySystem.uartTasks.rxTask();
}

void RTOS_UartTxTask(void *arguments) {
    neuroFuzzySystem.uartTasks.txTask();
}

void RTOS_LoadTask(void *arguments) {
    neuroFuzzySystem.loadTask.task();
}

void MX_FREERTOS_Init(void) {
    defaultTaskHande = osThreadNew(RTOS_DefaultTask, NULL, &defaultTask_attributes);
    monitoringTaskHande = osThreadNew(RTOS_MonitoringTask, NULL, &monitoringTask_attributes);
    uartRxTaskHandle = osThreadNew(RTOS_UartRxTask, NULL, &uartRxTask_attributes);
    uartTxTaskHandle = osThreadNew(RTOS_UartTxTask, NULL, &uartTxTask_attributes);
    loadTaskHandle = osThreadNew(RTOS_LoadTask, NULL, &loadTask_attributes);
}
