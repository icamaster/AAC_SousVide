#include "Arduino.h"
#include "SousVide.h"

SousVide::SousVide(int sp, int rp)
{
  //Initialise the pins that we use
  sensor_pin = sp;
  relay_pin = rp;
  pinMode(relay_pin, OUTPUT);
  //Attribute start-up time to be the last update
  last_update = millis();
  //Initialise values to close, almost and keep time. TODO methods to change them
  closer = 10000;
  almost = 8000;
  keep = 5000;
  temp = 0;
  last_temp = read_temp();
  //Set values to all members of sensor smoothing array
  for (int thisReading = 0; thisReading < numReadings; thisReading++) 
  {
    readings[thisReading] = 0;
  }
}

  void SousVide::set_temp(float st)
  {
    if (st > 20 && st < 80)
      temp = st;
  }

  float SousVide::read_temp()
  {
    
    // subtract the last reading:
    total = total - readings[readIndex];
    // read from the sensor:
    readings[readIndex] = analogRead(sensor_pin);
    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;
    // if we're at the end of the array...
    if (readIndex >= numReadings)
    {
      // ...wrap around to the beginning:
      readIndex = 0;
    }
    // calculate the average:
    average = total / numReadings;
    float realtemp = ((average * (5000.0 / 1023.0)) - 400) / 19.5; //Formula used by the sensor
    return realtemp;
  }

  void SousVide::cook()
  {
    curr_temp = read_temp();

    if (curr_temp >= temp)
    {
      digitalWrite(relay_pin, LOW);
      status = "STEP 1";
    }
    else if (curr_temp < temp - 10.0)
    {
      digitalWrite(relay_pin, HIGH);
      status = "STEP 2";
    }
    else if ((curr_temp >= temp - 10.0) && (curr_temp < temp - 4.0))
    {
      relay_adjust(closer, curr_temp);
      status = "STEP 3";
    }
    else if ((curr_temp >= temp - 4.0) && (curr_temp < temp - 2.0))
    {
      relay_adjust(almost, curr_temp);
      status = "STEP 4";
    }
    else if ((curr_temp >= temp - 2.0) && (curr_temp < temp))
    {
      relay_adjust(keep, curr_temp);
      status = "STEP 5";
    }
  }

  void SousVide::relay_adjust(unsigned long check_time, float curr_temp)
  {
    //Check if sufficient time has passed
    if (millis() - last_update > check_time)
    {
      //Check if the temperature is rising or getting lower
      if (curr_temp - last_temp < 1.0)
      {
        digitalWrite(relay_pin, HIGH); 
      }
      else
      {
        digitalWrite(relay_pin, LOW);
      }
      last_temp = curr_temp;
      last_update = millis();
    }
  }
