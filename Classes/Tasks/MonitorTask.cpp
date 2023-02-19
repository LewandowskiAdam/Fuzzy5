/*
    MonitorTask.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "MonitorTask.h"
#include "Configuration.h"
#include "cmsis_os.h"
#include "Led.h"

#include "SEGGER_RTT.h"
#include "StringLiterals.h"
#include "cstring"

namespace Tasks {
    void MonitorTask::setSystemListener(NeuroFuzzy::SystemListener *newSystemListener) {
        if(newSystemListener!= nullptr){
            systemListener=newSystemListener;
        }
    }
    void MonitorTask::task() {
        static char data[MONITORING_TASK_BUFFER_DATA_SIZE] RTOS_MEMORY_ALLOCATION;;
        while (1) {
            osDelay(MONITORING_TASK_INTERVAL_MS-MONITORING_TASK_LED_ON_TIME_MS);
            LLA::Led::on(LLA::LedBlue());
            osDelay(MONITORING_TASK_LED_ON_TIME_MS);
            LLA::Led::off(LLA::LedBlue());
            vTaskGetRunTimeStats(data);
#if MONITORING_USE_RTT
            SEGGER_RTT_printf(0, StringLiterals::MonitoringStartSeparator);
            SEGGER_RTT_printf(0, data);
            SEGGER_RTT_printf(0, StringLiterals::MonitoringSeparator);
#endif
#if MONITORING_USE_UART
            systemListener->uartSend(std::string(StringLiterals::MonitoringStartSeparator));
            systemListener->uartSend(std::string(data));
            systemListener->uartSend(std::string(StringLiterals::MonitoringSeparator));
#endif
        }
    }
}