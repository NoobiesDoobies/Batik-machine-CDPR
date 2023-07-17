#include <digitalWriteFast.h>

const float maxSpeed = 1.0;
const int minDelayPerStep = 20; // us
const int maxDelayPerStep = 200;
// Define stepper motor connections and steps per revolution:
#define dirPin1 9
#define stepPin1 8
#define dirPin2 7
#define stepPin2 6
#define dirPin3 5
#define stepPin3 4
#define dirPin4 3
#define stepPin4 2

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

float calculateSpeed(int totalStep, int nthStep, float currentSpeed, float nextSpeed) {
  return pow(currentSpeed, 2.0)*(1 - smoothStepFunction(2*(float) nthStep/totalStep)) + pow(nextSpeed, 2.0)*smoothStepFunction(2*(float) nthStep/totalStep - 1.0);
}

int convertSpeedToDelayus(float speed){
  return (minDelayPerStep - maxDelayPerStep) * speed + maxDelayPerStep;
}

void step(int pin, float speed) {
  digitalWriteFast(pin, HIGH);
  delayMicroseconds(convertSpeedToDelayus(speed));
  digitalWriteFast(pin, LOW);
  delayMicroseconds(convertSpeedToDelayus(speed));
}

void gerakStepper(int step1, int step2, int step3, int step4, float currentSpeed, float nextSpeed) {
  // Maximum step
  int stepMax = max(max(abs(step1), abs(step2)), max(abs(step3), abs(step4)));

  // Set the spinning direction:
  digitalWriteFast(dirPin3, (step1 >= 0));
  digitalWriteFast(dirPin1, (step2 >= 0));
  digitalWriteFast(dirPin2, (step3 >= 0));
  digitalWriteFast(dirPin4, (step4 >= 0));

  // Step all the steppers simultaneously
  for (int i = 1; i <= stepMax; i++) {
    float speed = calculateSpeed(stepMax, i, currentSpeed, nextSpeed);
    // 1st stepper
    if (step1 != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(step1))) == 0) {
        step(stepPin3, speed);
      }
    }

    // 2nd stepper
    if (step2 != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(step2))) == 0) {
        step(stepPin1, speed);
      }
    }

    // 3rd stepper
    if (step3 != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(step3))) == 0) {
        step(stepPin2, speed);
      }
    }

    // 4th stepper
    if (step4 != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(step4))) == 0) {
        step(stepPin4, speed);
      }
    }
  }
}