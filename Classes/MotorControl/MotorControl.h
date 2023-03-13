/*
    MotorControl.h
    Created on: 24.02.23
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_MOTORCONTROL_H
#define NEUROFUZZYS22_MOTORCONTROL_H

#include "MotorLowSide.h"
#include "SystemListener.h"
#include "Adc.h"
#include "Configuration.h"

namespace MotorControl {
    enum class MotorSwitchPhase {
        HighALowB,
        HighALowC,
        HighBLowC,
        HighBLowA,
        HighCLowA,
        HighCLowB,
    };

    enum class Direction{
        Clockwise,
        CounterClockwise,
    };

    enum class Lock{
        Unlocked,
        Locked,
    };

    enum class Slope{
        NoSlope,
        Rising,
        Falling,
    };

    class MotorControl {
    private:
    protected:
        NeuroFuzzy::SystemListener *systemListener = nullptr;
        LLA::MotorLowSide *motorLowSideInstance = nullptr;
        LLA::Adc *adcInstance = nullptr;

        //control fields
        Lock motorLocked = Lock::Locked;
        int pwmValue = 0;
        Direction direction = Direction::Clockwise;
        MotorSwitchPhase currentActivePhase = MotorSwitchPhase::HighALowB;

        //adc fields
        //tables for calculating mean values
        uint32_t phaseValue[MOTOR_CONTROL_ADC_AVERAGE_SAMPLES_COUNT] = {0};
        uint32_t zeroValue[MOTOR_CONTROL_ADC_AVERAGE_SAMPLES_COUNT] = {0};
        //other adc related variables
        bool phaseValuesReady = false;
        uint8_t sampleCount = 0;
        uint32_t phaseMeanValue = 0;
        uint32_t zeroMeanValue = 0;
        int previousPhaseZeroDiffSign = 0;


        void shutdown();
        MotorSwitchPhase getNextPhase();
        void updatePhaseOutput();
        void updateAdcValues();
        void checkRotationState();
        void checkSlopeDirection(Slope slopeDirection);
    public:
        MotorControl();
        void setMotorLowSideInstance(LLA::MotorLowSide* newInstance);
        void setSystemListener(NeuroFuzzy::SystemListener* newSystemListener);
        void setAdcInstance(LLA::Adc *newInstance);

        //control methods
        Lock getLockState();
        void lockMotor();
        void unlockMotor();
        void setPwmValue(int newPwmValue);
        int getPwmValue();
        void setDirection(Direction newDirection);
        Direction getDirection();

        void adcISR();
    };
}

/*
 * void set_next_step(){
  switch(sequence_step){
    case 0:
      AH_BL();
      BEMF_C_RISING();
      break;
    case 1:
      AH_CL();
      BEMF_B_FALLING();
      break;
    case 2:
      BH_CL();
      BEMF_A_RISING();
      break;
    case 3:
      BH_AL();
      BEMF_C_FALLING();
      break;
    case 4:
      CH_AL();
      BEMF_B_RISING();
      break;
    case 5:
      CH_BL();
      BEMF_A_FALLING();
      break;
  }
}//end of BLDC_move
 */

#endif