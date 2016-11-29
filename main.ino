/*
**Sous-Vide Software using PID
**AllAboutCircuits.com
**Author : Cezar Chirila
*/

#include "main.h"

void setup()
{
  //Start the windowStartTime for the PID
  windowStartTime = millis();
  //Tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);
  //Set PID sample time to 1000
  myPID.SetSampleTime(1000);
  //Turn the PID on
  myPID.SetMode(AUTOMATIC);
  //Set relay pin as output
  pinMode(relay_pin, OUTPUT);
  //Initialise the display
  lcd.init();
  lcd.backlight();
  //Set top and bottom text to default values
  //Top =  First row
  //Bottom = Second row
  top = topDefault;
  bottom = botDefault;
  topOld = top;
  botOld = bottom;
  //Print default screen
  lcd.setCursor(0, 0);
  lcd.print(top);
  lcd.setCursor(0, 1);
  lcd.print(bottom);
  //Set pins connected to push buttons as inputs and set up internal pull up resistor
  pinMode(button1Pin, INPUT);     // set pin to input
  digitalWrite(button1Pin, HIGH); // turn on pullup resistors
  pinMode(button2Pin, INPUT);     // set pin to input
  digitalWrite(button2Pin, HIGH); // turn on pullup resistors
  pinMode(button3Pin, INPUT);     // set pin to input
  digitalWrite(button3Pin, HIGH); // turn on pullup resistors
  leftOld = left;
  rightOld = right;
  okOld = ok;
  //This is for the timeout of the backlight
  lastAction = millis();
  //Reading of the temperature
  lastTempUpdate = millis();
  //initialise all readings with zero
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
  {
    readings[thisReading] = 0;
  }

}

void loop()
{
  //Check if we have started and what time it is
  if ((start ==  true) && ((millis() - startTime) < (cookingTimeMin * 60 * 1000))) {
    //If we are still in time and have to cook, then we go ahead and cook
    cook();
    //In this time, display the current temperature and remaining time
    if (submenu == 0 || submenu == 4)
    {
      if(millis()-lastTempUpdate > 1000)
      {
        tempToDisplay = Input;
        lastTempUpdate = millis();
      }
      top = "     T=";
      top += tempToDisplay;
      top += "\337C     ";
      bottom = "Countdown=";
      bottom += cookingTimeMin - ((millis() - startTime) / 60000);
      bottom += "min";
    }
  }
  else
  {
    //Turn off the sous-vide
    digitalWrite(relay_pin, LOW);
    
    if (submenu == 0)
    {
      if(!topDefault.equals(top))
        lcd.clear();
      top = topDefault;
      bottom = botDefault;
    }
  }
  updateButtons();  //update state of the buttons
  checkBacklight(); //check if we need to turn the backlight on/off
  checkMenu();      //check if we need to navigate around the menu
  updateDisplay();  //update what is shown on the display

}

void updateDisplay()
{
  //Check if we need to update the display
  if (!topOld.equals(top) || !botOld.equals(bottom))
  {

    lcd.setCursor(0, 0);
    lcd.print(top);
    lcd.setCursor(0, 1);
    lcd.print(bottom);

    //If we need to display the temperature
    //We set the cursor and blink the digit that we are adjusting
    if (submenu == 2 || submenu == 3 || submenu == 4 || submenu == 5)
    {
      if (submenu == 2 || submenu == 5)
        lcd.setCursor(1, 1);
      else
        lcd.setCursor(0, 1);
      lcd.blink();
    }
    else
    {
      lcd.noBlink();
    }
    //Update the old variables
    topOld = top;
    botOld = bottom;
  }
}

void updateButtons()
{
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (digitalRead(button1Pin) == left)
      left = !left;
    if (digitalRead(button2Pin) == ok)
      ok = !ok;
    if (digitalRead(button3Pin) == right)
      right = !right;
    lastDebounceTime = millis();
  }
}
//Function to show the temperature and add degrees symbol and celsius
void setBotTemp()
{
  bottom = (int)temperature;
  bottom += "\337C";
}

void setBotCookingTime()
{
  bottom = "";
  if (cookingTimeMin < 100)
    bottom += "0";
  bottom += cookingTimeMin;
  bottom += "min";
}

void checkMenu() {
  if (leftOld != left || rightOld != right || okOld != ok)
  {
    if (ok)
    {
      switch (submenu)
      {
        case 0:
          {
            top = "   Main  Menu";
            bottom = menuItems[displayChoices];
            lcd.clear();
            submenu = 1;
            break;
          }
        case 1:
          {
            if (displayChoices == 0)
            {
              start = !start;
              lcd.clear();
              submenu = 0;
              startTime = millis();
            }
            if (displayChoices == 1)
            {
              top = "Temperature :";
              setBotTemp();
              lcd.clear();
              submenu = 2;
            }
            if (displayChoices == 2)
            {
              lcd.clear();
              submenu = 0;
            }
            if (displayChoices == 3)
            {
              top = "Set cooking time";
              setBotCookingTime();
              lcd.clear();
              submenu = 5;
            }
            break;
          }
        case 2:
          {
            submenu = 3;
            lcd.setCursor(0, 1);
            break;
          }
        case 3:
          {
            Setpoint = temperature;
            submenu = 0;
            break;
          }
        case 4:
          {
            submenu = 0;
            lastAction = millis();
            break;
          }
        case 5:
          {
            submenu = 6;
            lcd.setCursor(0, 1);
            break;
          }
        case 6:
          {
            submenu = 0;
            break;
          }
      }
    }

    if (left)
    {
      switch (submenu)
      {
        case 1:
          {
            if (displayChoices > 0)
            {
              displayChoices--;
              lcd.clear();
            }
            bottom = menuItems[displayChoices];
            break;
          }
        case 2:
          {
            if (temperature > 20)
            {
              temperature--;
              setBotTemp();
            }
            break;
          }
        case 3:
          {
            if (temperature >= 30)
            {
              temperature -= 10;
              setBotTemp();
            }
            break;
          }
        case 5:
          {
            if (cookingTimeMin > 10) {
              cookingTimeMin -= 10;
              setBotCookingTime();
            }
            break;
          }
        case 6:
          {
            if (cookingTimeMin > 100) {
              cookingTimeMin -= 100;
              setBotCookingTime();
            }
            break;
          }
      }
    }

    if (right)
    {
      switch (submenu)
      {
        case 1:
          {
            if (displayChoices < 3)
            {
              displayChoices++;
              lcd.clear();
            }
            bottom = menuItems[displayChoices];
            break;
          }
        case 2:
          {
            if (temperature < 80)
            {
              temperature++;
              setBotTemp();
            }
            break;
          }
        case 3:
          {
            if (temperature <= 70)
            {
              temperature += 10;
              setBotTemp();
            }
            break;
          }
        case 5:
          {
            if (cookingTimeMin < 280) {
              cookingTimeMin += 10;
              setBotCookingTime();
            }
            break;
          }
        case 6:
          {
            if (cookingTimeMin < 200) {
              cookingTimeMin += 100;
              setBotCookingTime();
            }
            break;
          }
      }
    }
    leftOld = left;
    rightOld = right;
    okOld = ok;
    lastAction = millis();
  }
}

void checkBacklight()
{
  //After 30 seconds we shut down the backlight
  if (millis() - lastAction > timeout)
  {
    lcd.noBacklight();
    submenu = 4;
  }
  else
    lcd.backlight();
}


void cook()
{
  Input = read_temp();
  myPID.Compute();
  if (millis() - windowStartTime > WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if (Output > millis() - windowStartTime)
    digitalWrite(relay_pin, HIGH);
  else
    digitalWrite(relay_pin, LOW);
}

double read_temp()
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
  double realtemp = ((average * (5000.0 / 1023.0)) - 400) / 19.5; //Formula used by the sensor
  return realtemp;
}

