/*
    UartTasks.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_UARTTASKS_H
#define NEUROFUZZYS22_UARTTASKS_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "Configuration.h"
#include "Uart.h"
#include "SystemListener.h"
#include "UartString.h"

namespace NeuroFuzzy {
    class System;
}

namespace Tasks {
    class UartTasks {
    private:
    protected:
        NeuroFuzzy::SystemListener *systemControlListener = nullptr;

        QueueHandle_t rxFreertosQueue;
        QueueHandle_t txFreertosQueue;

        StaticQueue_t rxQueueControlBlock;
        StaticQueue_t txQueueControlBlock;

        uint8_t rxQueueStorageArea[RX_QUEUE_DEPTH * sizeof(LLA::UartString)];
        uint8_t txQueueStorageArea[TX_QUEUE_DEPTH * sizeof(LLA::UartString)];

        SemaphoreHandle_t txSemaphore = NULL;
        StaticSemaphore_t txSemaphoreBuffer;

        LLA::UartString txBuffer;
        LLA::UartString rxBuffer;

        LLA::Uart *uartInstance;

    public:
        UartTasks();

        void rxTask();

        void txTask();

        void setUartClassInstance(LLA::Uart *newUartClassInstance);

        void setSystemControlListener(NeuroFuzzy::SystemListener *newListener);

        void addToTxQueue(std::string newMessage);

        void addToRxQueueFromISR(LLA::UartString newMessage);
    };

}

#endif