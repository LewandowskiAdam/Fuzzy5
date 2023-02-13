/*
    SystemListener.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_SYSTEMLISTENER_H
#define NEUROFUZZYS22_SYSTEMLISTENER_H

namespace NeuroFuzzy {
    class SystemListener {
    public:
        virtual void loadControlListener(std::string message) = 0;
        virtual void uartSend(std::string message)=0;
    };
}
#endif