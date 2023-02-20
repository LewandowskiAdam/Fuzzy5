/*
    Adc.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_ADC_H
#define NEUROFUZZYS22_ADC_H

#include <cstdint>
#include "MemorySpecialAllocators.h"
#include "Phase.h"

namespace LLA {
    class Adc {
    protected:
        static constexpr uint8_t channelCount = 4;
        uint16_t adcValues[channelCount];
        uint8_t isInitialized;

        Adc();

        Adc(const Adc &) {};
    public:
        uint16_t getValue(LLA::Phase phase);

        void init();

        static Adc *getInstance();
    };
}

#endif