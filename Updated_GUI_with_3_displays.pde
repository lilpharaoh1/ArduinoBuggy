import processing.net.*;
import controlP5.*;

Client myClient;
String data;
ControlP5 p5;
Button DriveButton;
Button StopButton;
int sonar_counter = 0;
int sonar_counter2 = 0;
Textarea myTextArea;
Textarea myTextArea2;
Textarea myTextArea3;
boolean obs_detected = true;
boolean brake = false;
String stringthing;


void setup() {
  size(350, 360);
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
                  
  myTextArea2 = p5.addTextarea("speed_display")
                  .setPosition(50,180)
                  .setSize(250,60)
                  .setFont(createFont("arial",20))
                  .setLineHeight(14)
                  .setColor(color(128))
                  .setColorBackground(color(200,100));
                  //.setColorForeground(color(0,100));
                  
  myTextArea3 = p5.addTextarea("PID_display")
                  .setPosition(50,260)
                  .setSize(250,80)
                  .setFont(createFont("arial",20))
                  .setLineHeight(25)
                  //.setLineSpacing(4)
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
  
  if(sonar_counter2 > 9) {
    myClient.write('u');
    sonar_counter2 = 0;
  }
  else {
    sonar_counter2++;
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
       
     else if (data.charAt(0) == 5) {
       stringthing = data.substring(1, 3);
       myTextArea2.clear();
       myTextArea2.setText(stringthing); 
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
  myTextArea3.clear();
  myTextArea3.setText("KP = 7 \nKI = 4 \nKD = 5");
  
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
