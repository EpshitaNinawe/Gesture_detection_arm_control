#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <ESP32Servo.h>

//back left
#define in1 4  
#define in2 8
#define en1 19

//back right
#define in3 21
#define in4 22
#define en2 23

//servo
Servo myservo;
#define an1 16

void setMotors(int inA, int inB, int pwm, int speed)
{
  if (speed > 0)
  {
    digitalWrite(inA, HIGH);
    digitalWrite(inB, LOW);
    analogWrite(pwm, speed);
  }
  else if (speed < 0)
  {
    digitalWrite(inA, LOW);
    digitalWrite(inB, HIGH);
    analogWrite(pwm, -speed);
  }
  else
  {
    digitalWrite(inA, LOW);
    digitalWrite(inB, LOW);
    analogWrite(pwm, 0);
  }
}



void setup() {
  Serial.begin(115200);
  Dabble.begin("Aashi");
  Serial.println("Dabble Ready!");

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(an1, OUTPUT);

  myservo.attach(an1);
}



void loop() {
  Dabble.processInput();

  int x_value = GamePad.getx_axis();
  int y_value = GamePad.gety_axis();
  int r_value = GamePad.getRadius();
  int a_value = GamePad.getAngle();

  int speed = map(r_value, 0, 7, 0, 255);

  if(a_value==0 && x_value==0 && y_value==0)
  {
    Serial.println("stop");
    setMotors(in1, in2, en1, 0);
    setMotors(in3, in4, en2, 0);
  }
  else if(a_value >= 45 && a_value <= 135)
  {
    Serial.println("Forward");
    setMotors(in1, in2, en1, -speed);
    setMotors(in3, in4, en2, -speed);
  }
  else if(a_value >= 135 && a_value <=225)
  {
    Serial.println("Left Pressed");
    setMotors(in1, in2, en1, -speed);
    setMotors(in3, in4, en2, speed);
  }
  else if(a_value >= 225 && a_value <= 315)
  {
    Serial.println("Backward");
    setMotors(in1, in2, en1, speed);
    setMotors(in3, in4, en2, speed);
  }
  else
  {
    Serial.println("Right");
    setMotors(in1, in2, en1, speed);
    setMotors(in3, in4, en2, -speed);
  } 

  if (GamePad.isCirclePressed()) {
    Serial.println("Circle Pressed"); 
    myservo.write(90);
    delay(20);
  }

  if (GamePad.isTrianglePressed()) {
    Serial.println("Triangle pressed");
    myservo.write(-90);
    delay(20);
  }
}
