#include <digitalWriteFast.h>


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
  // speed = constrain(speed, 0, 1);
  return (minDelayPerStep - maxDelayPerStep) * pow(speed, 1) + maxDelayPerStep;
}

void step(int pin, float speed) {
  // speed = constrain(speed, 0, 1);
  digitalWriteFast(pin, HIGH);
  // delayMicroseconds(200);
  delayMicroseconds(convertSpeedToDelayus(speed));
  digitalWriteFast(pin, LOW);
  // delayMicroseconds(200);
  delayMicroseconds(convertSpeedToDelayus(speed));
  // stepWithoutDelay(pin);
}

void stepWithoutDelay(int pin){
  digitalWriteFast(pin, HIGH);
  delayMicroseconds(10);
  digitalWriteFast(pin, LOW);
  delayMicroseconds(10);
}

void gerakStepper(int stepSizes[], float speedPrev=0.0, float speedCurr=1.0) {
  // Maximum step

  int forceCalibratedStepSizes[stepper_count];
  for(int i=0; i<stepper_count; i++){
    forceCalibratedStepSizes[i] = stepSizes[i] + compensateCounter[i];
  }

  // int stepMax = max(max(abs(stepSizes[0]), abs(stepSizes[1])), max(abs(stepSizes[2]), abs(stepSizes[3])));
  int stepMax = max(max(abs(forceCalibratedStepSizes[0]), abs(forceCalibratedStepSizes[1])), max(abs(forceCalibratedStepSizes[2]), abs(forceCalibratedStepSizes[3])));


  // Set the spinning direction:
  for(int i=0; i<stepper_count;i++){
    digitalWriteFast(dirPin[i], (forceCalibratedStepSizes[i] >= 0));
  }
  
  // Step all the steppers simultaneously
  for (int i = 1; i <= stepMax; i++) {
    float speed = interpolate(i, 1, stepMax, speedPrev, speedCurr);
    // Serial.println(String(speed));
    // 1st stepper
    if (forceCalibratedStepSizes[0] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(forceCalibratedStepSizes[0]))) == 0) {
        step(stepPin3, speed);
      }
    }

    // 2nd stepper
    if (forceCalibratedStepSizes[1] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(forceCalibratedStepSizes[1]))) == 0) {
        step(stepPin1, speed);
      }
    }

    // 3rd stepper
    if (forceCalibratedStepSizes[2] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(forceCalibratedStepSizes[2]))) == 0) {
        step(stepPin2, speed);
      }
    }

    // 4th stepper
    if (forceCalibratedStepSizes[3] != 0) {
      if ((int) floor(fmodf(i, (float) stepMax/fabs(forceCalibratedStepSizes[3]))) == 0) {
        step(stepPin4, speed);
      }
    }
  }

    // Serial.print(stepMax);
    // Serial.print(" ");
    // Serial.println(speed);
}



void autoCalibrateForce(){
  // for(int i=0; i<stepper_count; i++){
  //   // Menarik ketika longgar
  //   if(force[i] < avg_force + force_window/2){
  //       digitalWriteFast(dirPin[i], 1);
  //   }

  //       // Mengulur ketika force sensor mengukur lebih dari 0 gram 
  //   if(force[i] < avg_force - force_window/2){
  //       digitalWriteFast(dirPin[i], 0);
  //   }
  // }
    
  // // Menarik
  // for(int i=0; i<stepper_count; i++){
  //   if(force[i] < avg_force - force_window/2){
  //     int delta_force = abs(avg_force - force[i]);
  //     for(int j=0; j< (int) (KP_auto_calibrate*delta_force);j++){
  //       stepWithoutDelay(stepPin[i]);
  //     }
  //     compensateCounter[i]+=(int) (KP_auto_calibrate*delta_force);
  //   }
  // }

  // // Mengulur
  // for(int i=0; i<stepper_count; i++){
  //   if(force[i] > avg_force + force_window/2){
  //     int delta_force = abs(avg_force - force[i]);
  //     for(int j=0; j<(int) (KP_auto_calibrate*delta_force);j++){
  //       stepWithoutDelay(stepPin[i]);
  //     }
  //     compensateCounter[i]-=(int) (0.25*KP_auto_calibrate*delta_force);
  //   }
  // }

  for(int i=0; i<stepper_count; i++){
    // Menari ketika longgar
    if(force[i] < avg_force - force_window/2){
        int delta_force = abs(avg_force - force[i]);
        // compensateCounter[i] = (int) (KP_auto_calibrate*delta_force);
        compensateCounter[i]  = 10;
    }

      // Mengulur 
    if(force[i] > avg_force + force_window/2){
        int delta_force = abs(avg_force - force[i]);
        // compensateCounter[i] = -(int) (KP_auto_calibrate*delta_force);
        compensateCounter[i] = -10;
    }
  }

}

void printCompensateCounter(){
  for(int i=0; i<stepper_count; i++){
    Serial.print(String(compensateCounter[i]) + " ");
  }
}

void emptyCompensateCounter(){
  for(int i=0; i<stepper_count; i++){
    digitalWriteFast(dirPin[i], compensateCounter[i] < 0);
  }
  for(int i=0; i<stepper_count; i++){
    Serial.println("Emptying motor compensate counter " + String(i));
    for(int j=0; j<compensateCounter[i]; j++){
      stepWithoutDelay(stepPin[i]);  
    }
  }
}

void smoothStart(float processedDecelerationFactor[], int steps[][4], int n){
  float speed = 0.0;
  float acceleration = processedDecelerationFactor[n]/n;
  for(int i=1; i<=n; i++){
    // Serial.println("speed: " + String(speed));
    gerakStepper(steps[i], speed, speed+acceleration);
    speed+=acceleration;
  }
}