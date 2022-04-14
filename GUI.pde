import processing.net.*;
import controlP5.*;

Client myClient;
String data;
ControlP5 p5;
Button DriveButton;
Button StopButton;
int sonar_counter = 0;
Textarea myTextArea;
Textarea myTextArea2;
Textarea myTextArea3;



void setup() {
  size(350, 300);
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
                  
  myTextArea3 = p5.addTextarea("pid_display")
                  .setPosition(50,200)
                  .setSize(250,80)
                  .setFont(createFont("arial",20))
                  .setLineHeight(20)
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
    // 1 = Full Speed, 2 = Half Speed, 3 = Low Speed 9 = Brake
     if (int(data) == 1) {
       myTextArea.clear();
       myTextArea.setText("Full Speed!");
     }
     else if (int(data) == 2) {
       myTextArea.clear();
       myTextArea.setText("Half Speed!");
     }
     else if (int(data) == 3) {
       myTextArea.clear();
       myTextArea.setText("Low Speed!");
     }
     else {
        myTextArea.clear();
        myTextArea.setText("BRAKE!");
        println("Sonar Msg: " + "BRAKE!");
     }
  }
  
  myTextArea3.clear();
  myTextArea3.setText("KP = 5 \nKI = 0 \nKD = 15");
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
