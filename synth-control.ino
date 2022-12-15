#include "SAMD51_InterruptTimer.h"
#include "pwmHandler.h"
#include "waveforms/SDHandling.h"
#include <MIDI.h>
#include <vector>

#define N_SAMPLES 2048          // Number of samples in each wavetable entry
#define N_WAVEFORMS 3           // Number of waveforms stored in wavetable
#define WAVEFORM_SELECT_PIN 1   // Pin for reading hardware to change waveforms


TC_Timer TC_adsr(4);         // Interrupt timer for envelope generator
TC_Timer TC_Midi(3);         // Interrupt timer for converting MIDI to analog waveforms
TC_Timer TC_knob(2);         // Interrupt timer for reading filter knobs
TC_Timer TC_adsrParams(1);   // Interrupt timer for reading adsr knobs

// Custom PWM writers for pins 5 - 7
pwmHandler pwm5(5);          // for filter cutoff control signal
pwmHandler pwm6(6);          // for filter Q control signal
pwmHandler pwm7(7);          // for ADSR envelope signal

MIDI_CREATE_DEFAULT_INSTANCE();

double A440_num = 69;

double wavetable[N_WAVEFORMS][N_SAMPLES];
int counter = 0;
int downsample = 1;

int WAVEFORM_SEL_IDX = 1; // 0: sine, 1: square, 2: sawtooth

// For reading manual cutoff frequency and Q control knobs
int cutoffPin = A2;
int cutoffVal = 255;
int qPin = A3; 
int qVal = 0;

// For reading ADSR knobs
int APin = A4; 
int DPin = A5;
int SPin = A6;
int RPin = A7;
int attack = 100;
int decay = 200;
int sustain_parameter = 127;
int release = 1000;
int maxVCA = 255;

// Keep track of last waveform change for de-bouncing
unsigned long last_waveform_isr_time = 0;

// ISR function to send waveform samples to DAC
void noteISR() {
  analogWrite(A0, wavetable[WAVEFORM_SEL_IDX][downsample * counter] * (pow(2,11) - 1));
  counter = (counter + 1) % (N_SAMPLES / downsample);
}

// ISR function to change waveforms from user input
void waveformISR() {
  unsigned long isrTime = millis();
  if (isrTime - last_waveform_isr_time > 200) {
    WAVEFORM_SEL_IDX = (WAVEFORM_SEL_IDX + 1) % N_WAVEFORMS;
  }
  last_waveform_isr_time = isrTime;
  Serial.print("waveform select idx: ");
  Serial.println(WAVEFORM_SEL_IDX);
}

// ISR to check the filter control knobs
void filterKnobISR() {
  // adjust cutoff frequency and Q
  cutoffVal = analogRead(cutoffPin);
  cutoffVal = cutoffVal / 4;
  qVal = analogRead(qPin);
  qVal = 255 - qVal / 4;
  pwm5.fast_pwm_analogWrite(cutoffVal);
  pwm6.fast_pwm_analogWrite(qVal);
}

// ISR to check the ADSR control knobs
void adsrKnobISR() {
  int div = 1024 / (maxVCA + 1);
  // adjust ADSR envelope parameters
  int attackVal = analogRead(APin);
  attack = maxVCA - (attackVal / div);
  int decayVal = analogRead(DPin);
  // decay = maxVCA - (decayVal / div);
  decay = (maxVCA + 1) * div - decayVal;
  int sustainVal = analogRead(SPin);
  sustain_parameter = maxVCA - (sustainVal / div);
  int releaseVal = analogRead(RPin);
  release = (maxVCA + 1) * div - releaseVal;
}

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(9600);
  // while (!Serial) {}

  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

  Serial.println("Entering setup()");
  Serial.println(WAVEFORM_SEL_IDX);

  // Populate wavetable from SD Card memory
  init_SDCard();

  std::vector<double> sine = read_SDCard("sine.txt", 2048);
  std::vector<double> square = read_SDCard("square.txt", 2048);
  std::vector<double> saw = read_SDCard("sawtooth.txt", 2048);

  for (int i = 0; i < N_SAMPLES; i++) {
    wavetable[0][i] = sine[i];
    wavetable[1][i] = square[i];
    wavetable[2][i] = saw[i];
  }

  Serial.println("MIDI begin");

  MIDI.begin(MIDI_CHANNEL_OMNI); // initialize the Midi Library (listen to all channels)
  MIDI.setHandleNoteOn(MyHandleNoteOn); // set callback function for when Note On is receieved
  MIDI.setHandleNoteOff(MyHandleNoteOff); // set callback function for Note Off

  // set Q control and cutoff frequency
  pwm6.fast_pwm_analogWrite(qVal);  // Q control
  pwm5.fast_pwm_analogWrite(cutoffVal); // cutoff frequency
  TC_knob.startTimer(100000, filterKnobISR); 

  // set ADSR values
  TC_adsrParams.startTimer(1000000, adsrKnobISR);

  // Set hardware interrupt for waveform selection
  pinMode(WAVEFORM_SELECT_PIN, INPUT_PULLUP); //Setup internal pullup for digital input
  attachInterrupt(digitalPinToInterrupt(WAVEFORM_SELECT_PIN), waveformISR, FALLING); //Create interrupt whenever this pin is pulled low
}

// the loop function waits for MIDI data
void loop() {
  MIDI.read();
}

// Global variables for ADSR EG
int attack_count = 0;
int decay_count = 0;
int release_count = 0;
uint8_t sustain = 0;
bool attack_done = false;
bool decay_done = false;
bool release_done = false;
bool midi_off = false;
bool note_playing = false;
int decay_time_between_writes = 0;
int release_time_between_writes = 0;
uint8_t last_value;
int current_note;

// MIDI Note On Handler
void MyHandleNoteOn(byte channel, byte pitch, byte velocity) { 

  current_note = int(pitch);

  double diff = double(pitch) - A440_num;
  double freq = 440 * pow(2, diff/12);

  Serial.println(freq);
  Serial.println(pitch);

  // -------- Uncomment this section to improve tuning --------
  // if (pitch < 15) {
  //   diff = double(pitch) - (A440_num - 60);
  //   freq = 13.75 * pow(2, diff/12);
  // } else if (pitch >= 15 && pitch < 27) {
  //   diff = double(pitch) - (A440_num - 48);
  //   freq = 27.5 * pow(2, diff/12);
  // } else if (pitch >= 27 && pitch < 39) {
  //   diff = double(pitch) - (A440_num - 36);
  //   freq = 55 * pow(2, diff/12);
  // } else if (pitch >= 39 && pitch < 51) {
  //   diff = double(pitch) - (A440_num - 24);
  //   freq = 110 * pow(2, diff/12);
  // } else if (pitch >= 51 && pitch < 63) {
  //   diff = double(pitch) - (A440_num - 12);
  //   freq = 220 * pow(2, diff/12);
  // } else if (pitch >= 63 && pitch < 75) {
  //   diff = double(pitch) - A440_num;    
  //   freq = 440 * pow(2, diff/12);
  // } else if (pitch >= 75 && pitch < 87) {
  //   diff = double(pitch) - (A440_num + 12);    
  //   freq = 880 * pow(2, diff/12);
  // } else if (pitch >= 87 && pitch < 99) {
  //   diff = double(pitch) - (A440_num + 24);    
  //   freq = 1760 * pow(2, diff/12);
  // } else {
  //   diff = double(pitch) - (A440_num + 36);    
  //   freq = 3520 * pow(2, diff/12);
  // }
  // ------------------------------------------------------------

  if (pitch > 54 && pitch < 67) {
    downsample = 2;
  } else if (pitch < 79) {
    downsample = 4;
  } else if (pitch < 91) {
    downsample = 8;
  } else if (pitch < 103) {
    downsample = 16;
  } else if (pitch < 115) {
    downsample = 32;
  } else {
    downsample = 64;
  }

  double samps_per_sec = freq * N_SAMPLES / downsample;
  
  if (note_playing) {
    TC_Midi.stopTimer();
    TC_adsr.stopTimer();

    attack_count = 0;
    decay_count = 0;
    release_count = 0;
    attack_done = false;
    decay_done = false;
    release_done = false;
    midi_off = false;
  }  

  // Time is given in 10s of ns
  TC_Midi.startTimer(((1.0 / samps_per_sec) * 100000000.0), noteISR); // start playing note
  note_playing = true;
  // writeADSREnvelope(100, 200, 127, 1000);
  writeADSREnvelope(attack, decay, sustain_parameter, release);
}

// MIDI Note Off Handler
void MyHandleNoteOff(byte channel, byte pitch, byte velocity) { 
  if (int(pitch) == current_note) {
    attack_count = 0;
    decay_count = 0;
    attack_done = true;
    decay_done = true;
    midi_off = true;
    release_time_between_writes = release_time_between_writes / last_value;
    TC_adsr.stopTimer();
    TC_adsr.startTimer(release_time_between_writes, adsrISR);
    // TC_Midi.stopTimer(); // stop playing note
  }
}

// ISR function to produce ADSR envelope
void adsrISR() {

    if (!attack_done) {
      if (attack_count < maxVCA + 1) {
        pwm7.fast_pwm_analogWrite(maxVCA - attack_count);
        last_value = attack_count;
        attack_count++;
      } else {
        attack_count = 0;
        TC_adsr.stopTimer();
        TC_adsr.startTimer(decay_time_between_writes, adsrISR);
        attack_done = true;
      }
    } else if (!decay_done) {
      if ((maxVCA-decay_count) >= sustain && decay_count < maxVCA + 1) {
        pwm7.fast_pwm_analogWrite(decay_count);
        last_value = maxVCA - decay_count;
        decay_count++;
      } else {
        decay_count = 0;
        // TC_adsr.stopTimer();
        // TC_adsr.startTimer(release_time_between_writes, adsrISR);
        decay_done = true;
      }
    } else if (midi_off) {
      if ((last_value - release_count) >= 0) {
        // analogWrite(5, last_value - release_count);
        pwm7.fast_pwm_analogWrite(maxVCA - (last_value - release_count));      
        release_count++;
      } else {
        TC_adsr.stopTimer();
        TC_Midi.stopTimer();
        
        attack_count = 0;
        decay_count = 0;
        release_count = 0;
        attack_done = false;
        decay_done = false;
        release_done = false;
        midi_off = false;
        note_playing = false;
        analogWrite(A0, 0); // reset DAC output to 0
        
      }
    }

}

// Function to trigger ADSR envelope (input times in milliseconds)
void writeADSREnvelope(int A, int D, uint8_t S, int R) {
  if (A == 0) {
    A = 1;
  }  
  int attack_tens_of_ns = A * 1000 * 100;
  int attack_time_between_writes = attack_tens_of_ns / (maxVCA + 1);
  
  int decay_tens_of_ns = D * 1000 * 100;
  decay_time_between_writes = decay_tens_of_ns / (maxVCA + 1 - S);
  sustain = S;

  int release_tens_of_ns = R * 1000 * 100;
  // release_time_between_writes = release_tens_of_ns / 256;
  release_time_between_writes = release_tens_of_ns;

  TC_adsr.startTimer(attack_time_between_writes, adsrISR);
}
