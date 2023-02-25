/*
    UartTasks.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "UartTasks.h"
#include "cmsis_os2.h"
#include "StringLiterals.h"
#include "UartString.h"
#include <cassert>

namespace Tasks {
    UartTasks::UartTasks() {
        txFreertosQueue = xQueueCreateStatic(TX_QUEUE_DEPTH, sizeof(LLA::UartString), txQueueStorageArea,
                                             &txQueueControlBlock);
        txSemaphore = xSemaphoreCreateBinaryStatic(&txSemaphoreBuffer);
        rxFreertosQueue = xQueueCreateStatic(RX_QUEUE_DEPTH, sizeof(LLA::UartString), rxQueueStorageArea,
                                             &rxQueueControlBlock);
        //registering queues for statistics purposes (not necessary for them to work properly)
        vQueueAddToRegistry(txFreertosQueue, StringLiterals::UartTxQueueName);
        vQueueAddToRegistry(rxFreertosQueue, StringLiterals::UartRxQueueName);
    }

    void UartTasks::setUartClassInstance(LLA::Uart *newUartClassInstance) {
        assert(newUartClassInstance != nullptr);
        uartInstance = newUartClassInstance;
        uartInstance->setTxSemaphore(txSemaphore);
    }

    void UartTasks::setSystemControlListener(NeuroFuzzy::SystemListener *newListener) {
        assert(newListener != nullptr);
        systemControlListener = newListener;
    }

    void UartTasks::addToTxQueue(std::string newMessage) {
        LLA::UartString string;
        string.convertFromString(newMessage);
        xQueueSendToBack(txFreertosQueue, (void *) &string, 0);
    }

    void UartTasks::addToTxQueueFromISR(std::string newMessage) {
        LLA::UartString string;
        string.convertFromString(newMessage);
        xQueueSendToBackFromISR(txFreertosQueue, (void *) &string, 0);
    }

    void UartTasks::addToRxQueueFromISR(LLA::UartString newMessage) {
        xQueueSendToBackFromISR(rxFreertosQueue, (void *) &newMessage, 0);
    }

    void UartTasks::txTask() {
        while (1) {
            xQueueReceive(txFreertosQueue, &txBuffer, portMAX_DELAY);
            xSemaphoreGive(txSemaphore);
            uartInstance->sendMessage(txBuffer);
            xSemaphoreTake(txSemaphore, portMAX_DELAY);
        }
    }

    void UartTasks::rxTask() {
        while (1) {
            xQueueReceive(rxFreertosQueue, &rxBuffer, portMAX_DELAY);
            std::string message;
            message = rxBuffer.convertToString().c_str();
            if ((message.find(StringLiterals::Load, 0)) != std::string::npos) {
                systemControlListener->loadControlListener(message);
            }
        }
    }
}