/*
    Uart.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_UART_H
#define NEUROFUZZYS22_UART_H

#include <string>
#include "Configuration.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "UartString.h"

namespace LLA {
    class UartListener {
    private:
    protected:
    public:
        virtual void dataReceivedFromISR(UartString uartString) = 0;
    };

    class UartRtosInterface {
    private:
    protected:
        SemaphoreHandle_t txSemaphore = nullptr;
        bool possesTxSemaphore = false;
    public:

        UartRtosInterface() {}

        void setTxSemaphore(SemaphoreHandle_t newTxSemaphore);

        void getTxSemaphore();

        void giveTxSemaphore();
    };

    class Uart : public UartRtosInterface {
    private:
    protected:
        uint8_t uartRxBuffer[2][UART_BUFFER_SIZE];
        uint8_t uartTxBuffer[UART_BUFFER_SIZE];
        uint8_t *uartRxDmaBuffer;
        uint16_t bytesToSend;
        LLA::UartListener *uartListener;
        uint8_t isInitialized;

        Uart();

        Uart(const Uart &) {}

        void clearBuffer(uint8_t *buffer);

        void swapReceiverBuffers();

        void sendBuffer();

    public:
        void init();

        void startReceiving();

        void sendMessage(std::string stringToSend);

        void sendMessage(UartString stringToSend);

        void setListener(UartListener *newListener);

        static Uart *getInstance();

        //interrupts
        void rxEventCallback(uint16_t Size);

        void txEventCallback();
    };
}
#endif