#ifndef SousVide_h
#define SousVide_h

#include "Arduino.h"

class SousVide
{
private:
  
  //Integers to store which pins we use
  int sensor_pin;
  int relay_pin;
  //Variables to the set and other temperatures
  float temp;
  float curr_temp;
  float last_temp;
  bool first_rise = true;
  //Variables to store time
  unsigned long last_update;
  unsigned long closer;
  unsigned long almost;
  unsigned long keep;
  //Variables for sensor value smoothing
  const static int numReadings = 5;
  int readings[numReadings]; // the readings from the analog input
  int readIndex = 0;         // the index of the current reading
  int total = 0;             // the running total
  int average = 0;           // the average
  int inputPin = A0;
  
  void relay_adjust(unsigned long check_time, float curr_temp);

public:
  SousVide(int sp, int rp);
  void set_temp(float st);
  float read_temp();
  void cook();
  //Debbuging - Just use Serial.println(status) or if you use SoftwareSerial yourSoftwareSerialName.println(status)
  String status; 
};

#endif
