/*
    MotorControl.cpp
    Created on: 24.02.23
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "MotorControl.h"
#include "MotorHighSide.h"
#include <cassert>

namespace MotorControl {
    MotorControl::MotorControl() {
    }

    void MotorControl::setMotorLowSideInstance(LLA::MotorLowSide *newInstance) {
        assert(newInstance != nullptr);
        motorLowSideInstance = newInstance;
    }

    void MotorControl::setSystemListener(NeuroFuzzy::SystemListener *newSystemListener) {
        assert(newSystemListener != nullptr);
        systemListener = newSystemListener;
    }

    void MotorControl::setAdcInstance(LLA::Adc *newInstance) {
        assert(newInstance != nullptr);
        adcInstance = newInstance;
    }


    void MotorControl::lockMotor() {
        locked = true;
        turnOffAll();
    }

    void MotorControl::unlockMotor() {
        locked = false;
    }

    uint8_t MotorControl::getLockState() {
        return locked;
    }

    void MotorControl::setCurrentPhase(MotorSwitchPhase newPhase) {
        currentActivePhase = newPhase;
    }

    MotorSwitchPhase MotorControl::getNextPhase() {
        if(direction==0){
            switch (currentActivePhase){
                case MotorSwitchPhase::HighALowB:
                    return MotorSwitchPhase::HighALowC;
                case MotorSwitchPhase::HighALowC:
                    return MotorSwitchPhase::HighBLowC;
                case MotorSwitchPhase::HighBLowC:
                    return MotorSwitchPhase::HighBLowA;
                case MotorSwitchPhase::HighBLowA:
                    return MotorSwitchPhase::HighCLowA;
                case MotorSwitchPhase::HighCLowA:
                    return MotorSwitchPhase::HighCLowB;
                case MotorSwitchPhase::HighCLowB:
                    return MotorSwitchPhase::HighALowB;
                default:
                    return MotorSwitchPhase::HighALowB;
            }
        }
        else{
            switch (currentActivePhase){
                case MotorSwitchPhase::HighALowB:
                    return MotorSwitchPhase::HighCLowB;
                case MotorSwitchPhase::HighALowC:
                    return MotorSwitchPhase::HighALowB;
                case MotorSwitchPhase::HighBLowC:
                    return MotorSwitchPhase::HighALowC;
                case MotorSwitchPhase::HighBLowA:
                    return MotorSwitchPhase::HighBLowC;
                case MotorSwitchPhase::HighCLowA:
                    return MotorSwitchPhase::HighBLowA;
                case MotorSwitchPhase::HighCLowB:
                    return MotorSwitchPhase::HighCLowA;
                default:
                    return MotorSwitchPhase::HighALowB;
            }
        }
    }

    void MotorControl::turnOffAll() {
        assert(motorLowSideInstance != nullptr);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseA);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseB);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseC);
        phaseValuesReady = 0;
        sampleCount = 0;
        motorLowSideInstance->shutdown();
    }

    void MotorControl::setPhase() {
        turnOffAll();
        switch (currentActivePhase) {
            case MotorSwitchPhase::HighALowB:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseA);
                motorLowSideInstance->setValue(LLA::Phase::PhaseB, pwmLowSideValue);
                break;
            case MotorSwitchPhase::HighALowC:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseA);
                motorLowSideInstance->setValue(LLA::Phase::PhaseC, pwmLowSideValue);
                break;
            case MotorSwitchPhase::HighBLowC:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseB);
                motorLowSideInstance->setValue(LLA::Phase::PhaseC, pwmLowSideValue);
                break;
            case MotorSwitchPhase::HighBLowA:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseB);
                motorLowSideInstance->setValue(LLA::Phase::PhaseA, pwmLowSideValue);
                break;
            case MotorSwitchPhase::HighCLowA:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseC);
                motorLowSideInstance->setValue(LLA::Phase::PhaseA, pwmLowSideValue);
                break;
            case MotorSwitchPhase::HighCLowB:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseC);
                motorLowSideInstance->setValue(LLA::Phase::PhaseB, pwmLowSideValue);
                break;
        }
    }

    void MotorControl::setPwmValue(int newPwmValue) {
        pwmLowSideValue = newPwmValue;
    }

    void MotorControl::setDirection(int newDirection) {
        if (newDirection == 0) direction = 0;
        else direction = 1;
    }

    void MotorControl::checkRotationState() {
        if (phaseValuesReady != 0) {
            //calculating mean values
            phaseMeanValue = 0;
            zeroMeanValue = 0;
            for (int i = 0; i < MOTOR_CONTROL_ADC_AVERAGE_SAMPLES_COUNT; i++) {
                phaseMeanValue += phaseValue[i];
                zeroMeanValue += phaseZeroValue[i];
            }
            phaseMeanValue = (phaseMeanValue >> MOTOR_CONTROL_ADC_AVERAGE_DIVIDE_VALUE);
            zeroMeanValue = (zeroMeanValue >> MOTOR_CONTROL_ADC_AVERAGE_DIVIDE_VALUE);

            //detecting zero crossing and direction of crossing
            if (previousPhaseMeanValue < phaseMeanValue) {
                crossingDirection = 1;
            } else crossingDirection = -1;
            previousPhaseMeanValue = phaseMeanValue;

            //checking if zero was crossed
            if (crossingDirection > 0) {

            }

        }

    }

    void MotorControl::updateAdcValues() {
        switch (currentActivePhase) {
            case MotorSwitchPhase::HighALowB:
                tempAdcValue = adcInstance->getValue(LLA::Phase::PhaseC);
                break;
            case MotorSwitchPhase::HighALowC:
                tempAdcValue = adcInstance->getValue(LLA::Phase::PhaseB);
                break;
            case MotorSwitchPhase::HighBLowC:
                tempAdcValue = adcInstance->getValue(LLA::Phase::PhaseA);
                break;
            case MotorSwitchPhase::HighBLowA:
                tempAdcValue = adcInstance->getValue(LLA::Phase::PhaseC);
                break;
            case MotorSwitchPhase::HighCLowA:
                tempAdcValue = adcInstance->getValue(LLA::Phase::PhaseB);
                break;
            case MotorSwitchPhase::HighCLowB:
                tempAdcValue = adcInstance->getValue(LLA::Phase::PhaseA);
                break;
            default:
                break;
        }
        phaseValue[sampleCount] = tempAdcValue;
        phaseZeroValue[sampleCount] = adcInstance->getValue(LLA::Phase::Zero);
        sampleCount++;
        if (sampleCount >= MOTOR_CONTROL_ADC_AVERAGE_SAMPLES_COUNT) {
            phaseValuesReady = 1;
            sampleCount = 0;
        }
        if (previousPhaseMeanValue == 0) {
            previousPhaseMeanValue = phaseValue[0];
        }


    }

    void MotorControl::adcISR() {
        if (locked == 0) {
            updateAdcValues();
            checkRotationState();
        }
    }
}