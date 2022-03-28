#include <Arduino_LSM6DS3.h>

float x, y, z;

//PID setup
double Kp;     // gain values which need to be tuned to get smooth motion
double Ki;
double Kd;
double setPoint = 0;     // some value between high (900) and low (50) i.e. 
unsigned long currTime, prevTime = 0;
double deltaT;
double currE, prevE = 0;
double iE, dE; // errors for integral an derivative values
int input_x;
double output;


void setup() {
  
if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Kp = 0.1;
  Ki = 0;
  Kd = 0.3;
}

void loop() {
  

  if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(x, y, z);
        input_x = x;
  }
  output = PID(input_x);
  Serial.println(output);
  delay(1);
}

double PID(int input){
  currTime = millis();
  deltaT = currTime - prevTime;

  currE = setPoint - input;
  iE = iE + currE * deltaT;
  dE = (currE - prevE)/deltaT;

  int output = (Kp*currE) + (Ki*iE) + (Kd*dE);

  prevE = currE;
  prevTime = currTime;

  return output;
  
}
