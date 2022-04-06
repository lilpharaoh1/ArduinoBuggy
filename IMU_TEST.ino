#include <Arduino_LSM6DS3.h>
#include <BasicLinearAlgebra.h>
//#include <MatrixMath.h>
using namespace BLA;

#define ir_pin A0

float x, y, z;

//PID setup
double Kp;     // gain values which need to be tuned to get smooth motion
double Ki;
double Kd;
int setPoint = 0;     // some value between high (900) and low (50) i.e. 
unsigned long currTime, prevTime = 0;
double deltaT;
double currE, prevE;
double iE, dE; // errors for integral an derivative values
int input_x;
double output;

// KF variables
int ir_value;

//mtx_type state[2][1] = {90, 0};
//mtx_type P[2][2] = {0.1, 0, 0, 0.1};
//mtx_type C[2][2];

BLA::Matrix<2, 1> state = {90, 0};
BLA::Matrix<2, 2> P = {0.1, 0, 0, 0.1};
BLA::Matrix<2, 2> I2;



void setup() {
  
if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Kp = 10;
  Ki = 0;
  Kd = 3;

  pinMode(ir_pin, INPUT);

  I2.Fill(0); I2(0,0) = 1.0; I2(1,1) = 1;
}

void loop() {
  ir_value = analogRead(ir_pin);
  if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(x, y, z);
        input_x = x;
  }
  //Serial.println(ir_value);

  kf(ir_value, input_x);
  //Serial.println(state(0,0));
  //output = PID(kf_output);

  
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

void kf(int ir_value, double ang_vel) {
  double pose = ir2pose(ir_value);
  //Serial.println(pose);
  double cT = millis();
  double dt = cT - prevTime; 

  //predict step
  BLA::Matrix<2, 2> A = {1, dt*0.001, 0, 0};
  BLA::Matrix<2, 1> B = {0, ang_vel}; 

  BLA::Matrix<2, 1> pred_state = A*state + B;
  BLA::Matrix<2, 2> pred_p = A*P*(~A); // Add noise

  state = pred_state;
  P = pred_p;
  Serial.print("P : ");
  Serial.println(pred_p(1,1));

  // update step
//  BLA::Matrix<1, 2> H = {1, 0};
//  BLA::Matrix<1, 1> measured_pose = {measured_pose};
//  BLA::Matrix<1, 1> y_k = measured_pose - (H*pred_state);
//
//
//  auto S_k = H*pred_p*(~H); // Add Noise
//  auto S_k_inv = S_k;
//  Invert(S_k_inv);
//  BLA::Matrix<2, 1> K = pred_p*(~H)*S_k_inv;
//  Serial.print("K : ");
//  Serial.print(K(0,0));
//  Serial.println(K(1,0));
//
//  state = pred_state + K*y_k;
//  Serial.print("state : ");
//  Serial.println(state(0,0));
//  P = (I2 - (K*H))*pred_p;
}

double ir2pose(int ir_value) {
  double input = ir_value;
  double y = 0.02*(input - 500) + 90;
  return y;
  
}
