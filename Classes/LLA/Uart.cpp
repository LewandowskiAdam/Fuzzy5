/*
    Uart.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "Uart.h"
#include "Configuration.h"
#include "MemorySpecialAllocators.h"
#include "usart.h"
#include "cstring"

//Cube functions overrides
//global RX callback (interrupt)
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART1) {
        LLA::Uart::getInstance()->rxEventCallback(Size);
    }
}

//global TX callback (interrupt)
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        LLA::Uart::getInstance()->txEventCallback();
    }
}

#include "SEGGER_RTT.h"
#include "UartString.h"

namespace LLA {
    void UartRtosInterface::setTxSemaphore(SemaphoreHandle_t newTxSemaphore) {
        if (newTxSemaphore != nullptr) {
            txSemaphore = newTxSemaphore;
        }
    }

    void UartRtosInterface::getTxSemaphore() {
        if (txSemaphore == nullptr) return;
        if (!possesTxSemaphore) {
            if (xSemaphoreTake(txSemaphore, portMAX_DELAY) == pdTRUE) {
                possesTxSemaphore = true;
            }
        }
    }

    void UartRtosInterface::giveTxSemaphore() {
        if (txSemaphore == nullptr) return;
        if (possesTxSemaphore) {
            xSemaphoreGiveFromISR(txSemaphore, nullptr);
            possesTxSemaphore = false;
        }
    }

    //UART class definitions
    Uart::Uart() {
        for (int i = 0; i < UART_BUFFER_SIZE; i++) {
            uartRxBuffer[0][i] = 0;
            uartRxBuffer[1][i] = 0;
            uartTxBuffer[i] = 0;
        }
        bytesToSend = 0;
        uartRxDmaBuffer = uartRxBuffer[1];
        isInitialized = 0;
        uartListener = nullptr;
    }

    void Uart::init() {
        if (isInitialized == 0) {
            isInitialized = 1;
            startReceiving();
        }
    }

    Uart *Uart::getInstance() {
        static Uart uartInstance MEM_FORCE_D3RAM;
        return &uartInstance;
    }


    void Uart::clearBuffer(uint8_t *buffer) {
        for (int i = 0; i < UART_BUFFER_SIZE; i++) {
            buffer[i] = 0;
        }
    }

    void Uart::swapReceiverBuffers() {
        if (uartRxDmaBuffer == uartRxBuffer[1]) {
            clearBuffer(uartRxBuffer[0]);
            uartRxDmaBuffer = uartRxBuffer[0];
        } else {
            clearBuffer(uartRxBuffer[1]);
            uartRxDmaBuffer = uartRxBuffer[1];
        }
    }

    void Uart::startReceiving() {
        swapReceiverBuffers();
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uartRxDmaBuffer, UART_BUFFER_SIZE);
    }

    void Uart::sendBuffer() {
        HAL_UART_Transmit_DMA(&huart1, uartTxBuffer, bytesToSend);
        getTxSemaphore();
    }

    void Uart::sendMessage(std::string stringToSend) {
        UartString lowLvlString(stringToSend);
        sendMessage(lowLvlString);
    }

    void Uart::sendMessage(UartString stringToSend) {
        memcpy(uartTxBuffer, &stringToSend.string, stringToSend.length);
        bytesToSend = stringToSend.length;
        sendBuffer();
    }

    void Uart::setListener(UartListener *newListener) {
        if (newListener != nullptr) {
            uartListener = newListener;
        }
    }

    void Uart::rxEventCallback(uint16_t Size) {
        uint8_t *tempBuffer = uartRxDmaBuffer;
        startReceiving();
        UartString string;
        string.length = Size;
        memcpy(&string.string, tempBuffer, Size);
        uartListener->dataReceivedFromISR(string);
    }

    void Uart::txEventCallback() {
        giveTxSemaphore();
    }
}