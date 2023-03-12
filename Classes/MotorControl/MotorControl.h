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
    enum class MotorSwitchPhase{
        HighALowB,
        HighALowC,
        HighBLowC,
        HighBLowA,
        HighCLowA,
        HighCLowB,
    };

    class MotorControl {
    private:
    protected:
        NeuroFuzzy::SystemListener* systemListener = nullptr;
        LLA::MotorLowSide* motorLowSideInstance = nullptr;
        LLA::Adc* adcInstance = nullptr;


        int pwmLowSideValue = 0;
        //if set to one all outputs are blocked in reset state (to change state motor will need to be first unlocked)
        uint8_t locked = 1;

        uint8_t phaseValuesReady = false;
        uint32_t phaseValue[MOTOR_CONTROL_ADC_AVERAGE_SAMPLES_COUNT] = {0};
        uint32_t phaseZeroValue[MOTOR_CONTROL_ADC_AVERAGE_SAMPLES_COUNT] = {0};
        uint8_t sampleCount = 0;
        //0 for clockwise rotation, 1 for counterclockwise rotation
        uint8_t direction = 0;
        MotorSwitchPhase currentActivePhase = MotorSwitchPhase::HighALowB;
        uint32_t phaseMeanValue = 0;
        uint32_t zeroMeanValue = 0;
        uint32_t previousPhaseMeanValue = 0;
        int32_t crossingDirection = 0;

        //local variables to be placed inside class structure not on stack
        uint32_t tempAdcValue;

        void turnOffAll();
        void checkRotationState();
        void updateAdcValues();
        void setCurrentPhase(MotorSwitchPhase newPhase);
        void setPhase();
        MotorSwitchPhase getNextPhase();
    public:
        MotorControl();
        void setMotorLowSideInstance(LLA::MotorLowSide* newInstance);
        void setSystemListener(NeuroFuzzy::SystemListener* newSystemListener);
        void setAdcInstance(LLA::Adc *newInstance);
        void setDirection(int newDirection);
        void setPwmValue(int newPwmValue);

        void lockMotor();
        void unlockMotor();
        uint8_t getLockState();

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