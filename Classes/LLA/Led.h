/*
    Led.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_LED_H
#define NEUROFUZZYS22_LED_H

namespace LLA {
    class LedRed0 {
    };

    class LedBlue {
    };

    class LedRed1 {
    };

    class Led {
    private:
    protected:
        Led() {};

        Led(const Led &) {};
    public:
        static void toggleLed(LLA::LedRed0);

        static void toggleLed(LLA::LedBlue);

        static void toggleLed(LLA::LedRed1);

        static void on(LLA::LedRed0);

        static void on(LLA::LedBlue);

        static void on(LLA::LedRed1);

        static void off(LLA::LedRed0);

        static void off(LLA::LedBlue);

        static void off(LLA::LedRed1);
    };
}

#endif