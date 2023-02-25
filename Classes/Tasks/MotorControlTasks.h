/*
    MotorControlTasks.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_MOTORCONTROLTASKS_H
#define NEUROFUZZYS22_MOTORCONTROLTASKS_H

#include "SystemListener.h"
#include "Adc.h"
#include "MotorControl.h"

namespace Tasks {
    class MotorControlTasks {
    private:
    protected:
        LLA::Adc *adcInstance = nullptr;
        MotorControl::MotorControl motorControl;
        LLA::MotorLowSide *motorLowSideInstance = nullptr;
        NeuroFuzzy::SystemListener *systemListener = nullptr;

        bool motorLocked = true;

        void printAdcValues();

        void printAdcValuesCSV();

    public:
        MotorControlTasks();

        void task();

        void setSystemListenerInstance(NeuroFuzzy::SystemListener *newSystemListener);

        void setMotorLowSide(LLA::MotorLowSide *newInstance);

        //ISR
        void buttonISR(int buttonPressed);
    };
}
#endif