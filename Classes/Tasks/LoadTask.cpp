/*
    LoadTask.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "LoadTask.h"
#include "Configuration.h"
#include "cmsis_os.h"
#include "StringLiterals.h"
#include "Led.h"
#include "Load.h"

using namespace std;
namespace Tasks {
    LoadTask::LoadTask() {
        loadFreertosQueue = xQueueCreateStatic(LOAD_QUEUE_DEPTH, sizeof(LLA::UartString), loadQueueStorageArea,
                                               &loadQueueControlBlock);
        vQueueAddToRegistry(loadFreertosQueue, StringLiterals::LoadQueueName);
        loadInstance = LLA::Load::getInstance();
    }

    void LoadTask::addToQueue(LLA::UartString newMessage) {
        xQueueSendToBackFromISR(loadFreertosQueue, (void *) &newMessage, 0);
    }

    bool LoadTask::isNumber(const string &str) {
        for (char const &c: str) {
            if (std::isdigit(c) == 0) return false;
        }
        return true;
    }

    void LoadTask::setPwmValue(int newValue) {
        if (loadInstance != nullptr) {
            loadInstance->setValue(LLA::Phase::PhaseA, newValue);
            loadInstance->setValue(LLA::Phase::PhaseB, newValue);
            loadInstance->setValue(LLA::Phase::PhaseC, newValue);
        }
    }

    void LoadTask::setSystemListener(NeuroFuzzy::SystemListener *newSystemListener) {
        if (newSystemListener != nullptr) {
            systemListener = newSystemListener;
        }
    }

    void LoadTask::task() {
        static LLA::UartString receivedString;
        while (1) {
            xQueueReceive(loadFreertosQueue, &receivedString, portMAX_DELAY);
            string::size_type controlTypeStringStart;
            string::size_type numberEnd;
            string message;
            message = receivedString.convertToString();
            message = message.substr(message.find(StringLiterals::Load, 0) + sizeof(StringLiterals::Load) - 1);
            if (message.find(StringLiterals::Set, 0) != string::npos) {
                //cut string
                message = message.substr(message.find(StringLiterals::Set, 0) + sizeof(StringLiterals::Set) - 1);
                //check if separator is present
                if ((controlTypeStringStart = message.find(StringLiterals::Separator, 0)) != string::npos) {
                    //again cut string
                    message = message.substr(controlTypeStringStart + sizeof(StringLiterals::Separator) - 1);
                    numberEnd = message.find(StringLiterals::Separator, 0);
                    if (numberEnd == string::npos) {
                        numberEnd = message.size() - 1;
                    }
                    message = message.substr(0, numberEnd);
                    if (isNumber(message)) {
                        int setValue = std::stoi(message);
                        if (setValue > 100) {
                            setValue = 100;
                        }
                        if (setValue < 0) {
                            setValue = 0;
                        }
                        setPwmValue(setValue);
                        pwmValue = setValue;
                        string toSend(StringLiterals::LoadSetMessage);
                        toSend += message;
                        toSend += "\n";
                        systemListener->uartSend(toSend);
                    } else {
                        string toSend(StringLiterals::LoadSetNotProperlyFormatted);
                        toSend += "\n";
                        systemListener->uartSend(toSend);
                    }
                } else {
                    string toSend(StringLiterals::LoadSetNotProperlyFormatted);
                    toSend += "\n";
                    systemListener->uartSend(toSend);
                }
            } else if (message.find(StringLiterals::Get, 0) != string::npos) {
                string toSend(StringLiterals::LoadGetMessage);
                toSend += " " + to_string(pwmValue) + "\n";
                systemListener->uartSend(toSend);
            } else {
                string toSend(StringLiterals::LoadSetNotProperlyFormatted);
                toSend += "\n";
                systemListener->uartSend(toSend);
            }
        }
    }
}