#include "IRControl.h"

IRControl::IRControl(int input_pin) { 
  pinMode(input_pin, INPUT);
  IR_pin = input_pin;
}

void IRControl::read_value() { 
  ir_value = analogRead(IR_pin);
}

int IRControl::get_ir_value() { 
  return ir_value;
}
