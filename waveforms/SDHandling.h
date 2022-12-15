#include <SD.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <sstream>

// const int chipSelect = SDCARD_SS_PIN;

void init_SDCard() {
  pinMode(SDCARD_SS_PIN, OUTPUT);
 
  if (!SD.begin(SDCARD_SS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}


// Read a file containing a comma-separated list of N doubles,
// return the data as a vector of doubles
std::vector<double> read_SDCard(String filename, int N) {
    std::vector<double> v(N, 0.0);

    File myFile = SD.open(filename);
    Serial.print("Reading from file ");
    Serial.println(filename);

    if (myFile) {
        std::string data;
        while (myFile.available()) {
            data += char(myFile.read());
        }

        std::stringstream ss(data);

        int idx = 0;
        Serial.println("string streaming");
        while (ss.good()) {
            std::string substr;
            std::getline(ss, substr, ',');
            v[idx] = std::stod(substr);
            Serial.print(std::stod(substr));
            Serial.print(" ");
            idx++;

            if (idx >= N) {
              break;
            }
        }
        Serial.println("");

        Serial.print("v: ");
        for (int i = 0; i < v.size(); i++) {
          Serial.print(v[i]);
          Serial.print(" ");
        }
        Serial.println("");

        // close the file:
        myFile.close();

    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening file for reading");
    }

    return v;
}


// Write a comma-separated list of doubles to the SD card
void write_SDCard(String filename, std::vector<double> vals) {
  File myFile = SD.open(filename, FILE_WRITE);

  if (myFile) {
    String dataString = "";

    for (int i = 0; i < vals.size(); i++) {
      dataString = dataString + String(vals[i]);
      dataString = dataString + ",";
    }

    myFile.println(dataString);

    myFile.close();
    Serial.println("file write compelete");

  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file for writing");
  }
}