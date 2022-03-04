import processing.net.*;
import controlP5.*;

Client myClient;
String data;
ControlP5 p5;
Button DriveButton;
Button StopButton;
int sonar_counter = 0;
Textarea myTextArea;
boolean obs_detected = true;
boolean brake = false;


void setup() {
  size(350, 200);
  background(200, 100, 200);
  p5 = new ControlP5(this);
  DriveButton = p5.addButton("Drive").setPosition(50, 40).setSize(100, 20);
  StopButton = p5.addButton("Stop").setPosition(200, 40).setSize(100, 20);
  myClient = new Client(this, "192.168.4.1", 907);

  myTextArea = p5.addTextarea("sonar_display")
                  .setPosition(50,100)
                  .setSize(250,60)
                  .setFont(createFont("arial",20))
                  .setLineHeight(14)
                  .setColor(color(128))
                  .setColorBackground(color(200,100));
                  //.setColorForeground(color(0,100));
}

void draw() {
  
  //Poke Arduino ( 119 )
  if (sonar_counter > 10) {
    myClient.write('w');
    sonar_counter = 0;  
  }
  else { 
    sonar_counter++;
  }
  
   //Processing data
  data = myClient.readString();//Until('\n');
  if (data != null) { 
    // 1 = Obstacle, 0 = No Obstacle, 9 = Brake
     if (int(data) == 1) {
       if (!obs_detected) {
         myTextArea.clear();
         myTextArea.setText("OBSTACLE DETECTED!");
         println("Sonar Msg: " + "OBSTACLE DETECTED!");
         obs_detected = true;
         brake = false;
       }
     }
     else if (int(data) == 0) {
       if (obs_detected) {
         myTextArea.clear();
         myTextArea.setText("NO OBSTACLE!");
         println("Sonar Msg: " + "NO OBSTACLE!");
         obs_detected = false;
         brake = false;
       }
     }
      else {
        if (!brake){
          myTextArea.clear();
          myTextArea.setText("BRAKE!");
          println("Sonar Msg: " + "BRAKE!");
          brake = true; 
          obs_detected = true;
       }
      }
  }
}

public void controlEvent(ControlEvent ev) {
  if (ev.getController().getName() == "Drive") {
    myClient.write('d'); // Send Drive ( 100 )
    //println("in drive...");  
} 
  else if (ev.getController().getName() == "Stop") {
    myClient.write('s'); // Send Stop ( 115 )
    //println("in stop...");  
}
  //println(ev.getController().getName());
}
