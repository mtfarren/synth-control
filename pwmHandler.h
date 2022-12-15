// This class increases the PWM frequency on three digital pins
// to roughly 47kHz. It works with pins D5, D6, and D7 on the
// Adafruit Metro M4 Grand Central (SAMD51).

// Note: true measured pwm frequency is ~46.8kHz

#include <Arduino.h>

#ifndef PWMHANDLER_H
#define PWMHANDLER_H

class pwmHandler {
  public:
    pwmHandler();
    pwmHandler(int pin);
    int getPinNumber();
    void setPinNumber(int pin);
    void fast_pwm_analogWrite(uint8_t val);

  private:
    int pin;
    void init();
};

#endif