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

#ifndef SAMD51_ISR_Timer_h
#define SAMD51_ISR_Timer_h

class TC_Timer {
  public:
    TC_Timer();
    TC_Timer(int TC_num);
    void startTimer(unsigned long period, void (*f)());
    void stopTimer();
    void restartTimer(unsigned long period);
    void setPeriod(unsigned long period);
    void setTCNumber(int n);
    int getTCNumber();

  private:
    int TC_num;
    void TC_wait_for_sync();
};

#endif
