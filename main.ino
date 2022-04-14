#include <WiFiNINA.h>
#include "MotorControl.h"
#include "SR04.h"
#include "IRControl.h"

#define ir_pin A0

#define crossroads_pin A6

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
int readIndex = 0;              // the index of the sonar_currEnt reading
int total = 0;                  // the running total
int average = 0;                // the average

// Bronze Challenge
int dir = 1;  
int sonar_counter = 0; 
int wifi_counter = 0; 
double turn = 0;   
bool begin_counter = false;
int crossroads_counter = -1;
bool drive_msg = false;
bool obstacle = false;
char msg[4] = "";
int speed = 0;

// Silver Challenge
int dist = 15, tolerance = 2;
long sonar_dist = 20; // check if this is best?
int test_counter = 0;

// Silver 2
float sonar_currE = 0, sonar_prevE = 0;
unsigned long sonar_currTime = millis() + 1, sonar_prevTime = millis();
float ir_currE = 0, ir_prevE = 0;

unsigned long ir_currTime = millis() + 1, ir_prevTime = millis();

double Kp = 15, Ki = 0, Kd = 5, sonar_pid_mid = 135, ir_pid_mid = 400;
int sonar_pid_output = sonar_pid_mid;
int ir_pid_output = ir_pid_mid;

// Networking
char ssid[] = "GROUPZ777";
char pass[] = "GROUPZ777";
WiFiServer server(907);
IPAddress ip;

void calc_ir_pid(int ir_value) { 
  double ir_Kp = 15, ir_Kd = 5;
  ir_currE = ir_value;
  ir_currTime = millis();

  unsigned long ir_deltaT = ir_currTime - ir_prevTime;
  float dE = (ir_currE - ir_prevE)/ir_deltaT;

  ir_pid_output = (Kp*ir_currE) + (Kd*dE);

  ir_prevE = ir_currE;
  ir_prevTime = ir_currTime;
}

double calc_turn(int dir) { 
  int ir = ircontrol.get_ir_value();
  calc_ir_pid(ir); // approx. White  = 700, Black = 10300

  if (ir_pid_output <= 500) return -90;
  else if (ir_pid_output >= 10300) return 90; 

  float pid_store = ir_pid_output;

  ir = 0.02*(pid_store - 500) - 90; 

  if (dir == -1) ir *= -1;
  else if (dir == 0) ir = 0;
  
  return ir;

  if (dir == 1) 
    turn = (180.0/550.0)*ir - 90.0;
  else if (dir == -1)
    turn = -(180.0/550.0)*ir + 90.0;
  else turn = 0;

  return turn;
}

int calc_speed() {
  if (sonar_pid_output <= 30) return 200;
  else if (sonar_pid_output >= 800) return 200; 

  float pid_store = sonar_pid_output;
  int calc_speed;

  if (sonar_pid_output > sonar_pid_mid && sonar_pid_output < 800) { 
    calc_speed = (200 / (1000 - sonar_pid_mid))*(pid_store - sonar_pid_mid);
    return calc_speed;
  }
  else {
    calc_speed = (200 / (80 - sonar_pid_mid))*(pid_store - sonar_pid_mid);
    return calc_speed;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ir_pin, INPUT);
  // pinMode(crossroads_pin, INPUT);
  
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  };

  WiFi.beginAP(ssid, pass);
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  // 0.26m/s Full Speed

 // Line Following
 // white = 50
 // black = 700

  // Serial.println(ip);
  if (wifi_counter >= 200) {
    WiFiClient client = server.available();
    // Serial.println(client.connected());
    if (client.connected()) { 
      int data = client.read();
      if (data == 100) drive_msg = true;
      else if (data == 115) drive_msg = false;

      if (data == 119) {
        if (drive_msg) {
          if (speed >= 185) {
            char msg = '1';
            client.write(msg);
          }
          else if (speed >= 100 && speed < 185) {
            char msg = '2';
            client.write(msg);
          }
          else {
            char msg = '3';
            client.write(msg);
          }
        }
        else {
          char msg = '9';
          client.write(msg);
        }
        
      }

    }
    if (wifi_counter > 250) wifi_counter = 0;
  }
  else wifi_counter++;

  //IR Input
  ircontrol.read_value();

  //Signal Smoothing
  // total = total - readings[readIndex];
  // // read from the sensor:
  // readings[readIndex] = ircontrol.get_ir_value();
  // // add the reading to the total:
  // total = total + readings[readIndex];
  // // advance to the next position in the array:
  // readIndex = readIndex + 1;
  // // if we're at the end of the array...
  // if (readIndex >= numReadings) {
  //   // ...wrap around to the beginning:
  //   readIndex = 0;
  // }

  // // calculate the average:
  // double value = total / numReadings;

  // Serial.println(value);
  // Serial.print("Value :");
  // Serial.println(value);

  if (sonar_counter >= 100) {
    sonar_currE = sr04.Distance();
    sonar_currTime = millis();

    unsigned long sonar_deltaT = sonar_currTime - sonar_prevTime;
    float dE = (sonar_currE - sonar_prevE)/sonar_deltaT;

    sonar_pid_output = (Kp*sonar_currE) + (Kd*dE);

    sonar_prevE = sonar_currE;
    sonar_prevTime = sonar_currTime;
  }
  else { 
    sonar_counter++;
  }
//  if (analogRead(crossroads_pin) < 200) {
//    if (begin_counter < 0) begin_counter = 0;
//    else if (begin_counter >= 0 && begin_counter < 1000){ 
//      begin_counter++;
//      turn = calc_turn(dir);
//    }
//    else if (begin_counter >= 1000) {
//      turn = 0;
//    }
//  }
//  else {
//    turn = calc_turn(dir);
//  }

  if (sonar_pid_output <= 0);
  else if (sonar_pid_output >= sonar_pid_mid) dir = 1;
  else dir = -1;

  turn = calc_turn(dir);

  // if (analogRead(crossroads_pin) < 200) { 
  //   begin_counter = true;
  // }
  // if (begin_counter) {
  //   crossroads_counter++;
  //   if (crossroads_counter <= 100);
  //   else if (crossroads_counter > 1000) turn = 0;
  //   if (crossroads_counter > 500) {
  //     crossroads_counter = -1;
  //     begin_counter = false;
  //   }
  // }


  //  Serial.print("begin_counter: ");
  //  Serial.println(begin_counter);

  //calc_speed
  speed = calc_speed();

  if (drive_msg) {
    motor.drive(speed, dir, turn);
    if (sonar_dist == 0);
    else if (sonar_dist < 15) {
      obstacle = true; 
    }
    else {
      obstacle = false;
    }
  }
  else {
    motor.brake(10);
  }
}
