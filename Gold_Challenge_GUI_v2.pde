import processing.net.*;
import controlP5.*;

ControlP5 cp5;
PFont font;
PFont font2;
PImage sprite;
ControlP5 p5;

float BuggyAngle;

Textarea myTextArea;

void setup() {
  fullScreen();
  //size(1000, 1000);
  p5 = new ControlP5(this);

  font = createFont("Times New Roman", 100);
  font2 = createFont("Times New Roman", 30);

  sprite = loadImage("Buggy_Sprite-removebg-preview.png");
  
  myTextArea = p5.addTextarea("angle_display")
                  .setPosition(width/2 - 200,height - 250)
                  .setSize(400,150)
                  .setFont(font2)
                  .setLineHeight(50)
                 // .textAlign(CENTER, CENTER)
                  .setColor(color(52))
                  .setColorBackground(color(200,100));
}

void draw() {
  
  background(30, 180, 120);
  
  textFont(font);
  text("Tilt Angle from Vertical", width/2 - 450, height - 300);
  BuggyAngle= 110;
  
  myTextArea.clear();
  String angle = str(BuggyAngle);
  myTextArea.setText("\n                        " + angle);
  
  line(width, height/2 + 32, 0, height/2 + 32);
  line(width/2, height/2 + 32, width/2, 0);
  //line(width/2, height/2 + 32, 0, 0);
  //line(width, 0, width/2, height/2 + 32);
  textFont(font2);
  text("-90 degrees", width/2 - 600, height/2 + 100);
  text("90 degrees", width/2 + 600, height/2 + 100);
  text("0 degrees", width/2 + 50, 180);
  pushMatrix();
  translate((width/2), (height/2) + 32);
  //rotate(radians(-90));
  rotate(radians(BuggyAngle - 90));
  image(sprite, - 134, -432);
  popMatrix();
  
}
