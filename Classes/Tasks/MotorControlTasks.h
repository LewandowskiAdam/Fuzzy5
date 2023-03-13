/*
    MotorControlTasks.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_MOTORCONTROLTASKS_H
#define NEUROFUZZYS22_MOTORCONTROLTASKS_H

#include "FreeRTOS.h"
#include "queue.h"
#include "Configuration.h"
#include "SystemListener.h"
#include "Adc.h"
#include "MotorControl.h"

namespace Tasks {
    class MotorControlTasks {
    private:
    protected:
        QueueHandle_t motorControlQueue;
        StaticQueue_t motorControlQueueControlBlock;
        uint8_t motorControlQueueStorageArea[MOTOR_CONTROL_QUEUE_DEPTH];

        LLA::Adc *adcInstance = nullptr;
        MotorControl::MotorControl motorControl;
        LLA::MotorLowSide *motorLowSideInstance = nullptr;
        NeuroFuzzy::SystemListener *systemListener = nullptr;

        int count = 0;

        void printADCValues();
        void printADCValuesCSV();

    public:
        MotorControlTasks();

        void task();

        void setSystemListenerInstance(NeuroFuzzy::SystemListener *newSystemListener);

        void setMotorLowSide(LLA::MotorLowSide *newInstance);

        void setAdcInstance(LLA::Adc *newInstance);

        //ISR's
        void buttonISR(int buttonPressed);

        void adcISR();
    };
}
#endif