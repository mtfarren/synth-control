/*
  SAMD51_InterruptTimer.h
  Atmel SAMD51 interrupt service routine timer library
  For e.g. Adafruit M4 Metro/Feather/ItsyBitsy Express

  Method names mimic the 'ZeroTimer' library by Tamasa (@EHbtj) for easily
  switching compilation between SAMD21 and SAMD51 microprocessor boards.
  See https://github.com/EHbtj/ZeroTimer for the SAMD21 library.

  Adapted by MIDI Music Magic from Dennis van Gils
  November 2022  
*/

#include "Arduino.h"
#include "SAMD51_InterruptTimer.h"

// Adafruit M4 code (cores/arduino/startup.c) configures these clock generators:
// 120MHz - GCLK0
// 100MHz - GCLK2
// 48MHz  - GCLK1
// 12MHz  - GCLK4

#define GCLK1_HZ 48000000
#define GCLK0_HZ 120000000
#define TIMER_PRESCALER_DIV 1024

void (*func0)();
void (*func1)();
void (*func2)();
void (*func3)();
void (*func4)();
void (*func5)();

void TC_Timer::TC_wait_for_sync() {
  switch (TC_num) {
    case 0:
      while (TC0->COUNT16.SYNCBUSY.reg != 0) {}
      break;
    case 1:
      while (TC1->COUNT16.SYNCBUSY.reg != 0) {}
      break;
    case 2:
      while (TC2->COUNT16.SYNCBUSY.reg != 0) {}
      break;
    case 3:
      while (TC3->COUNT16.SYNCBUSY.reg != 0) {}
      break;
    case 4:
      while (TC4->COUNT16.SYNCBUSY.reg != 0) {}
      break;
    case 5:
      while (TC5->COUNT16.SYNCBUSY.reg != 0) {}
      break;
  }
}

TC_Timer::TC_Timer() {
  this -> TC_num = 3;
}

TC_Timer::TC_Timer(int TC_num) {
  this -> TC_num = TC_num;
}

void TC_Timer::setTCNumber(int n) {
  this -> TC_num = n;
  if (!(n >= 0 && n <= 5)) {
    Serial.println("**** WARNING: unsupported TC number (must be 0 - 5) ****");
    this -> TC_num = 3;
  }
}

int TC_Timer::getTCNumber() {
  return this -> TC_num;
}

void TC_Timer::startTimer(unsigned long period, void (*f)()) {
  // Enable the TC bus clock, use clock generator 1
  // GCLK->PCHCTRL[TC3_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val |
  //                                  (1 << GCLK_PCHCTRL_CHEN_Pos);
  switch (this -> TC_num) {
    case 0:
      GCLK->PCHCTRL[TC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                    (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC0->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC0->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC0->COUNT16.INTENSET.reg = 0;
      TC0->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC0_IRQn);

      func0 = f;

      break;
    
    case 1:
      GCLK->PCHCTRL[TC1_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                    (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC1->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC1->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC1->COUNT16.INTENSET.reg = 0;
      TC1->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC1_IRQn);

      func1 = f;

      break;

    case 2:
      GCLK->PCHCTRL[TC2_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                    (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC2->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC2->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC2->COUNT16.INTENSET.reg = 0;
      TC2->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC2_IRQn);

      func2 = f;

      break;
    
    case 3:
      GCLK->PCHCTRL[TC3_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                    (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC3->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC3->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC3->COUNT16.INTENSET.reg = 0;
      TC3->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC3_IRQn);

      func3 = f;

      break;
    
    case 4:
      GCLK->PCHCTRL[TC4_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                    (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC4->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC4->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC4->COUNT16.INTENSET.reg = 0;
      TC4->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC4_IRQn);

      func4 = f;

      break;

    case 5:
      GCLK->PCHCTRL[TC5_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                    (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC5->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC5->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC5->COUNT16.INTENSET.reg = 0;
      TC5->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC5_IRQn);

      func5 = f;

      break;
  }

  setPeriod(period);
}

void TC_Timer::stopTimer() {
  switch (TC_num) {
    case 0:
      TC0->COUNT16.CTRLA.bit.ENABLE = 0;
    case 1:
      TC1->COUNT16.CTRLA.bit.ENABLE = 0;
      break;
    case 2:
      TC2->COUNT16.CTRLA.bit.ENABLE = 0;
      break;
    case 3:
      TC3->COUNT16.CTRLA.bit.ENABLE = 0;
      break;
    case 4:
      TC4->COUNT16.CTRLA.bit.ENABLE = 0;
      break;
    case 5:
      TC5->COUNT16.CTRLA.bit.ENABLE = 0;
      break;
  }
}

void TC_Timer::restartTimer(unsigned long period) {
  // Enable the TC bus clock, use clock generator 1
  // GCLK->PCHCTRL[TC3_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val |
  //                                  (1 << GCLK_PCHCTRL_CHEN_Pos);
  switch (TC_num) {
    case 0:
      GCLK->PCHCTRL[TC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                      (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC0->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC0->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC0->COUNT16.INTENSET.reg = 0;
      TC0->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC0_IRQn);

      break;
    
    case 1:
      GCLK->PCHCTRL[TC1_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                      (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC1->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC1->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC1->COUNT16.INTENSET.reg = 0;
      TC1->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC1_IRQn);

      break;
      
    case 2:
      GCLK->PCHCTRL[TC2_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                      (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC2->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC2->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC2->COUNT16.INTENSET.reg = 0;
      TC2->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC2_IRQn);

      break;
    
    case 3:
      GCLK->PCHCTRL[TC3_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                      (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC3->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC3->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC3->COUNT16.INTENSET.reg = 0;
      TC3->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC3_IRQn);

      break;
    
    case 4:
      GCLK->PCHCTRL[TC4_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                      (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC4->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC4->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC4->COUNT16.INTENSET.reg = 0;
      TC4->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC4_IRQn);

      break;
    
    case 5:
      GCLK->PCHCTRL[TC5_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0_Val |
                                      (1 << GCLK_PCHCTRL_CHEN_Pos);
      while (GCLK->SYNCBUSY.reg > 0);

      TC5->COUNT16.CTRLA.bit.ENABLE = 0;
      
      // Use match mode so that the timer counter resets when the count matches the
      // compare register
      TC5->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
      TC_wait_for_sync();
      
      // Enable the compare interrupt
      TC5->COUNT16.INTENSET.reg = 0;
      TC5->COUNT16.INTENSET.bit.MC0 = 1;

      // Enable IRQ
      NVIC_EnableIRQ(TC5_IRQn);

      break;
  }  

  setPeriod(period);
}

void TC_Timer::setPeriod(unsigned long period) {
  int prescaler;
  uint32_t TC_CTRLA_PRESCALER_DIVN;

  switch (TC_num) {
    case 0:
      TC0->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
      TC_wait_for_sync();
      TC0->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1024;
      TC_wait_for_sync();
      TC0->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV256;
      TC_wait_for_sync();
      TC0->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV64;
      TC_wait_for_sync();
      TC0->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV16;
      TC_wait_for_sync();
      TC0->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV4;
      TC_wait_for_sync();
      TC0->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV2;
      TC_wait_for_sync();
      TC0->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1;
      TC_wait_for_sync();
      break;
    
    case 1:
      TC1->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
      TC_wait_for_sync();
      TC1->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1024;
      TC_wait_for_sync();
      TC1->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV256;
      TC_wait_for_sync();
      TC1->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV64;
      TC_wait_for_sync();
      TC1->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV16;
      TC_wait_for_sync();
      TC1->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV4;
      TC_wait_for_sync();
      TC1->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV2;
      TC_wait_for_sync();
      TC1->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1;
      TC_wait_for_sync();
      break;
    
    case 2:
      TC2->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
      TC_wait_for_sync();
      TC2->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1024;
      TC_wait_for_sync();
      TC2->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV256;
      TC_wait_for_sync();
      TC2->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV64;
      TC_wait_for_sync();
      TC2->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV16;
      TC_wait_for_sync();
      TC2->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV4;
      TC_wait_for_sync();
      TC2->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV2;
      TC_wait_for_sync();
      TC2->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1;
      TC_wait_for_sync();
      break;
    
    case 3:
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
      TC_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1024;
      TC_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV256;
      TC_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV64;
      TC_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV16;
      TC_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV4;
      TC_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV2;
      TC_wait_for_sync();
      TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1;
      TC_wait_for_sync();
      break;
    
    case 4:
      TC4->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
      TC_wait_for_sync();
      TC4->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1024;
      TC_wait_for_sync();
      TC4->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV256;
      TC_wait_for_sync();
      TC4->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV64;
      TC_wait_for_sync();
      TC4->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV16;
      TC_wait_for_sync();
      TC4->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV4;
      TC_wait_for_sync();
      TC4->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV2;
      TC_wait_for_sync();
      TC4->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1;
      TC_wait_for_sync();
      break;
    
    case 5:
      TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
      TC_wait_for_sync();
      TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1024;
      TC_wait_for_sync();
      TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV256;
      TC_wait_for_sync();
      TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV64;
      TC_wait_for_sync();
      TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV16;
      TC_wait_for_sync();
      TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV4;
      TC_wait_for_sync();
      TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV2;
      TC_wait_for_sync();
      TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1;
      TC_wait_for_sync();
      break;
  }      

  if (period > 300000) {
    TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV1024;
    prescaler = 1024;
  } else if (80000 < period && period <= 300000) {
    TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV256;
    prescaler = 256;
  } else if (20000 < period && period <= 80000) {
    TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV64;
    prescaler = 64;
  } else if (10000 < period && period <= 20000) {
    TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV16;
    prescaler = 16;
  } else if (5000 < period && period <= 10000) {
    TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV8;
    prescaler = 8;
  } else if (2500 < period && period <= 5000) {
    TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV4;
    prescaler = 4;
  } else if (1000 < period && period <= 2500) {
    TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV2;
    prescaler = 2;
  } else if (period <= 1000) {
    TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV1;
    prescaler = 1;
  }

  // int compareValue = (int)(GCLK1_HZ / (prescaler/((float)period / 1000000))) - 1;
  int compareValue = (int)(GCLK0_HZ / (prescaler/((float)period / 100000000))) - 1;

  switch (TC_num) {
    case 0:
      TC0->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIVN;
      TC_wait_for_sync();

      // Make sure the count is in a proportional position to where it was
      // to prevent any jitter or disconnect when changing the compare value.
      TC0->COUNT16.COUNT.reg = map(TC0->COUNT16.COUNT.reg, 0,
                                  TC0->COUNT16.CC[0].reg, 0, compareValue);
      TC0->COUNT16.CC[0].reg = compareValue;
      TC_wait_for_sync();

      TC0->COUNT16.CTRLA.bit.ENABLE = 1;
      TC_wait_for_sync();

      break;
    
    case 1:
      TC1->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIVN;
      TC_wait_for_sync();

      // Make sure the count is in a proportional position to where it was
      // to prevent any jitter or disconnect when changing the compare value.
      TC1->COUNT16.COUNT.reg = map(TC1->COUNT16.COUNT.reg, 0,
                                  TC1->COUNT16.CC[0].reg, 0, compareValue);
      TC1->COUNT16.CC[0].reg = compareValue;
      TC_wait_for_sync();

      TC1->COUNT16.CTRLA.bit.ENABLE = 1;
      TC_wait_for_sync();

      break;
    
    case 2:
      TC2->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIVN;
      TC_wait_for_sync();

      // Make sure the count is in a proportional position to where it was
      // to prevent any jitter or disconnect when changing the compare value.
      TC2->COUNT16.COUNT.reg = map(TC2->COUNT16.COUNT.reg, 0,
                                  TC2->COUNT16.CC[0].reg, 0, compareValue);
      TC2->COUNT16.CC[0].reg = compareValue;
      TC_wait_for_sync();

      TC2->COUNT16.CTRLA.bit.ENABLE = 1;
      TC_wait_for_sync();

      break;
    
    case 3:
      TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIVN;
      TC_wait_for_sync();

      // Make sure the count is in a proportional position to where it was
      // to prevent any jitter or disconnect when changing the compare value.
      TC3->COUNT16.COUNT.reg = map(TC3->COUNT16.COUNT.reg, 0,
                                  TC3->COUNT16.CC[0].reg, 0, compareValue);
      TC3->COUNT16.CC[0].reg = compareValue;
      TC_wait_for_sync();

      TC3->COUNT16.CTRLA.bit.ENABLE = 1;
      TC_wait_for_sync();

      break;
    
    case 4:
      TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIVN;
      TC_wait_for_sync();

      // Make sure the count is in a proportional position to where it was
      // to prevent any jitter or disconnect when changing the compare value.
      TC4->COUNT16.COUNT.reg = map(TC4->COUNT16.COUNT.reg, 0,
                                  TC4->COUNT16.CC[0].reg, 0, compareValue);
      TC4->COUNT16.CC[0].reg = compareValue;
      TC_wait_for_sync();

      TC4->COUNT16.CTRLA.bit.ENABLE = 1;
      TC_wait_for_sync();

      break;
    
    case 5:
      TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIVN;
      TC_wait_for_sync();

      // Make sure the count is in a proportional position to where it was
      // to prevent any jitter or disconnect when changing the compare value.
      TC5->COUNT16.COUNT.reg = map(TC5->COUNT16.COUNT.reg, 0,
                                  TC5->COUNT16.CC[0].reg, 0, compareValue);
      TC5->COUNT16.CC[0].reg = compareValue;
      TC_wait_for_sync();

      TC5->COUNT16.CTRLA.bit.ENABLE = 1;
      TC_wait_for_sync();

      break;
  }
}


void TC0_Handler() {
  // If this interrupt is due to the compare register matching the timer count
  if (TC0->COUNT16.INTFLAG.bit.MC0 == 1) {
    TC0->COUNT16.INTFLAG.bit.MC0 = 1;
    (*func0)();
  }
}

void TC1_Handler() {
  // If this interrupt is due to the compare register matching the timer count
  if (TC1->COUNT16.INTFLAG.bit.MC0 == 1) {
    TC1->COUNT16.INTFLAG.bit.MC0 = 1;
    (*func1)();
  }
}

void TC2_Handler() {
  // If this interrupt is due to the compare register matching the timer count
  if (TC2->COUNT16.INTFLAG.bit.MC0 == 1) {
    TC2->COUNT16.INTFLAG.bit.MC0 = 1;
    (*func2)();
  }
}

void TC3_Handler() {
  // If this interrupt is due to the compare register matching the timer count
  if (TC3->COUNT16.INTFLAG.bit.MC0 == 1) {
    TC3->COUNT16.INTFLAG.bit.MC0 = 1;
    (*func3)();
  }
}

void TC4_Handler() {
  // If this interrupt is due to the compare register matching the timer count
  if (TC4->COUNT16.INTFLAG.bit.MC0 == 1) {
    TC4->COUNT16.INTFLAG.bit.MC0 = 1;
    (*func4)();
  }
}

void TC5_Handler() {
  // If this interrupt is due to the compare register matching the timer count
  if (TC5->COUNT16.INTFLAG.bit.MC0 == 1) {
    TC5->COUNT16.INTFLAG.bit.MC0 = 1;
    (*func5)();
  }
}
