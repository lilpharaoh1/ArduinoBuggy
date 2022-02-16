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
int dir; 
int prev_error = 0;  
int speed = 200;                   


int dist = 15, tolerance = 4;
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
  

    // Steering Control
//  motor.drive(255, 1, 0.0);
//  delay(2000);
//  motor.drive(255, 1, 45.0);
//  delay(2000);
//  motor.drive(255, 1, -45.0);
//  delay(2000);
//  motor.drive(255, -1, 80.0);
//  delay(2000);

    // Silver Challenge

//    motor.drive(255, 1); // give a value between 0 and 255  
  sonar_dist = sr04.Distance();
//  Serial.print(sonar_dist);
//  Serial.println("cm");
//  delay(0);

  if (sonar_dist == 0);
  else if (sonar_dist > dist) {
//    motor.drive(255, 1, 0);
    dir = 1;
//    Serial.println("Drive Forward!");
  }
  else if (sonar_dist < dist) {
//    motor.drive(255, -1, 0);
    dir = -1;
//    Serial.println("Drive Backward!");
  }
  else {
    dir = 0;
//    Serial.println("Brake!");
  }

  int sonar_error = abs(dist - sonar_dist);

  speed += sonar_error;

  if (speed < 0) speed = 0;
  else if (speed > 255) speed = 255;

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

 double turn = (180.0/800.0)*value - 90;

 if (dir == -1) {
  turn *= -1;
 }

// Serial.print("Turn :");
// Serial.println(turn);

// if (value  < 500) { 
//  motor.drive(200, 1, 80);
// }
// else {
//  motor.drive(200, 1, -80);
// }

  Serial.println(dir);

  motor.drive(200, dir, turn);
 
}
