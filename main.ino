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
int readIndex = 0;              // the index of the current reading
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
char msg[5] = "";

// Silver Challenge
int dist = 15, tolerance = 2;
long sonar_dist = 20; // check if this is best?
int test_counter = 0;

// Networking
char ssid[] = "GROUPZ777";
char pass[] = "GROUPZ777";
WiFiServer server(907);
IPAddress ip;

double calc_turn(int dir) { 
    if (dir == 1) 
      turn = (180.0/700.0)*ircontrol.get_ir_value() - 90.0;
    else if (dir == -1)
      turn = -(180.0/700.0)*ircontrol.get_ir_value() + 90.0;
    else turn = 0;

    return turn;
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
          if (obstacle) {
            char msg = '1';
            client.write(msg);
          }
          else {
            char msg = '0';
            client.write(msg);
          }
        }
        else {
          char msg = '9';
          client.write(msg);
        }
        // //Serial.println("in here...");
        
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

    // Silver Challenge
  sonar_dist = sr04.Distance();

  // if (sonar_dist == 0);
  // else if (sonar_dist > dist + tolerance) {
  //   dir = 1;
  // }
  // else if (sonar_dist < dist - tolerance) {
  //   dir = -1;
  // }
  // else {
  //   dir = 0;
  }

//   sonar_counter = 0;
// }
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

  if (drive_msg) {
    if (sonar_dist == 0);
    else if (sonar_dist < 15) {
      obstacle = true; 
      motor.brake(10);
    }
    else {
      motor.drive(120, dir, turn);
      obstacle = false;
    }
  }
  else {
    motor.brake(10);
  }
}
