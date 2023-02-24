/*
    MotorLowSide.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_MOTORLOWSIDE_H
#define NEUROFUZZYS22_MOTORLOWSIDE_H

#include "Phase.h"
#include <cstdint>

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

        uint8_t isInitialized = 0;
        uint8_t phaseAValue = 0;
        uint8_t phaseBValue = 0;
        uint8_t phaseCValue = 0;

        MotorLowSide() {};

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