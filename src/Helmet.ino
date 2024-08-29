#include <Arduino.h>
#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <SPI.h>


#define buttonPin 5
#define leftEye 26
#define rightEye 25

#define leftServoPin 19 // left from the front
#define rightServoPin  18 // right from the front

Servo leftServo; // closed at 180
Servo rightServo; // closed at 0

int leftServoOpen = 60;
int rightServoOpen = 180;

int leftServoClose = 180;
int rightServoClose = 0; 

int spammProtection = 0;

bool helmetOpen = true;


// Initialize Helmet
void setup() {

  delay(500);
  
  Serial.begin(115200);


  // initialize Pins
  // Button
  pinMode(buttonPin, INPUT_PULLUP);
  // Leds
  pinMode(leftEye, OUTPUT);
  pinMode(rightEye, OUTPUT);

  // Helmet boots open so eyelights are off
  analogWrite(leftEye, LOW); // LOW == 0, MAX == 255, HIGH == ?
  analogWrite(rightEye, LOW);

  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

	leftServo.setPeriodHertz(50);    // standard 50 hz servo
  rightServo.setPeriodHertz(50);

  leftServo.attach(leftServoPin, 500, 2400); // attaches the servo on leftServoPin to the servo object
  rightServo.attach(rightServoPin, 500, 2400);

  // Open visor 
  leftServo.write(leftServoOpen);
  rightServo.write(rightServoOpen);

  delay(200);
}

// Run Helmet
void loop() {


  int buttonState = digitalRead(buttonPin);

  Serial.println(buttonState);

  // Check if button is pressed and spamm protection is off
  // otherwise reduce spamm protection (if on)
  if(spammProtection == 0 && buttonState == LOW){

    spammProtection = 5; 

    // open or close the visor based on its current state
    if(helmetOpen){
      closeVisor();
    } else if(!helmetOpen){
      openVisor();
    }

  } else { 
    if(spammProtection > 0){
      spammProtection--;
    }
  }

  delay(200);
}


// open the visor and shut off eye lights
void openVisor() {
  leftServo.write(leftServoOpen);
  rightServo.write(rightServoOpen);
  
  analogWrite(leftEye, LOW);
  analogWrite(rightEye, LOW);
  
  helmetOpen = true;
  print();
}

// close the visor and turn on eye lights
void closeVisor() {
  leftServo.write(leftServoClose);
  rightServo.write(rightServoClose);

  delay(250); // turn on the light once the visor is closed 
  //TODO: FIND perfekt timing

  analogWrite(leftEye, 255);
  analogWrite(rightEye, 255);

  helmetOpen = false;
  print();
}

void print(){

  if(helmetOpen){
    Serial.println("Open");
  } else {
    Serial.println("Close");
  }

  Serial.print("leftServo:");
  Serial.println(leftServo.read());
  Serial.print("rightServo:");
  Serial.println(rightServo.read());
}