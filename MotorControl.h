#ifndef MC_H
#define MC_H

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include "pins_arduino.h"

#include <inttypes.h>

class MotorControl {
  public:
    MotorControl(int _left_enable, int _left_f, int _left_b, int _right_enable, int _right_f, int _right_b);
    void drive(int speed, int dir, double steer);
    void brake(int brake_time);

   private:
    int left_enable, left_f, left_b, right_enable, right_f, right_b;
};

#endif
