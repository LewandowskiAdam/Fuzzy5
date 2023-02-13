/*
    MotorLowSide.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_MOTORLOWSIDE_H
#define NEUROFUZZYS22_MOTORLOWSIDE_H

#include "Phase.h"

namespace LLA {
    class MotorLowSide {
    private:
    protected:

        inline void enableAPhase();

        inline void enableBPhase();

        inline void enableCPhase();

        inline void shutdownAPhase();

        inline void shutdownBPhase();

        inline void shutdownCPhase();

        uint8_t isInitialized;
        uint8_t phaseAValue;
        uint8_t phaseBValue;
        uint8_t phaseCValue;

        MotorLowSide();

        MotorLowSide(const MotorLowSide &) {};
    public:

        void init();

        void setValue(LLA::Phase phase, int pwmPercentage);

        int getValue(LLA::Phase phase);

        inline void shutdown();

        static MotorLowSide *getInstance();
    };

}
#endif