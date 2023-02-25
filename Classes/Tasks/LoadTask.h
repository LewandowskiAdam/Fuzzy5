/*
    LoadTask.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_LOADTASK_H
#define NEUROFUZZYS22_LOADTASK_H

#include "FreeRTOS.h"
#include "queue.h"
#include "Configuration.h"
#include "UartString.h"
#include "Load.h"
#include "SystemListener.h"

namespace Tasks {
    class LoadTask {
    private:
    protected:
        QueueHandle_t loadQueue;
        StaticQueue_t loadQueueControlBlock;
        uint8_t loadQueueStorageArea[LOAD_QUEUE_DEPTH * sizeof(LLA::UartString)];
        LLA::Load* loadInstance = nullptr;
        NeuroFuzzy::SystemListener* systemListener = nullptr;
        int pwmValue;

        bool isNumber(const std::string& str);
        void setPwmValue(int newValue);
    public:
        LoadTask();
        void task();
        void addToQueue(LLA::UartString newMessage);
        void setSystemListener(NeuroFuzzy::SystemListener* newSystemListener);
    };
}
#endif