#include <SoftwareSerial.h>
// SoftwareSerial for serial communication with HM10 bluetooth module.
#include <ArduinoBlue.h>
// ArduinoBlue bluetooth library
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Servo.h>
// Library for servo motor

int BLUETOOTH_TX =2;
int BLUETOOTH_RX =3;


Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *Rmotor = AFMS.getMotor(3);
Adafruit_DCMotor *Lmotor = AFMS.getMotor(4);

Servo upDown;
Servo pinces;
SoftwareSerial softSerial(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(softSerial);


void forward() {
  Rmotor -> run(FORWARD);
  Lmotor -> run(FORWARD);
}

void backward() {
  Rmotor -> run(BACKWARD);
  Lmotor -> run(BACKWARD);
}

void Stop() {
  Rmotor -> run(RELEASE);
  Lmotor -> run(RELEASE);
}

void turnRight() {
  Rmotor -> run(BACKWARD);
  Lmotor -> run(FORWARD);
}
void turnLeft() {
  Lmotor -> run(BACKWARD);
  Rmotor -> run(FORWARD);
}
int button;
void robotControl() {

  int throttle = phone.getThrottle() -49;
  int steering = phone.getSteering() -49;
  int sliderID = phone.getSliderId();
  int sliderVal = phone.getSliderVal();
  int bouton = phone.getButton();


  if(bouton != -1) {
    button = bouton;
  }
  if (button == 2) {
    pinces.attach(9);
    pinces.write(90);
    delay(200);
    pinces.detach();
  }
  if (button == 3) {
    pinces.attach(9);
    pinces.write(0);
    delay(200);
    pinces.detach();
  }
  if (button == 4) {
    pinces.detach();
  }

  if (steering == 0 && throttle == 0) {
    Stop();
  }
  else if (steering > 0) {
    turnRight();
  }
  else if (steering < 0) {
    turnLeft();
  }
  else if (throttle < 0) {
    forward();
  }
  else if (throttle > 0 ) {
    backward();
  }

  if (sliderID == 1) {
    if (sliderVal >= 60) {
   
      upDown.write(180);
   
    }
    else if (sliderVal <= 40) {
     
      upDown.write(0);
   

    }
    else {
     
      upDown.write(90);
    
    }
  }

}

void setup() {
  AFMS.begin();
  delay(500);
  softSerial.begin(9600);
  Rmotor -> setSpeed(180);
  Lmotor -> setSpeed(180);

  pinces.attach(9);
  pinces.write(0);
  pinces.detach();

  upDown.attach(10);
  upDown.write(90);
 
}

void loop() {

  robotControl();
 
}
