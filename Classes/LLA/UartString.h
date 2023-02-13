/*
    UartString.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_UARTSTRING_H
#define NEUROFUZZYS22_UARTSTRING_H

#include <cstdint>
#include <string>
#include "Configuration.h"

namespace LLA {
    class UartString {
    private:
    protected:
    public:
        //only public members
        uint16_t length = 0;
        uint8_t string[UART_BUFFER_SIZE] = {""};

        UartString();

        UartString(std::string &stringToConvert);

        std::string convertToString();

        void convertFromString(std::string &stringToConvert);

    };
}
#endif