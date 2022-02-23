#include "MotorControl.h"

MotorControl::MotorControl(int _left_enable, int _left_f, int _left_b, int _right_enable, int _right_f, int _right_b) { 
  left_enable = _left_enable;
  left_f = _left_f;
  left_b = _left_b;

  right_enable = _right_enable;
  right_f = _right_f;
  right_b = _right_b;

  pinMode(left_enable, INPUT);
  pinMode(left_f, OUTPUT);
  pinMode(left_b, OUTPUT);
  pinMode(right_enable, INPUT);
  pinMode(right_f, OUTPUT);
  pinMode(right_b, OUTPUT);
}

void MotorControl::drive(int speed, int dir, double steer) { 
//  ```
//  speed -> Value between 0 ( no drive ) to 255 ( full speed )
//  dir -> Either 1 ( drive forward ) or -1 ( drive backward )
//  steer -> Value between -90 ( full steer left ) and 90 ( full steer right ), 0 is drive forward 
//  ```
  // Speed and Steering Control
  // drive forward
  if (steer == 0) {
    analogWrite(left_enable, speed);
    analogWrite(right_enable, speed);
  }
  // steer right
  else if (steer > 0 && steer <= 90){
    double turn_coeff = 1.0 - (steer / 90.0);
    analogWrite(left_enable, speed);
    analogWrite(right_enable, speed*turn_coeff);
  }
  // steer left
  else if (steer < 0 && steer >= -90) { 
    double turn_coeff = 1.0 + (steer / 90.0);
    analogWrite(left_enable, speed*turn_coeff);
    analogWrite(right_enable, speed);
  }
  
  // Direction Control
  // Drive Forward
  if (dir == 1) {
  digitalWrite(left_f, HIGH);
  digitalWrite(left_b, LOW);
  digitalWrite(right_f, HIGH);
  digitalWrite(right_b, LOW);
  }
  // Drive Backward
  else if (dir == -1) {
   digitalWrite(left_f, LOW);
  digitalWrite(left_b, HIGH);
  digitalWrite(right_f, LOW);
  digitalWrite(right_b, HIGH);
  }
  else if (dir ==  0) {
    brake(0);
  }
}

void MotorControl::brake(int brake_time) { 
  digitalWrite(left_f, LOW);
  digitalWrite(left_b, LOW);
  digitalWrite(right_f, LOW);
  digitalWrite(right_b, LOW);
  delay(brake_time);
}
