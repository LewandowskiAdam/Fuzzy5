/*
    System.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "System.h"
#include "MotorHighSide.h"
#include "StringLiterals.h"
#include "UartString.h"
#include "SEGGER_RTT.h"

Tasks::SystemBreathing NeuroFuzzy::System::defaultTask;
Tasks::UartTasks NeuroFuzzy::System::uartTasks;
Tasks::MonitorTask NeuroFuzzy::System::monitorTask;
Tasks::LoadTask NeuroFuzzy::System::loadTask;
Tasks::MotorControlTasks NeuroFuzzy::System::motorControlTask;

namespace NeuroFuzzy {
    System::System() {
    }

    void System::init() {
        uartSend(StringLiterals::InitializationStart);
        adc = LLA::Adc::getInstance();
        adc->init();
        load = LLA::Load::getInstance();
        load->init();
        load->setValue(LLA::Phase::PhaseA, 0);
        load->setValue(LLA::Phase::PhaseB, 0);
        load->setValue(LLA::Phase::PhaseC, 0);
        motorLowSide = LLA::MotorLowSide::getInstance();
        motorLowSide->init();
        uart = LLA::Uart::getInstance();
        uart->init();
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseA);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseB);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseC);
        uartTasks.setUartClassInstance(uart);
        uartTasks.setSystemControlListener(this);
        uart->setListener(this);
        uartSend(StringLiterals::InitializationComplete);
        loadTask.setSystemListener(this);
        monitorTask.setSystemListener(this);
        motorControlTask.setSystemListenerInstance(this);
        motorControlTask.setMotorLowSide(motorLowSide);
    }

    void System::gpioInterruptRoutine(uint16_t GpioPin) {
        int passingValue=0;
        switch (GpioPin) {
            case 1:
                passingValue=1;
                button1InterruptRoutine();
                break;
            case 2:
                passingValue=2;
                button2InterruptRoutine();
                break;
            default:
                buttonNotPresentInterruptRoutine();
                break;
        }
        motorControlTask.buttonISR(passingValue);
    }

    void System::buttonNotPresentInterruptRoutine() {
        SEGGER_RTT_printf(0, "WARNING! WARNING! WARNING!\n");
        SEGGER_RTT_printf(0, "EXTI interrupt from unknown source!\n");
    }

    void System::button1InterruptRoutine() {
        SEGGER_RTT_printf(0, "Button 1 pressed!\n");
    }

    void System::button2InterruptRoutine() {
        SEGGER_RTT_printf(0, "Button 2 pressed!\n");
    }

    //interfaces methods
    void System::dataReceivedFromISR(LLA::UartString uartString) {
        uartTasks.addToRxQueueFromISR(uartString);
    }

    void System::uartSend(std::string message) {
        uartTasks.addToTxQueue(message);
    }

    void System::loadControlListener(std::string message) {
        loadTask.addToQueue(LLA::UartString(message));
    }
}
