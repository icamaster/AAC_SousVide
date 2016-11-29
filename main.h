#include <PID_v1.h>
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4); 

//PID
//Define Variables we'll be connecting to
double Setpoint, Input, Output;
//Specify the links and initial tuning parameters
double Kp = 850, Ki = 0.1, Kd = 0.25;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
int WindowSize = 10000;
unsigned long windowStartTime;

//Constant and variables for smoothing the sensor data
const int numReadings = 5;
int readings[numReadings]; // the readings from the analog input
int readIndex = 0;         // the index of the current reading
int total = 0;             // the running total
int average = 0;

//Constants (pin assignements)
const int relay_pin = 2;
const int sensor_pin = A0;
const int button1Pin = 7; //LEFT
const int button2Pin = 6; //OK
const int button3Pin = 5; //RIGHT

//Text for LCD. There are spaces because the cursor is constant
const String topDefault = "AAC  mySous-Vide";
const String botDefault = "      Menu";
const String menuItems[] = {"  Start / Stop", "Set  temperature", "      Exit", "Set cooking time"};
//LCD text Variables
String top;
String bottom;
String topOld;
String botOld;
int displayChoices = 0;
int submenu = 0;

//Button Variables
bool left = LOW;
bool ok = LOW;
bool right = LOW;
bool leftOld;
bool okOld;
bool rightOld;

//Time variables and consatnt
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers
unsigned long timeout = 30000;      // How many seconds to keep the display on
unsigned long lastAction;
unsigned long lastTempUpdate;

//Cooking variables
bool start = false;
int temperature = 20;
unsigned long startTime;
int cookingTimeMin = 60; 
int tempToDisplay;
