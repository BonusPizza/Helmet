#include <Arduino.h>
#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <SPI.h>
#include <VoiceRecognitionV3.h>


#define visorButton 5
#define eyeButton 15

#define leftEye 25
#define rightEye 26
#define statusLed 27
#define lightSensor 33

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

bool visorOpen = true;
bool eyesOn = false;

int brightness = 255; // LOW == 0, MAX == 255, HIGH == ?

const long interval = 100;
unsigned long prevMillis = 0;
int blinksLeft = 0; // add 1 Blink == blinksleft += 2;

// define voice recognition stuff
VR jarvis(16,17);

// Initialize Helmet
void setup() {

  delay(400);
  
  Serial.begin(115200);

  // initialize Voice Recognition
  jarvis.begin(9600);

  // initialize Pins
  // Button
  pinMode(visorButton, INPUT_PULLUP);
  pinMode(eyeButton, INPUT_PULLUP);
  // Leds
  pinMode(leftEye, OUTPUT);
  pinMode(rightEye, OUTPUT);
  pinMode(statusLed, OUTPUT);
  // Sensor
  pinMode(lightSensor, INPUT);

  // Helmet boots open so eyelights are off
  shutEyesOff();
  digitalWrite(statusLed, HIGH);
  delay(100);
  digitalWrite(statusLed, LOW);

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

  int visorBS = digitalRead(visorButton);
  int eyeBS = digitalRead(eyeButton);

  // Check if button is pressed and spamm protection is off
  // otherwise reduce spamm protection (if on)
  if(visorSpamm == 0 && visorBS == LOW){
    // open or close the visor based on its current state
    if(visorOpen){
      closeVisor();
    } else if(!visorOpen){
      openVisor();
    }

    visorSpamm = 10;

  } else {
    if(visorSpamm > 0){
      visorSpamm--;
    }
  }

  // same thing but for the eyes
  if(eyeSpamm == 0 && eyeBS == LOW){
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

  if(eyesOn){
    setBrightness(map(analogRead(lightSensor), 0, 4095, 0, 255));
  }

  if(blinksLeft > 0){
    unsigned long currMillis = millis();
    if(currMillis - prevMillis >= interval){
      prevMillis = currMillis;
      digitalWrite(statusLed, !digitalRead(statusLed));
      blinksLeft--;
    }
  }

  delay(100);
}


// open the visor and shut off eye lights
void openVisor() {
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

  delay(250); // turn on the light once the visor is closed TODO: FIND perfekt timing
  
  turnEyesOn();
  print();
}

// turn eyes on
void turnEyesOn(){
  // Only turn on Eyes if Visor is closed.
  if(!visorOpen){
    analogWrite(leftEye, brightness);
    analogWrite(rightEye, brightness);

    eyesOn = true;
  } else {
    exceptionHandler(1);
  }
}

// shut eyes off
void shutEyesOff(){
  analogWrite(leftEye, LOW);
  analogWrite(rightEye, LOW);

  eyesOn = false;
}

// set new brightness of led eyes
void setBrightness(int newBrightness){
  brightness = newBrightness;
  if(eyesOn){
    analogWrite(leftEye, brightness);
    analogWrite(rightEye, brightness);
  }
}

// function to make the user understand what is going wrong
void exceptionHandler(int errorCode){
  switch (errorCode){
  case 1:
    Serial.println("The visor is open!");
    // 2 short blinks on statusLed
    blinksLeft = 4;
    break;
  default:
    break;
  }
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