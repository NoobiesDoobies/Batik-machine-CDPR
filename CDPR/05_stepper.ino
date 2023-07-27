#include <digitalWriteFast.h>

const int stepper_count = 4;
const float maxSpeed = 1.0;
const int minDelayPerStep = 100; // us
const int maxDelayPerStep = 250;
// Define stepper motor connections and steps per revolution:
const int dirPin1 = 9;
const int stepPin1 = 8;
const int dirPin2 = 7;
const int stepPin2 = 6;
const int dirPin3 = 5;
const int stepPin3 = 4;
const int dirPin4 = 3;
const int stepPin4 = 2;

const int dirPin[4] = {dirPin3, dirPin1, dirPin2, dirPin4};
const int stepPin[4] = {stepPin3, stepPin1, stepPin2, stepPin4};


const int total_compensate_step = 1;

float force[4] = {0,0,0,0};
int compensateCounter[4] = {0,0,0,0};

void initStepper(){
    // Declare pins as output:
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(dirPin3, OUTPUT);
  pinMode(stepPin4, OUTPUT);
  pinMode(dirPin4, OUTPUT);
}

float smoothStepFunction(float x) {
  if (x < 0) {
    return 0;
  }
  else if (x > 1) {
    return 1;
  }
  else {
    return x*x*(3 - 2*x);
  }
}



int convertSpeedToDelayus(float speed){
  speed = constrain(speed, 0, 1);
  return (minDelayPerStep - maxDelayPerStep) * speed + maxDelayPerStep;
}

void step(int pin, float speed) {
  speed = constrain(speed, 0, 1);
  digitalWriteFast(pin, HIGH);
  delayMicroseconds(convertSpeedToDelayus(speed));
  digitalWriteFast(pin, LOW);
  delayMicroseconds(convertSpeedToDelayus(speed));
}

void stepWithoutDelay(int pin){
  digitalWriteFast(pin, HIGH);
  delayMicroseconds(10);
  digitalWriteFast(pin, LOW);
  delayMicroseconds(10);
}

void gerakStepper(int stepSizes[], float speed) {
  // Maximum step
  int stepMax = max(max(abs(stepSizes[0]), abs(stepSizes[1])), max(abs(stepSizes[2]), abs(stepSizes[3])));

  // Set the spinning direction:
  for(int i=0; i<stepper_count;i++){
    digitalWriteFast(dirPin[i], (stepSizes[i] >= 0));
  }
  
  // Step all the steppers simultaneously
  for (int i = 1; i <= stepMax; i++) {
    // 1st stepper
    if (stepSizes[0] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(stepSizes[0]))) == 0) {
        step(stepPin3, speed);
      }
    }

    // 2nd stepper
    if (stepSizes[1] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(stepSizes[1]))) == 0) {
        step(stepPin1, speed);
      }
    }

    // 3rd stepper
    if (stepSizes[2] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(stepSizes[2]))) == 0) {
        step(stepPin2, speed);
      }
    }

    // 4th stepper
    if (stepSizes[3] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(stepSizes[3]))) == 0) {
        step(stepPin4, speed);
      }
    }
  }

    // Serial.print(stepMax);
    // Serial.print(" ");
    // Serial.println(speed);
}



void autoCalibrateForce(){
  for(int i=0; i<stepper_count; i++){
    // Menarik ketika longgar
    if(force[i] < -50.0){
      // for(int j=0; j<total_compensate_step;j++){
      //   digitalWriteFast(dirPin[i], 1);
      //   stepWithoutDelay(stepPin[i]);
      // }
        digitalWriteFast(dirPin[i], 0);
        stepWithoutDelay(stepPin[i]);
      compensateCounter[i]+=total_compensate_step;
    }

    // Mengulur ketika force sensor mengukur lebih dari 0 gram 
    if((force[i] > 5.0) && (compensateCounter[i] > 0)){
      // for(int j=0; j<total_compensate_step;j++){
      //   digitalWriteFast(dirPin[i], 0);
      //    stepWithoutDelay(stepPin[i]);
      // }
        digitalWriteFast(dirPin[i], 0);
        stepWithoutDelay(stepPin[i]);
      compensateCounter[i]-=total_compensate_step;
    }
  }
}

void printCompensateCounter(){
  for(int i=0; i<stepper_count; i++){
    Serial.print(String(compensateCounter[i]) + " ");
  }
}