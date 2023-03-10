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

extern NeuroFuzzy::System neuroFuzzySystem;

typedef StaticTask_t osStaticThreadDef_t;

osThreadId_t systemBreathing RTOS_MEMORY_ALLOCATION;
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

osThreadId_t motorControlTaskHandle RTOS_MEMORY_ALLOCATION;
uint32_t motorControlTaskBuffer[MOTOR_CONTROL_TASK_BUFFER_SIZE] RTOS_MEMORY_ALLOCATION;
osStaticThreadDef_t motorControlTaskControlBlock RTOS_MEMORY_ALLOCATION;
const osThreadAttr_t motorControlTask_attributes = {
        .name = MOTOR_CONTROL_TASK_NAME,
        .cb_mem = &motorControlTaskControlBlock,
        .cb_size = sizeof(motorControlTaskControlBlock),
        .stack_mem = &motorControlTaskBuffer[0],
        .stack_size = sizeof(motorControlTaskBuffer),
        .priority = (osPriority_t) MOTOR_CONTROL_TASK_PRIORITY,
};

volatile unsigned long SystemTicksCounter;
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    switch(GPIO_Pin){
        case GPIO_PIN_13:
            neuroFuzzySystem.gpioInterruptRoutine(1);
            break;
        case GPIO_PIN_14:
            neuroFuzzySystem.gpioInterruptRoutine(2);
            break;
        default:
            neuroFuzzySystem.gpioInterruptRoutine(0);
            break;
    }
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

void RTOS_MotorControlBEMFMonitoringTask(void *arguments) {
    neuroFuzzySystem.motorControlTask.task();
}

void MX_FREERTOS_Init(void) {
    systemBreathing = osThreadNew(RTOS_DefaultTask, NULL, &defaultTask_attributes);
    monitoringTaskHande = osThreadNew(RTOS_MonitoringTask, NULL, &monitoringTask_attributes);
    uartRxTaskHandle = osThreadNew(RTOS_UartRxTask, NULL, &uartRxTask_attributes);
    uartTxTaskHandle = osThreadNew(RTOS_UartTxTask, NULL, &uartTxTask_attributes);
    loadTaskHandle = osThreadNew(RTOS_LoadTask, NULL, &loadTask_attributes);
    motorControlTaskHandle = osThreadNew(RTOS_MotorControlBEMFMonitoringTask, NULL, &motorControlTask_attributes);
}
