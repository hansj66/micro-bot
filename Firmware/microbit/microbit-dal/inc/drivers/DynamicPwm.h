/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "mbed.h"
#include "MicroBitConfig.h"

#ifndef MICROBIT_DYNAMIC_PWM_H
#define MICROBIT_DYNAMIC_PWM_H

#define NO_PWMS 3
#define MICROBIT_DEFAULT_PWM_PERIOD 20000

enum PwmPersistence
{
    PWM_PERSISTENCE_TRANSIENT = 1,
    PWM_PERSISTENCE_PERSISTENT = 2,
};

/**
  * Class definition for DynamicPwm.
  *
  * This class addresses a few issues found in the underlying libraries.
  * This provides the ability for a neat, clean swap between PWM channels.
  */
class DynamicPwm : public PwmOut
{
    private:
    static DynamicPwm* pwms[NO_PWMS];
    static uint8_t lastUsed;
    static uint16_t sharedPeriod;
    uint8_t flags;
    float lastValue;



    /**
      * An internal constructor used when allocating a new DynamicPwm instance.
      *
      * @param pin the name of the pin for the pwm to target
      *
      * @param persistance the level of persistence for this pin PWM_PERSISTENCE_PERSISTENT (can not be replaced until freed, should only be used for system services really.)
      *                    or PWM_PERSISTENCE_TRANSIENT (can be replaced at any point if a channel is required.)
      */
    DynamicPwm(PinName pin, PwmPersistence persistence = PWM_PERSISTENCE_TRANSIENT);

    public:

    /**
      * Redirects the pwm channel to point at a different pin.
      *
      * @param pin the desired pin to output a PWM wave.
      *
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      * pwm->redirect(p0); // pwm is now produced on p0
      * @endcode
      */
    void redirect(PinName pin);


    /**
      * Creates a new DynamicPwm instance, or reuses an existing instance that
      * has a persistence level of PWM_PERSISTENCE_TRANSIENT.
      *
      * @param pin the name of the pin for the pwm to target
      *
      * @param persistance the level of persistence for this pin PWM_PERSISTENCE_PERSISTENT (can not be replaced until freed, should only be used for system services really.)
      *                    or PWM_PERSISTENCE_TRANSIENT (can be replaced at any point if a channel is required.)
      *
      * @return a pointer to the first available free pwm channel - or the first one that can be reallocated. If
      *         no channels are available, NULL is returned.
      *
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      * @endcode
      */
    static DynamicPwm* allocate(PinName pin, PwmPersistence persistence = PWM_PERSISTENCE_TRANSIENT);

    /**
      * Frees this DynamicPwm instance for reuse.
      *
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate();
      * pwm->release();
      * @endcode
      */
    void release();

    /**
      * A lightweight wrapper around the super class' write in order to capture the value
      *
      * @param value the duty cycle percentage in floating point format.
      *
      * @return MICROBIT_OK on success, MICROBIT_INVALID_PARAMETER if value is out of range
      *
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate();
      * pwm->write(0.5);
      * @endcode
      */
    int write(float value);

    /**
      * Retreives the PinName associated with this DynamicPwm instance.
      *
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      *
      * // returns the PinName n.
      * pwm->getPinName();
      * @endcode
      *
      * @note This should be used to check that the DynamicPwm instance has not
      *       been reallocated for use in another part of a program.
      */
    PinName getPinName();

    /**
      * Retreives the last value that has been written to this DynamicPwm instance.
      * in the range 0 - 1023 inclusive.
      *
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      * pwm->write(0.5);
      *
      * // will return 512.
      * pwm->getValue();
      * @endcode
      */
    int getValue();

    /**
      * Retreives the current period in use by the entire PWM module in microseconds.
      *
      * Example:
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      * pwm->getPeriod();
      * @endcode
      */
    int getPeriodUs();

    /**
      * Retreives the current period in use by the entire PWM module in milliseconds.
      *
      * Example:
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      * pwm->setPeriodUs(20000);
      *
      * // will return 20000
      * pwm->getPeriod();
      * @endcode
      */
    int getPeriod();

    /**
      * Sets the period used by the WHOLE PWM module.
      *
      * @param period the desired period in microseconds.
      *
      * @return MICROBIT_OK on success, MICROBIT_INVALID_PARAMETER if period is out of range
      *
      * Example:
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      *
      * // period now is 20ms
      * pwm->setPeriodUs(20000);
      * @endcode
      *
      * @note Any changes to the period will AFFECT ALL CHANNELS.
      */
    int setPeriodUs(int period);

    /**
      * Sets the period used by the WHOLE PWM module. Any changes to the period will AFFECT ALL CHANNELS.
      *
      * @param period the desired period in milliseconds.
      *
      * @return MICROBIT_OK on success, MICROBIT_INVALID_PARAMETER if period is out of range
      *
      * Example:
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      *
      * // period now is 20ms
      * pwm->setPeriod(20);
      * @endcode
      */
      int setPeriod(int period);
};

#endif
