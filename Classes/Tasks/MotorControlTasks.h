/*
    MotorControlTasks.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_MOTORCONTROLTASKS_H
#define NEUROFUZZYS22_MOTORCONTROLTASKS_H

#include "Adc.h"

namespace Tasks {
    class MotorControlTasks {
    private:
    protected:
        LLA::Adc* adcInstance = nullptr;

        void printAdcValues();
        void printAdcValuesCSV();
    public:
        MotorControlTasks();
        void task();
    };
}
#endif