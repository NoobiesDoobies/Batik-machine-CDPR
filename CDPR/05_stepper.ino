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

float calculateSpeed(int totalStep, int nthStep, float currentDecelerationFactor, float nextDecelarationFactor) {
  return pow(currentDecelerationFactor, 2.0)*(1 - smoothStepFunction(2*(float) nthStep/totalStep)) + pow(nextDecelarationFactor, 2.0)*smoothStepFunction(2*(float) nthStep/totalStep - 1.0);
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

void gerakStepper(int steps[], float currentDecelerationFactor, float nextDecelarationFactor) {
  // Maximum step
  int stepMax = max(max(abs(steps[0]), abs(steps[1])), max(abs(steps[2]), abs(steps[3])));

  // Set the spinning direction:
  digitalWriteFast(dirPin3, (steps[0] >= 0));
  digitalWriteFast(dirPin1, (steps[1] >= 0));
  digitalWriteFast(dirPin2, (steps[2] >= 0));
  digitalWriteFast(dirPin4, (steps[3] >= 0));

  // Step all the steppers simultaneously
  for (int i = 1; i <= stepMax; i++) {
    float speed = calculateSpeed(stepMax, i, currentDecelerationFactor, nextDecelarationFactor);
    // 1st stepper
    if (steps[0] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(steps[0]))) == 0) {
        step(stepPin3, speed);
      }
    }

    // 2nd stepper
    if (steps[1] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(steps[1]))) == 0) {
        step(stepPin1, speed);
      }
    }

    // 3rd stepper
    if (steps[2] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(steps[2]))) == 0) {
        step(stepPin2, speed);
      }
    }

    // 4th stepper
    if (steps[3] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(steps[3]))) == 0) {
        step(stepPin4, speed);
      }
    }
  }
}