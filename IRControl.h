#ifndef IRCONTROL_H
#define IRCONTROL_H

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include "pins_arduino.h"

#include <inttypes.h>

class IRControl {
  public:
    IRControl(int input_pin);
    void read_value();
    int get_ir_value();

  private:
    int ir_value, IR_pin;
};

#endif
