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
#define statusLed 27

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
  pinMode(statusLed, OUTPUT);

  // Helmet boots open so eyelights are off
  shutEyesOff();
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

  Serial.print(visorBS);
  Serial.println(eyeBS);

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
    if(visorBS == LOW){
      exceptionHandler(2);
    }
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
    if(eyeBS == LOW){
      exceptionHandler(3);
    }
    if(eyeSpamm > 0){
      eyeSpamm--;
    }
  }

  delay(100);
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
    exceptionHandler(1);
  }
}

void shutEyesOff(){
  analogWrite(leftEye, LOW);
  analogWrite(rightEye, LOW);

  eyesOn = false;
}

void setBrightness(int newBrightness){
  brightness = newBrightness;
}

// function to make the user understand what is going wrong
void exceptionHandler(int errorCode){
  switch (errorCode){
  case 1:
    Serial.println("The visor is open!");
    // 2 short blinks on statusLed
    digitalWrite(statusLed, HIGH);
    delay(100);
    digitalWrite(statusLed, LOW);
    delay(150);
    digitalWrite(statusLed, HIGH);
    delay(100);
    digitalWrite(statusLed, LOW);
    break;
  case 2:
    Serial.println("Visor Spamm Protection is on!");
    // 1 short blink
    digitalWrite(statusLed, HIGH);
    delay(100);
    digitalWrite(statusLed, LOW);
    break;
  case 3:
    Serial.println("Eye Spamm Protection is on!");
    // 1 long blink
    digitalWrite(statusLed, HIGH);
    delay(300);
    digitalWrite(statusLed, LOW);
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