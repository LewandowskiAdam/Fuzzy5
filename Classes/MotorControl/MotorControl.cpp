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
#include "SEGGER_RTT.h"

namespace MotorControl {
    void MotorControl::shutdown() {
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseA);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseB);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseC);
        phaseValuesReady = 0;
        sampleCount = 0;
        motorLowSideInstance->shutdown();
    }

    MotorSwitchPhase MotorControl::getNextPhase() {
        if (direction == Direction::Clockwise) {
            switch (currentActivePhase) {
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
        } else {
            switch (currentActivePhase) {
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

    void MotorControl::checkSlopeDirection(Slope slopeDirection) {
        MotorSwitchPhase nextPhase = currentActivePhase;
        SEGGER_RTT_printf(0, "Slope direction: %s\n", slopeDirection == Slope::Rising ? "rising" : "falling");
        if (direction == Direction::Clockwise) {
            switch (currentActivePhase) {
                case MotorSwitchPhase::HighALowB:
                    if (slopeDirection == Slope::Rising) nextPhase = getNextPhase();
                    break;
                case MotorSwitchPhase::HighALowC:
                    if (slopeDirection == Slope::Falling) nextPhase = getNextPhase();
                    break;
                case MotorSwitchPhase::HighBLowC:
                    if (slopeDirection == Slope::Rising) nextPhase = getNextPhase();
                    break;
                case MotorSwitchPhase::HighBLowA:
                    if (slopeDirection == Slope::Falling) nextPhase = getNextPhase();
                    break;
                case MotorSwitchPhase::HighCLowA:
                    if (slopeDirection == Slope::Rising) nextPhase = getNextPhase();
                    break;
                case MotorSwitchPhase::HighCLowB:
                    if (slopeDirection == Slope::Falling) nextPhase = getNextPhase();
                    break;
                default:
                    return;
            }
        } else {
            switch (currentActivePhase) {
                case MotorSwitchPhase::HighALowB:
                    if (slopeDirection == Slope::Falling) nextPhase = getNextPhase();
                    break;
                case MotorSwitchPhase::HighALowC:
                    if (slopeDirection == Slope::Rising) nextPhase = getNextPhase();
                    break;
                case MotorSwitchPhase::HighBLowC:
                    if (slopeDirection == Slope::Falling) nextPhase = getNextPhase();
                    break;
                case MotorSwitchPhase::HighBLowA:
                    if (slopeDirection == Slope::Rising) nextPhase = getNextPhase();
                    break;
                case MotorSwitchPhase::HighCLowA:
                    if (slopeDirection == Slope::Falling) nextPhase = getNextPhase();
                    break;
                case MotorSwitchPhase::HighCLowB:
                    if (slopeDirection == Slope::Rising) nextPhase = getNextPhase();
                    break;
                default:
                    return;
            }
        }
        if (nextPhase != currentActivePhase) {
            SEGGER_RTT_printf(0, "Active phase: %d switching to: %d\n", currentActivePhase, nextPhase);
            currentActivePhase = nextPhase;
            updatePhaseOutput();
        }
    }

    void MotorControl::updatePhaseOutput() {
        shutdown();
        switch (currentActivePhase) {
            case MotorSwitchPhase::HighALowB:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseA);
                motorLowSideInstance->setValue(LLA::Phase::PhaseB, pwmValue);
                break;
            case MotorSwitchPhase::HighALowC:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseA);
                motorLowSideInstance->setValue(LLA::Phase::PhaseC, pwmValue);
                break;
            case MotorSwitchPhase::HighBLowC:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseB);
                motorLowSideInstance->setValue(LLA::Phase::PhaseC, pwmValue);
                break;
            case MotorSwitchPhase::HighBLowA:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseB);
                motorLowSideInstance->setValue(LLA::Phase::PhaseA, pwmValue);
                break;
            case MotorSwitchPhase::HighCLowA:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseC);
                motorLowSideInstance->setValue(LLA::Phase::PhaseA, pwmValue);
                break;
            case MotorSwitchPhase::HighCLowB:
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseC);
                motorLowSideInstance->setValue(LLA::Phase::PhaseB, pwmValue);
                break;
        }
    }

    void MotorControl::updateAdcValues() {
        uint32_t tempAdcValue = 0;
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
        zeroValue[sampleCount] = adcInstance->getValue(LLA::Phase::Zero);
        sampleCount++;
        if (sampleCount >= MOTOR_CONTROL_ADC_AVERAGE_SAMPLES_COUNT) {
            phaseValuesReady = true;
            sampleCount = 0;
        }

        //calculating mean value
        if (phaseValuesReady == true) {
            phaseMeanValue = 0;
            zeroMeanValue = 0;
            for (int i = 0; i < MOTOR_CONTROL_ADC_AVERAGE_SAMPLES_COUNT; i++) {
                phaseMeanValue += phaseValue[i];
                zeroMeanValue += zeroValue[i];
            }
            phaseMeanValue = (phaseMeanValue >> MOTOR_CONTROL_ADC_AVERAGE_DIVIDE_VALUE);
            zeroMeanValue = (zeroMeanValue >> MOTOR_CONTROL_ADC_AVERAGE_DIVIDE_VALUE);
        }
    }

    void MotorControl::checkRotationState() {
        //casting relevant values
        int phaseV = static_cast<int>(phaseMeanValue);
        int zeroV = static_cast<int>(zeroMeanValue);
        int currentPhaseZeroDiff = phaseV - zeroV;

        int currPhaseZeroDiffSign = currentPhaseZeroDiff < 0 ? -1 : 1;
        Slope slopeDirection = Slope::NoSlope;

        if ((previousPhaseZeroDiffSign > 0) && (currPhaseZeroDiffSign < 0)) {
            //falling
            slopeDirection = Slope::Falling;
        } else if ((previousPhaseZeroDiffSign < 0) && (currPhaseZeroDiffSign > 0)) {
            //rising
            slopeDirection = Slope::Rising;
        }
        previousPhaseZeroDiffSign = currPhaseZeroDiffSign;
        if (slopeDirection != Slope::NoSlope) {
            SEGGER_RTT_printf(0, "PhaseV: %d, ZeroV: %d\n", phaseV, zeroV);
            checkSlopeDirection(slopeDirection);
        }
    }

/*----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *--------------------------------------------------------------------------------------------------------------------*/

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

    Lock MotorControl::getLockState() {
        return motorLocked;
    }

    void MotorControl::lockMotor() {
        shutdown();
        motorLocked = Lock::Locked;
        currentActivePhase = MotorSwitchPhase::HighALowB;
    }

    void MotorControl::unlockMotor() {
        motorLocked = Lock::Unlocked;
    }

    void MotorControl::setPwmValue(int newPwmValue) {
        pwmValue = newPwmValue;
    }

    int MotorControl::getPwmValue() {
        return pwmValue;
    }

    void MotorControl::setDirection(Direction newDirection) {
        direction = newDirection;
    }

    Direction MotorControl::getDirection() {
        return direction;
    }

/*----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *--------------------------------------------------------------------------------------------------------------------*/
    void MotorControl::adcISR() {
        if (motorLocked == Lock::Unlocked) {
            updateAdcValues();
            checkRotationState();
        }
    }
/*----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *--------------------------------------------------------------------------------------------------------------------*/
}