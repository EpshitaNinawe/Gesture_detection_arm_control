#include <WiFi.h>
#include <ESP32Servo.h> 
#include <ESPAsyncWebServer.h>

/*---Pin Definitions---*/
#define IN3 21
#define IN4 22
#define enB 23
#define IN2 4
#define IN1 18
#define enA 19
#define servoPin 16
#define MotSpeed 255

short unsigned int control;
bool ispick = true;

const char* ssid = "Galaxy A15 5G 880E";
const char* password = "12345678";

Servo myservo;
AsyncWebServer server(80);

//function declarations
void forward();
void backward();
void left();
void right();
void stopBot();
void pick();
void keep();


void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  myservo.attach(servoPin);
  control = 0;
  Serial.begin(115200);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  /*Code to establish connection to wifi*/
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {  // Try for 20 seconds
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  //Code after wifi is connected 
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    //here we are defining the requests that make changes to control variable
    server.on("/forward", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Forward");
      control = 1;
      request->send(200, "text/plain", "Moving forward");
    });
    server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Left");
      control = 2;
      request->send(200, "text/plain", "Moving left");
    });
    server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Right");
      control = 3;
      request->send(200, "text/plain", "Moving right");
    });
    server.on("/back", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Backward");
      control = 4;
      request->send(200, "text/plain", "Moving backward");
    });
    server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Stopping");
      control = 5;
      request->send(200, "text/plain", "Stopping");
    });
    // server.on("/pick", HTTP_GET, [](AsyncWebServerRequest *request){
    //   Serial.println("Picking");
    //   control = 6;
    //   request->send(200, "text/plain", "Picking");
    // });
    // server.on("/keep", HTTP_GET, [](AsyncWebServerRequest *request){
    //   Serial.println("Keeping");
    //   control = 7;
    //   request->send(200, "text/plain", "Keeping");
    // });
    server.on("/pinch", HTTP_GET, [](AsyncWebServerRequest *request){
      if(ispick == true){
        ispick = false;
        control = 6;
        Serial.println("picked!");
        request->send(200, "text/plain", "Picked");
      }
      else{
        ispick = true;
        control = 7;
        Serial.println("kept!");
        request->send(200, "text/plain", "Kept");
      }
    });
    server.begin();
    Serial.println("Server started !!");
  }
  else
  {
    Serial.println("\nFailed to connect to WiFi");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
 
  switch (control) {
    case 1: forward(); delay(50); break;
    case 2: left(); delay(50); break;
    case 3: right(); delay(50); break;
    case 4: backward(); delay(50); break;
    case 5: stopBot(); delay(50); break;
    case 6: pick(); delay(50); break;
    case 7: keep(); delay(50); break;

    case 0://idle case 
    default: break;
  }
delay(10);
}


void forward() {
  //Serial.println("Forward in loop");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(enA, MotSpeed);
  analogWrite(enB, MotSpeed);
}

void backward() {
  //Serial.println("Backward in loop");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(enA, MotSpeed);
  analogWrite(enB, MotSpeed);
}

void right() {
  //Serial.println("Right in loop" );
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(enA, MotSpeed/1.3);
  analogWrite(enB, MotSpeed/1.3);
}

void left() {
  //Serial.println("Left in loop");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(enA, MotSpeed/1.3);
  analogWrite(enB, MotSpeed/1.3);
}

void stopBot() {
  //Serial.println("Stop in loop");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void pick(){
  myservo.write(60);
    delay(1000);
    control = 0;
}

void keep(){
  myservo.write(0);
    delay(1000);
    control = 0;
}