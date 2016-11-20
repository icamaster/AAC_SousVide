#include "SousVide.h"
#include <SoftwareSerial.h>

SoftwareSerial blue(12, 11); // RX, TX
SousVide mySousVide(A1, 2);

float temperature;
unsigned long update_time = 1000;
unsigned long last_update;


void setup()
{
    blue.begin(9600);
    mySousVide.set_temp(30);
    last_update = millis();
}

void loop()
{
    if(millis() - last_update > update_time)
    {
      mySousVide.cook();
      blue.println(mySousVide.read_temp());
      blue.println(mySousVide.status);
      last_update = millis();
    }
    
}
