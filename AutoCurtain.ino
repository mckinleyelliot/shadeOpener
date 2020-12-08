//HI THERE. THIS IS CODE TO RUN MY ARDUINO UNO R3 WITH A STEPPER MOTOR, BUTTON, AND A CLOCK TO OPEN AND CLOSE MY BEDROOM BLINDS.
#include <Stepper.h>
#include <DS1302.h>
#include <Wire.h>
// THESE 2 LINES ENSURE A "FLICKERING" POWER SOURCE DOESN'T TOGGLE SWITCH
long time = 0;
long debounce = 200;
//THESE SETTINGS ARE SPECIFIC FOR MY STEPPER MOTOR (STEP # AND GEAR REDUCTION)
const float STEPS = 32;
const float gear_red = 64;
const float steps_per_out_rev = STEPS * gear_red;
int stepsrequired;
//PINS FOR STEPPER MOTOR CONTROLLER
Stepper stepper(STEPS, 9, 11, 8, 10);
//PINS FOR INPUT (SWITCH) AND OUTPUT (LED)
int inPin = 2;
int outPin = 3;
//TOGGLE SWITCH STATE FOR OPEN/CLOSED
int state = HIGH;
int reading;
int previous = LOW;
// DS1302:
//          VCC pin -> 3.3v power (NOT 5v)
//          GND -> GND
//          CLK pin -> Arduino Digital 6
//          DATA pin -> Arduino Digital 5
//          RST pin -> Arduino Digital 4
DS1302 rtc(4, 5, 6);
// INIT A TIME DATA-STRUCTURE
Time t;
void setup()
{
  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);
  //TURN ON THE CLOCK RAM
  rtc.halt(false);
  rtc.writeProtect(false);
  // UN COMMENT TO SET DATE/TIME
  //rtc.setDOW(MONDAY);        // Set Day-of-Week to FRIDAY
  rtc.setTime(6, 59, 55);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(4, 13, 2020);   // Set the date to August 6th, 2010
  Serial.begin(9600);
}
void loop()
{
  reading = digitalRead(inPin);
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH) {
      state = LOW;
      Serial.println("OPENING");
      //Half turn CW
      stepsrequired = 8192;
      stepper.setSpeed(700);
      stepper.step(stepsrequired);
      Serial.println("OPENED");
      delay(2000);
    }
    else {
      state = HIGH;
      Serial.println("CLOSING");
      //Half turn CCW
      stepsrequired = -8192;
      stepper.setSpeed(700);
      stepper.step(stepsrequired);
      Serial.println("CLOSED");
      delay(2000);
    }
  }
  t = rtc.getTime();
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(":");
  Serial.println(t.sec);
  if (t.hour == 7 && t.min == 0 && t.sec == 0)
  {
    Serial.println("OPENING, GOOD MORNING");
    state = LOW;
    //Half turn CW
    stepsrequired = 8192;
    stepper.setSpeed(700);
    stepper.step(stepsrequired);
    Serial.println("OPENED");
    delay(2000);
  }
  digitalWrite(outPin, state);
  if (state == HIGH) {
    Serial.println("SHADES ARE CLOSED");
  }
  else if (state == LOW) {
    Serial.println("SHADES ARE OPEN");
  }
  //Serial.println(rtc.getTimeStr());
  previous = reading;
}
