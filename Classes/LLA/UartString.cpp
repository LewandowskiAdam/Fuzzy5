/*
    UartString.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "UartString.h"
#include "StringLiterals.h"

namespace LLA {
    // UartString class definitions
    UartString::UartString() {

    }

    UartString::UartString(std::string &stringToConvert) {
        this->convertFromString(stringToConvert);
    }

    std::string UartString::convertToString() {
        if (length == 0) return StringLiterals::EmptyMessage;
        else if (length > UART_BUFFER_SIZE) return StringLiterals::TooLongMessage;
        else {
            std::string outputString = "";
            for (int i = 0; i < length; i++) {
                outputString += string[i];
            }
            return outputString;
        }
    }

    void UartString::convertFromString(std::string &stringToConvert) {
        std::string message;
        std::string *stringPtr = &message;
        if (stringToConvert.size() > UART_BUFFER_SIZE) {
            message = StringLiterals::TooLongMessage;
        } else if (stringToConvert.size() == 0) {
            message = StringLiterals::EmptyMessage;
        } else {
            stringPtr = &stringToConvert;
        }

        for (int i = 0; i < stringPtr->size(); i++) {
            string[i] = stringPtr->c_str()[i];
        }
        length = stringPtr->size();
    }
}