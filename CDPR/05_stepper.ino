#include <digitalWriteFast.h>

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


void smoothStep(int pin) {
  digitalWriteFast(pin, HIGH);
  delayMicroseconds(100);
  digitalWriteFast(pin, LOW);
  delayMicroseconds(100);
  //for (int i = 1; i <= 4; i++) {
  //  digitalWriteFast(pin, (1.0 - cosf(PI*i/2.0))/2.0);
  //}
}

void gerakStepper(int step1, int step2, int step3, int step4) {
  // Maximum step
  int stepMax = max(max(abs(step1), abs(step2)), max(abs(step3), abs(step4)));

  // Set the spinning direction:
  digitalWriteFast(dirPin3, (step1 >= 0));
  digitalWriteFast(dirPin1, (step2 >= 0));
  digitalWriteFast(dirPin2, (step3 >= 0));
  digitalWriteFast(dirPin4, (step4 >= 0));

  // Step all the steppers simultaneously
  for (int i = 1; i <= stepMax; i++) {
    // 1st stepper
    if (step1 != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(step1))) == 0) {
        smoothStep(stepPin3);
      }
    }

    // 2nd stepper
    if (step2 != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(step2))) == 0) {
        smoothStep(stepPin1);
      }
    }

    // 3rd stepper
    if (step3 != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(step3))) == 0) {
        smoothStep(stepPin2);
      }
    }

    // 4th stepper
    if (step4 != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(step4))) == 0) {
        smoothStep(stepPin4);
      }
    }
  }
}