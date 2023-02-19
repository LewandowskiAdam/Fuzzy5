/*
    Configuration.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_CONFIGURATION_H
#define NEUROFUZZYS22_CONFIGURATION_H

#include "MemorySpecialAllocators.h"

//Some task definitions/configurations

//sector allocator for rtos objects (most of them) into fast ram memory (Instruction Tightly Coupled Memory - ITCM, one cycle latency)
#define RTOS_MEMORY_ALLOCATION MEM_FORCE_ITCMRAM
//sector allocator for call type objects (functions/methods) into fast ram memory (Instruction Tightly Coupled Memory - ITCM, one cycle latency)
#define QUICK_ACCESS_CALL_ALLOCATION MEM_FORCE_ITCMRAM
//sector allocator for data type objects (functions/methods) into fast ram memory (Data Tightly Coupled Memory - DTCM, one cycle latency)
#define QUICK_ACCESS_DATA_ALLOCATION MEM_FORCE_DTCMRAM

//----------------Default task---------------------//
#define DEFAULT_TASK_LED_INTERVAL_MS 500
#define DEFAULT_TASK_BUFFER_SIZE 256
#define DEFAULT_TASK_NAME "System breathing"
#define DEFAULT_TASK_PRIORITY osPriorityBelowNormal7

//---------------Monitoring Task-------------------//
#define MONITORING_TASK_INTERVAL_MS 1000
#define MONITORING_TASK_LED_ON_TIME_MS 100
#define MONITORING_TASK_NAME "Monitoring"
#define MONITORING_TASK_PRIORITY osPriorityBelowNormal6
#define MONITORING_TASK_BUFFER_SIZE 256
#define MONITORING_TASK_BUFFER_DATA_SIZE 1024
#define MONITORING_USE_UART 1
#define MONITORING_USE_RTT 1

//-----------------UART tasks---------------------//
#if MONITORING_USE_UART
#define UART_BUFFER_SIZE MONITORING_TASK_BUFFER_DATA_SIZE
#else
#define UART_BUFFER_SIZE 256
#endif
//RX task

#define RX_QUEUE_DEPTH 10
#define UART_RX_TASK_BUFFER_SIZE 1024
#define UART_RX_TASK_NAME "Uart RX"
#define UART_RX_PRIORITY osPriorityNormal7

//TX task
#define TX_QUEUE_DEPTH 10
#define UART_TX_TASK_BUFFER_SIZE 256
#define UART_TX_TASK_NAME "Uart TX"
#define UART_TX_PRIORITY osPriorityNormal6

//------------------Load task---------------------//
#define LOAD_QUEUE_DEPTH 2
#define LOAD_TASK_BUFFER_SIZE 256
#define LOAD_TASK_NAME "Load"
#define LOAD_TASK_PRIORITY osPriorityNormal5

#endif