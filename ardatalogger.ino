#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

// global variables
RTC_DS3231 timeRTC;

Sd2Card card;
File myFile;

float tempMeasure = 0.0f;
float voltMeasure = 0.0f;
int32_t timeMeasure = 0.0f;

void setup() {
  Serial.begin(57600);
  
  // initialize RTC
  if (!timeRTC.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  // check to see if the SD has successfully initialized
  Serial.println("Initializing SD card");
  if (!card.init(SPI_HALF_SPEED, 10)) {
    Serial.println("SD initialization failed");
    while (1);
  }
  if (!SD.begin(10)) {
    Serial.println("SD initialized");
  while (1);
  }  
}

void loop() {
  // read values and assign to global variables
  DateTime now = timeRTC.now();
  timeMeasure = now.unixtime();
  tempMeasure = timeRTC.getTemperature();
  voltMeasure = analogRead(0)  / 204.6f;

  // gather csv values
  String stats = String(timeMeasure) + "," + String(tempMeasure) + "," + String(voltMeasure);
  
  // print to serial port
  Serial.print("time: ");
  Serial.println(timeMeasure);
  Serial.print("temperature: ");
  Serial.println(tempMeasure);
  Serial.print("voltage: ");
  Serial.println(voltMeasure);

  // open memory card
  myFile = SD.open("teststat.csv", FILE_WRITE);

  // if the file opened okay, write to it
  if (myFile) {
    Serial.println("Writing to file");
    myFile.println(stats);
    // close the file
    myFile.close();
  } else {
    // if the file didn't open, print an error
    Serial.println("Error opening file");
  }
  
  delay(60000);
}
