#include <Arduino.h>
#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <SPI.h>


#define visorButton 5
#define eyeButton 15
#define leftEye 25
#define rightEye 26

#define leftServoPin 19 // left from the front
#define rightServoPin 18 // right from the front

Servo leftServo; // closed at 180
Servo rightServo; // closed at 0

int leftServoOpen = 60;
int rightServoOpen = 180;

int leftServoClose = 180;
int rightServoClose = 0; 

int visorSpamm = 0;
int eyeSpamm = 0;

int brightness = 255; // LOW == 0, MAX == 255, HIGH == ?

bool visorOpen = true;
bool eyesOn = false;


// Initialize Helmet
void setup() {

  delay(500);
  
  Serial.begin(115200);


  // initialize Pins
  // Button
  pinMode(visorButton, INPUT_PULLUP);
  pinMode(eyeButton, INPUT_PULLUP);
  // Leds
  pinMode(leftEye, OUTPUT);
  pinMode(rightEye, OUTPUT);

  // Helmet boots open so eyelights are off
  shutEyesOff();

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

  int visorState = digitalRead(visorButton);
  int eyeState = digitalRead(eyeButton);

  Serial.print(visorState);
  Serial.println(eyeState);

  // Check if button is pressed and spamm protection is off
  // otherwise reduce spamm protection (if on)
  if(visorSpamm == 0 && visorState == LOW){
      // open or close the visor based on its current state
      if(visorOpen){
        closeVisor();
      } else if(!visorOpen){
        openVisor();
      }
    visorSpamm = 5;

  } else { 
    if(visorSpamm > 0){
      visorSpamm--;
    }
  }

  // same thing but for the eyes
  if(eyeSpamm == 0 && eyeState == LOW){
    // Toggle LED Eyes based on their current state
      if(eyesOn){
        shutEyesOff();
      } else if(!eyesOn){
        turnEyesOn();
      }
      eyeSpamm = 5;
  } else {
    if(eyeSpamm > 0){
      eyeSpamm--;
    }
  }

  delay(200);
}


// open the visor and shut off eye lights
void openVisor() {
  leftServo.attach(leftServoPin, 500, 2400);
  rightServo.attach(rightServoPin, 500, 2400);

  shutEyesOff();

  leftServo.write(leftServoOpen);
  rightServo.write(rightServoOpen);
  
  visorOpen = true;
  print();
}

// close the visor and turn on eye lights
void closeVisor() {
  leftServo.write(leftServoClose);
  rightServo.write(rightServoClose);

  visorOpen = false;

  delay(250); // turn on the light once the visor is closed 
  //TODO: FIND perfekt timing

  turnEyesOn();

  leftServo.detach();
  rightServo.detach();

  print();
}

void turnEyesOn(){
  // Only turn on Eyes if Visor is closed.
  if(!visorOpen){
    analogWrite(leftEye, brightness);
    analogWrite(rightEye, brightness);

    eyesOn = true;
  } else {
    Serial.println("Visor is open!");
  }// TODO Build else for user understanding
}

void shutEyesOff(){
  analogWrite(leftEye, LOW);
  analogWrite(rightEye, LOW);

  eyesOn = false;
}

void setBrightness(int newBrightness){
  brightness = newBrightness;
}

void print(){

  if(visorOpen){
    Serial.println("Open");
  } else {
    Serial.println("Close");
  }

  Serial.print("leftServo:");
  Serial.println(leftServo.read());
  Serial.print("rightServo:");
  Serial.println(rightServo.read());
}