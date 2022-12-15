#include "SDHandling.h"
#include <vector>

#define N_SAMPLES 2048
 
File myFile;
const int chipSelect = SDCARD_SS_PIN;

int mode = 1; // 0: read; 1: write

void additive_synthesis(int Ns, int n_terms, std::vector<double> &xx, std::vector<double> &a) {
  for (int k = 0; k < n_terms; k++) {
    // add the current harmonic
    for (int i = 0; i < Ns; i++) {
      xx[i] = xx[i] + a[k] * sin(2 * PI * (k+1) * i / Ns);
    }
  }

  // normalize to between 0 and 1
  double min, max;
  for (int i = 0; i < xx.size(); i++) {
    if (i == 0) {
      min = xx[i];
      max = xx[i];
    } else {
      if (xx[i] < min) {
        min = xx[i];
      } else if (xx[i] > max) {
        max = xx[i];
      }
    }
  }

  for (int i = 0; i < xx.size(); i++) {
    xx[i] = (xx[i] - min) / (max - min);
  }
}

std::vector<double> makeSine(int Ns) {
  std::vector<double> sinevec(Ns, 0.0);
  for (int i = 0; i < Ns; i++) {
    double angle = 2 * PI / Ns * i;
    sinevec[i] = 0.5 * sin(angle) + 0.5;
  }

  return sinevec;
}

std::vector<double> makeSquare(int Ns) {
  std::vector<double> sqrvec(Ns, 0.0);

  int n_terms = Ns / 2;    
  std::vector<double> a_sqr(n_terms, 0.0);
  for (int k = 0; k < n_terms; k++) {
    a_sqr[k] = ((k + 1) % 2) / (double(k) + 1);
  }  

  additive_synthesis(Ns, n_terms, sqrvec, a_sqr);

  return sqrvec;
}

std::vector<double> makeSaw(int Ns) {
  std::vector<double> sawvec(Ns, 0.0);

  int n_terms = Ns / 2;
  std::vector<double> a_saw(n_terms, 0.0);
  for (int k = 0; k < n_terms; k++) {
    a_saw[k] = 1 / (double(k) + 1);
  }

  additive_synthesis(Ns, n_terms, sawvec, a_saw);

  return sawvec;
}
 

void setup()
{
  Serial.begin(9600);
  while (!Serial) {}

/*
  // Sine
  std::vector<double> sinevec(N_SAMPLES, 0.0);
  for (int i = 0; i < N_SAMPLES; i++) {
    double angle = 2 * PI / N_SAMPLES * i;
    sinevec[i] = 0.5 * sin(angle) + 0.5;
  }

  // Square
  Serial.println("Computing square wave...");
  std::vector<double> sqrvec(N_SAMPLES, 2048);

  int Ns = N_SAMPLES;
  int n_terms = Ns / 2;    
  std::vector<double> a_sqr(n_terms, 0.0);
  for (int k = 0; k < n_terms; k++) {
    a_sqr[k] = ((k + 1) % 2) / (double(k) + 1);
  }  

  std::vector<double> xx_sqr(Ns, 0.0);
  additive_synthesis(Ns, n_terms, xx_sqr, a_sqr);

  // place square wave in vector
  for (int i = 0; i < xx_sqr.size(); i++) {
    sqrvec[i] = xx_sqr[i];
  }  

  // Sawtooth
  Serial.println("Computing sawtooth wave...");
  std::vector<double> sawvec(2048, 0.0);

  std::vector<double> a_saw(n_terms, 0.0);
  for (int k = 0; k < n_terms; k++) {
    a_saw[k] = 1 / (double(k) + 1);
  }

  std::vector<double> xx_saw(Ns, 0.0);
  additive_synthesis(Ns, n_terms, xx_saw, a_saw);

  // place sawtooth in vector
  for (int i = 0; i < xx_saw.size(); i++) {
    sawvec[i] = xx_saw[i];
  }  
*/

  Serial.println("Computing waveforms...");

  std::vector<double> sine2048 = makeSine(2048);
  std::vector<double> sine1024 = makeSine(1024);
  std::vector<double> sine512 = makeSine(512);
  std::vector<double> sine256 = makeSine(256);
  std::vector<double> sine128 = makeSine(128);
  std::vector<double> sine64 = makeSine(64);
  std::vector<double> sine32 = makeSine(32);

  std::vector<double> sqr2048 = makeSquare(2048);
  std::vector<double> sqr1024 = makeSquare(1024);
  std::vector<double> sqr512 = makeSquare(512);
  std::vector<double> sqr256 = makeSquare(256);
  std::vector<double> sqr128 = makeSquare(128);
  std::vector<double> sqr64 = makeSquare(64);
  std::vector<double> sqr32 = makeSquare(32);

  std::vector<double> saw2048 = makeSaw(2048);
  std::vector<double> saw1024 = makeSaw(1024);
  std::vector<double> saw512 = makeSaw(512);
  std::vector<double> saw256 = makeSaw(256);
  std::vector<double> saw128 = makeSaw(128);
  std::vector<double> saw64 = makeSaw(64);
  std::vector<double> saw32 = makeSaw(32);

  Serial.println("Done computing waveforms");

  init_SDCard();
 
  if (mode == 0) {
    std::vector<double> vals = read_SDCard("test3.txt", 4);

  } else if (mode == 1) {

    // std::vector<double> testvec {0.34, 0.2345, .1234, 234.34};
    // write_SDCard("test3.txt", testvec);

    // write_SDCard("sine.txt", sinevec);
    // write_SDCard("square.txt", sqrvec);
    // write_SDCard("sawtooth.txt", sawvec);    

    write_SDCard("sine2048.txt", sine2048);
    write_SDCard("sine1024.txt", sine1024);
    write_SDCard("sine512.txt", sine512);
    write_SDCard("sine256.txt", sine256);
    write_SDCard("sine128.txt", sine128);
    write_SDCard("sine64.txt", sine64);
    write_SDCard("sine32.txt", sine32);

    write_SDCard("square2048.txt", sqr2048);
    write_SDCard("square1024.txt", sqr1024);
    write_SDCard("square512.txt", sqr512);
    write_SDCard("square256.txt", sqr256);
    write_SDCard("square128.txt", sqr128);
    write_SDCard("square64.txt", sqr64);
    write_SDCard("square32.txt", sqr32);

    write_SDCard("saw2048.txt", saw2048);
    write_SDCard("saw1024.txt", saw1024);
    write_SDCard("saw512.txt", saw512);
    write_SDCard("saw256.txt", saw256);
    write_SDCard("saw128.txt", saw128);
    write_SDCard("saw64.txt", saw64);
    write_SDCard("saw32.txt", saw32);

  }
}
 
void loop()
{
	// nothing happens after setup
}
