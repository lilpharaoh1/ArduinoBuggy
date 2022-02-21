#include "MotorControl.h"
#include "SR04.h"
#include "IRControl.h"

#define ir_pin A0

#define right_enable 9
#define right_for 8
#define right_back 7

#define left_enable 6
#define left_for 5
#define left_back 4

#define trig_pin 3
#define echo_pin 2

MotorControl motor = MotorControl(left_enable, left_for, left_back, right_enable, right_for, right_back);
SR04 sr04 = SR04(echo_pin, trig_pin);
IRControl ircontrol = IRControl(ir_pin);

//signal smoothing stuff 

const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int dir = 1;  
int sonar_counter = 0;  
double turn = 0;                  


int dist = 15, tolerance = 2;
long sonar_dist;
int test_counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ir_pin, INPUT);
  
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  };
}

void loop() {
  // put your main code here, to run repeatedly:
  // 0.26m/s Full Speed
  

 // Line Following
 // white = 50
 // black = 700

  //IR Input
  ircontrol.read_value();

  //Signal Smoothing
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = ircontrol.get_ir_value();
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  double value = total / numReadings;
  // Serial.print("Value :");
  // Serial.println(value);

  if (sonar_counter >= 100) {

    // Silver Challenge
  sonar_dist = sr04.Distance();

  if (sonar_dist == 0);
  else if (sonar_dist > dist + tolerance) {
    dir = 1;
  }
  else if (sonar_dist < dist - tolerance) {
    dir = -1;
  }
  else {
    dir = 0;
  }

  sonar_counter = 0;
}
else { 
  sonar_counter++;
}

  if (dir == 1) 
    turn = (180.0/800.0)*ircontrol.get_ir_value() - 90.0;
  else if (dir == -1)
    turn = -(180.0/800.0)*ircontrol.get_ir_value() + 90.0;
  else turn = 0;


  Serial.print("Turn: ");
  Serial.println(turn);

  motor.drive(200, dir, turn);
 
}
