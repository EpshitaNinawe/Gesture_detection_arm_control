#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <ESP32Servo.h>

//back left
#define in1 18
#define in2 4
#define en1 19

//back right
#define in3 21
#define in4 22
#define en2 23

//servo
Servo myservo;
#define an1 16

//void setMotors(int, int, int, int);

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
  int speed = 255;

  if (GamePad.isUpPressed()) {
    Serial.println("Up Pressed");
    setMotors(in1, in2, en1, speed);
    setMotors(in3, in4, en2, speed);
  }

  if (GamePad.isDownPressed()) {
    Serial.println("Down Pressed");
    setMotors(in1, in2, en1, -speed);
    setMotors(in3, in4, en2, -speed);
  }

  if (GamePad.isLeftPressed()) {
    Serial.println("Left Pressed");
    setMotors(in1, in2, en1, -speed);
    setMotors(in3, in4, en2, speed);
  }

  if (GamePad.isRightPressed()) {
    Serial.println("Right Pressed");
    setMotors(in1, in2, en1, speed);
    setMotors(in3, in4, en2, -speed);
  }

  if (GamePad.isCirclePressed()) {
    Serial.println("Circle Pressed");
    myservo.write(90);
  }

  if (GamePad.isCrossPressed()) {
    Serial.println("Cross pressed");
    myservo.write(-90);
  }

  if (GamePad.isTrianglePressed()) {
    Serial.println("stop");
    setMotors(in1, in2, en1, 0);
    setMotors(in3, in4, en2, 0);
  }
}