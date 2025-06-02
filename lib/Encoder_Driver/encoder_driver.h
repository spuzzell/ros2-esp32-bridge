#pragma once

#ifndef TEST_DESKTOP
#include <esp_attr.h>
#endif


void IRAM_ATTR leftEncoderISR();


void IRAM_ATTR rightEncoderISR();

/**
 * @class roverEncoders
 * @brief Provides static methods for initializing and reading quadrature encoders.
 */
class roverEncoders
{
    public:

    /**
     * @brief Initializes the encoder system (pins and interrupts).
     *        Safe to call multiple times; only initializes once.
     */
    static void init()
    {
        static bool initialized = 0;
        if (!initialized)
        {
            init2();
            initialized = true;
        }
    }

    static long readEncoder(int i);

    static long getCountsAndReset(int i);

    static void resetEncoder(int i);

    static void resetEncoders();

    static void transmit();

private:

    static void init2();
};