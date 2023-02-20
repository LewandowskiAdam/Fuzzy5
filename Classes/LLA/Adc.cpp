/*
    Adc.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "Adc.h"
#include "adc.h"
#include "Phase.h"

namespace LLA {
    Adc::Adc() {
        isInitialized = 0;
        for (int i = 0; i < channelCount; i++) {
            adcValues[i] = 0;
        }
    }

    uint16_t Adc::getValue(LLA::Phase phase) {
        switch (phase) {
            case Phase::PhaseA:
                return adcValues[0];
            case Phase::PhaseB:
                return adcValues[2];
            case Phase::PhaseC:
                return adcValues[1];
            case Phase::Zero:
                return adcValues[3];
            default:
                return 0xffff;
        }
    }

    void Adc::init() {
        if (isInitialized == 0) {
            HAL_ADC_Start_DMA(&hadc3, (uint32_t *) adcValues, channelCount);
            isInitialized = 1;
        }
    }

    Adc *Adc::getInstance() {
        static Adc adcInstance MEM_FORCE_D3RAM;
        return &adcInstance;
    }
}