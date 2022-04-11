#include <Arduino_LSM6DS3.h>

float x, y, z;
float a, b, c;

//PID setup
double Kp, Ki, Kd, drift, curr_drift;     // gain values which need to be tuned to get smooth motion
double setPoint = 0;     // some value between high (900) and low (50) i.e. 
unsigned long currTime, prevTime = 0;
double deltaT;
double currE, prevE = 0;
double iE, dE; // errors for integral an derivative values
double input_x, input_a;
double output;

//motor

int right_enable = 9;
int right_for = 8;
int right_back = 7;

int left_enable = 6;
int left_for = 5;
int left_back = 4;

float sped;
int direct;


void setup() {
  
if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Kp = 6;
  Ki = 0.25;
  Kd = 37.5;
  drift = 1.038; 

// motor

pinMode(right_enable, OUTPUT);
pinMode(right_for, OUTPUT);
pinMode(right_back, OUTPUT);

pinMode(left_enable, OUTPUT);
pinMode(left_for, OUTPUT);
pinMode(left_back, OUTPUT);


delay(1000);

}

void loop() {
  
    IMU.readGyroscope(x, y, z);
    input_x = x;
//    IMU.readAcceleration(a, b, c);
//    input_a = b;
//    Serial.println(c*10);
  
  output = PID(input_x);
  // Serial.println(output);
 
  sped =  abs(output);
  Serial.println(sped);
  if (output <= 0){
    direct = 1;
  }
  else{
    direct = -1;
  }
  drive(sped, direct);

  delay(1);
 
}

double PID(double input){
  currTime = millis();
  deltaT = currTime - prevTime;

  currE = setPoint - input;
  iE = iE + currE * deltaT;
  dE = (currE - prevE)/deltaT;
  curr_drift = curr_drift + drift;

  int output = (Kp * currE) + (Ki * iE) + (Kd * dE) + curr_drift;

  prevE = currE;
  prevTime = currTime;

  return output;
  
}
  
void drive(int Speed, int dir) { 
//  ```
//  speed -> Value between 0 ( no drive ) to 255 ( full speed )
//  dir -> Either 1 ( drive forward ) or -1 ( drive backward )
//  steer -> Value between -90 ( full steer left ) and 90 ( full steer right ), 0 is drive forward 
//  ```
  // Speed and Steering Control
  analogWrite(left_enable, Speed);
  analogWrite(right_enable, Speed);
  // Direction Control
  // Drive Forward
  if (dir == 1) {
    digitalWrite(left_for, HIGH);
    digitalWrite(left_back, LOW);
    digitalWrite(right_for, HIGH);
    digitalWrite(right_back, LOW);
  }
  // Drive Backward
  if (dir == -1){
    digitalWrite(left_for, LOW);
    digitalWrite(left_back, HIGH);
    digitalWrite(right_for, LOW);
    digitalWrite(right_back, HIGH);
  }
}
