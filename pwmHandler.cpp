#include "pwmHandler.h"

pwmHandler::pwmHandler() {
  this -> pin = 5;
  this -> init();
}

pwmHandler::pwmHandler(int pin) {
  if (pin >=5 && pin <=7) {
    this -> pin = pin;
  } else {
    this -> pin = 5;
  }

  this -> init();
}

int pwmHandler::getPinNumber() {
  return this -> pin;
}

void pwmHandler::setPinNumber(int pin) {
  this -> pin = pin;
}

void pwmHandler::fast_pwm_analogWrite(uint8_t val) {
  switch (this -> pin) {
    case 5:
      TCC0->CC[5].reg = val;
      break;

    case 6:
      TCC1->CC[0].reg = val;
      break;

    case 7:
      TCC1->CC[1].reg = val;
      break;
  }
}

void pwmHandler::init() {
  if (this -> pin == 7) {
    // Set up the generic clock (GCLK7) to clock timer TCC0 
    GCLK->GENCTRL[7].reg = GCLK_GENCTRL_DIV(1) |       // Divide the 48MHz clock source by divisor 1: 48MHz/1 = 48MHz
                          GCLK_GENCTRL_IDC |          // Set the duty cycle to 50/50 HIGH/LOW
                          GCLK_GENCTRL_GENEN |        // Enable GCLK7
                          GCLK_GENCTRL_SRC_DFLL;      // Select 48MHz DFLL clock source

    while (GCLK->SYNCBUSY.bit.GENCTRL7);               // Wait for synchronization  

    GCLK->PCHCTRL[25].reg = GCLK_PCHCTRL_CHEN |        // Enable the TCC1 peripheral channel
                            GCLK_PCHCTRL_GEN_GCLK7;    // Connect generic clock 7 to TCC1

    // Enable the peripheral multiplexer on pin D7
    PORT->Group[g_APinDescription[7].ulPort].PINCFG[g_APinDescription[7].ulPin].bit.PMUXEN = 1;
    
    // Set the D7 (PORT_PB12) peripheral multiplexer to peripheral (even port number) E(6): TCC0, Channel 0
    PORT->Group[g_APinDescription[7].ulPort].PMUX[g_APinDescription[7].ulPin >> 1].reg |= PORT_PMUX_PMUXO(5);
    
    TCC1->CTRLA.reg = TC_CTRLA_PRESCALER_DIV4 |        // Set prescaler to 4, 48MHz/4 = 12MHz
                      TC_CTRLA_PRESCSYNC_PRESC;        // Set the reset/reload to trigger on prescaler clock                 

    TCC1->WAVE.reg = TC_WAVE_WAVEGEN_NPWM;             // Set-up TCC1 timer for Normal (single slope) PWM mode (NPWM)
    while (TCC1->SYNCBUSY.bit.WAVE)                    // Wait for synchronization
                         
    TCC1->PER.reg = 255;                               // Set-up the PER (period) register 47kHz PWM
    while (TCC1->SYNCBUSY.bit.PER);                    // Wait for synchronization
    
    TCC1->CC[1].reg = 127;                             // Set-up the CC (counter compare), channel 0 register for 50% duty-cycle
    while (TCC1->SYNCBUSY.bit.CC1);                    // Wait for synchronization

    TCC1->CTRLA.bit.ENABLE = 1;                        // Enable timer TCC0
    while (TCC1->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization

  } else if (pin == 6) {
    // Set up the generic clock (GCLK7) to clock timer TCC0 
    GCLK->GENCTRL[7].reg = GCLK_GENCTRL_DIV(1) |       // Divide the 48MHz clock source by divisor 1: 48MHz/1 = 48MHz
                          GCLK_GENCTRL_IDC |          // Set the duty cycle to 50/50 HIGH/LOW
                          GCLK_GENCTRL_GENEN |        // Enable GCLK7
                          GCLK_GENCTRL_SRC_DFLL;      // Select 48MHz DFLL clock source

    while (GCLK->SYNCBUSY.bit.GENCTRL7);               // Wait for synchronization  

    GCLK->PCHCTRL[25].reg = GCLK_PCHCTRL_CHEN |        // Enable the TCC0 peripheral channel
                            GCLK_PCHCTRL_GEN_GCLK7;    // Connect generic clock 7 to TCC0

    // Enable the peripheral multiplexer on pin D6
    PORT->Group[g_APinDescription[6].ulPort].PINCFG[g_APinDescription[6].ulPin].bit.PMUXEN = 1;
    
    PORT->Group[g_APinDescription[6].ulPort].PMUX[g_APinDescription[6].ulPin >> 1].reg |= PORT_PMUX_PMUXE(5);
    
    TCC1->CTRLA.reg = TC_CTRLA_PRESCALER_DIV4 |        // Set prescaler to 4, 48MHz/4 = 12MHz
                      TC_CTRLA_PRESCSYNC_PRESC;        // Set the reset/reload to trigger on prescaler clock                 

    TCC1->WAVE.reg = TC_WAVE_WAVEGEN_NPWM;             // Set-up TCC1 timer for Normal (single slope) PWM mode (NPWM)
    while (TCC1->SYNCBUSY.bit.WAVE)                    // Wait for synchronization

    TCC1->PER.reg = 255;                               // Set-up the PER (period) register 47kHz PWM
    while (TCC1->SYNCBUSY.bit.PER);                    // Wait for synchronization
    
    TCC1->CC[0].reg = 127;                             // Set-up the CC (counter compare), channel 0 register for 50% duty-cycle
    while (TCC1->SYNCBUSY.bit.CC0);                    // Wait for synchronization

    TCC1->CTRLA.bit.ENABLE = 1;                        // Enable timer TCC1
    while (TCC1->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization

  } else if (pin == 5) {
    // Set up the generic clock (GCLK7) to clock timer TCC0 
    GCLK->GENCTRL[7].reg = GCLK_GENCTRL_DIV(1) |       // Divide the 48MHz clock source by divisor 1: 48MHz/1 = 48MHz
                          GCLK_GENCTRL_IDC |          // Set the duty cycle to 50/50 HIGH/LOW
                          GCLK_GENCTRL_GENEN |        // Enable GCLK7
                          GCLK_GENCTRL_SRC_DFLL;      // Select 48MHz DFLL clock source

    while (GCLK->SYNCBUSY.bit.GENCTRL7);               // Wait for synchronization  

    GCLK->PCHCTRL[25].reg = GCLK_PCHCTRL_CHEN |        // Enable the TCC0 peripheral channel
                            GCLK_PCHCTRL_GEN_GCLK7;    // Connect generic clock 7 to TCC0

    // Enable the peripheral multiplexer on pin D5
    PORT->Group[g_APinDescription[5].ulPort].PINCFG[g_APinDescription[5].ulPin].bit.PMUXEN = 1;
    
    PORT->Group[g_APinDescription[5].ulPort].PMUX[g_APinDescription[5].ulPin >> 1].reg |= PORT_PMUX_PMUXO(5);
    
    TCC0->CTRLA.reg = TC_CTRLA_PRESCALER_DIV4 |        // Set prescaler to 4, 48MHz/4 = 12MHz
                      TC_CTRLA_PRESCSYNC_PRESC;        // Set the reset/reload to trigger on prescaler clock                 

    TCC0->WAVE.reg = TC_WAVE_WAVEGEN_NPWM;             // Set-up TCC0 timer for Normal (single slope) PWM mode (NPWM)
    while (TCC0->SYNCBUSY.bit.WAVE)                    // Wait for synchronization
                           
    TCC0->PER.reg = 255;                               // Set-up the PER (period) register 47kHz PWM
    while (TCC0->SYNCBUSY.bit.PER);                    // Wait for synchronization
                        
    TCC0->CC[5].reg = 127;                             // Set-up the CC (counter compare), channel 5 register for 50% duty-cycle
    while (TCC0->SYNCBUSY.bit.CC5);                    // Wait for synchronization

    TCC0->CTRLA.bit.ENABLE = 1;                        // Enable timer TCC0
    while (TCC0->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization
  }
}