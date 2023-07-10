#include <digitalWriteFast.h>
#include <MultiStepper.h>
#include <AccelStepper.h>

#define MAX_SPEED 2000

// Define stepper motor connections and steps per revolution:
#define dirPin1 9
#define stepPin1 8
#define dirPin2 7
#define stepPin2 6
#define dirPin3 5
#define stepPin3 4
#define dirPin4 3
#define stepPin4 2


AccelStepper stepper2(AccelStepper::DRIVER, 8, 9);
AccelStepper stepper3(AccelStepper::DRIVER, 6, 7);
AccelStepper stepper1(AccelStepper::DRIVER, 4, 5);
AccelStepper stepper4(AccelStepper::DRIVER, 2, 3);

// Stepper stepper1(stepsPerRevolution, dirPin1, stepPin1);
// Stepper stepper2(stepsPerRevolution, dirPin2, stepPin2);


MultiStepper steppers;

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

 stepper1.setMaxSpeed(MAX_SPEED);
 stepper2.setMaxSpeed(MAX_SPEED);
 stepper3.setMaxSpeed(MAX_SPEED);
 stepper4.setMaxSpeed(MAX_SPEED);

 stepper1.setAcceleration(4000);
 stepper2.setAcceleration(4000);
 stepper3.setAcceleration(4000);
 stepper4.setAcceleration(4000);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
  steppers.addStepper(stepper4);
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


void setStepperSpeedProportionally(int stepMax, int step1, int step2, int step3, int step4){
  int speed1 = (float)step1/(float)stepMax * MAX_SPEED;
  int speed2 = (float)step2/(float)stepMax * MAX_SPEED;
  int speed3 = (float)step3/(float)stepMax * MAX_SPEED;
  int speed4 = (float)step4/(float)stepMax * MAX_SPEED;
  stepper1.setSpeed(speed1);
  stepper2.setSpeed(speed2);
  stepper3.setSpeed(speed3);
  stepper4.setSpeed(speed4);
  Serial.println("Step1: " + String(step1) + "\tStep2: " + String(step2) + "\tStep3: " + String(step3) + "\tStep4: " + String(step4) + "\tSpeed1: "+ String(speed1) + "\tSpeed2: " + String(speed2) + "\tSpeed3: " + String(speed3) + "\tSpeed4: " + String(speed4));
  
}

void gerakStepper(int step1, int step2, int step3, int step4) {

  stepper1.moveTo(step1);
  stepper2.moveTo(step2);
  stepper3.moveTo(step3);
  stepper4.moveTo(step4);

//  int stepMax = max(max(abs(step1), abs(step2)), max(abs(step3), abs(step4)));
//  setStepperSpeedProportionally(stepMax, step1, step2, step3, step4);

  while(stepper1.distanceToGo() != 0 && stepper2.distanceToGo() != 0 && stepper3.distanceToGo() != 0  && stepper4.distanceToGo() != 0 )
  {
     stepper1.run();
     stepper2.run();
     stepper3.run();
     stepper4.run();

    //  Serial.println(String(stepper1.currentPosition()) + "\t" + String(stepper2.currentPosition()) + "\t" + String(stepper3.currentPosition()) + "\t" + String(stepper4.currentPosition()));
  }
  
}




  
  // Maximum step
//  int stepMax = max(max(abs(step1), abs(step2)), max(abs(step3), abs(step4)));
//
//  // Set the spinning direction:
//  digitalWriteFast(dirPin3, (step1 >= 0));
//  digitalWriteFast(dirPin1, (step2 >= 0));
//  digitalWriteFast(dirPin2, (step3 >= 0));
//  digitalWriteFast(dirPin4, (step4 >= 0));

  // Step all the steppers simultaneously
//  for (int i = 1; i <= stepMax; i++) {
//    // 1st stepper
//    if (step1 != 0) {
//      if ((int) floor(fmodf(i, (float) stepMax/fabs(step1))) == 0) {
//        smoothStep(stepPin3);
//      }
//    }
//
//    // 2nd stepper
//    if (step2 != 0) {
//      if ((int) floor(fmodf(i, (float) stepMax/fabs(step2))) == 0) {
//        smoothStep(stepPin1);
//      }
//    }
//
//    // 3rd stepper
//    if (step3 != 0) {
//      if ((int) floor(fmodf(i, (float) stepMax/fabs(step3))) == 0) {
//        smoothStep(stepPin2);
//      }
//    }
//
//    // 4th stepper
//    if (step4 != 0) {
//      if ((int) floor(fmodf(i, (float) stepMax/fabs(step4))) == 0) {
//        smoothStep(stepPin4);
//      }
//    }
//  }


  
