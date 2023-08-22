#include <Vector.h>


struct Point {
  float x;
  float y;
  float z;
};
const int n = 100;

struct Point points[n];
float decelerationFactor[n];
float processedDecelerationFactor[n];
float angles[n];
float speeds[n];

typedef Vector<int> Elements;



const float stepsPerRevolution = 1600.0 ;
const float kelilingExtruder = PI*28.5 ; // mm
const float stepAmount = kelilingExtruder/stepsPerRevolution; // mm/step

// Setup constants
const float boxLength = 980.0;     // mm
const float boxWidth = 480.0;      // mm
const float boxHeight = 600.0;     // mm
const float effectorLength = 70.0; // mm
const float effectorWidth = 70.0;  // mm
const float effectorHeight = 40.0; // mm
const float vertexRadius = 60.0;   // mm
const float zBias = 170.0;


// Setup constants
float L[4] = {0,0 ,0 ,0};
int steps[n][4];

float l1Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));
float l2Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));
float l3Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));
float l4Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));


Point lastPosition;

const float SMOOTHING_MULTIPLIER = 1;
const int SMOOTHING_WINDOW_SIZE = 3;

float getAngleBetweenPoints(struct Point point1, struct Point point2, struct Point point3){
    float a1 = point3.x - point2.x;
    float a2 = point3.y - point2.y;
    float a3 = point3.z - point2.z;
    float b1 = point1.x - point2.x;
    float b2 = point1.y - point2.y;
    float b3 = point1.z - point2.z;

    if (abs(a1 - b1) < 1e-9 && abs(a2 - b2) < 1e-9 && abs(a3 - b3) < 1e-9) {
      return M_PI;
    }

    float dot = a1*b1 + a2*b2 + a3*b3;

    // Fungsi tan
    float cross = sqrt(pow(a1*b2 - b1*a2, 2.0) + pow(a2*b3 - a3*b2, 2.0) + pow(a3*b1 - a1*b3, 2.0));
    float angle = fabs(atan2(cross, dot));


    // Fungsi cos
    // float A2 = pow(a1, 2) + pow(a2, 2) + pow(a3,2);
    // float B2 = pow(b1, 2) + pow(b2, 2) + pow(b3,2);
    // float angle = fabs(acos(dot/sqrt(A2*B2)));

    return angle;
}

void calculateAngleForEachPoint(){
  angles[0] = getAngleBetweenPoints(points[n-1], points[0], points[1]);
  for(int i = 1; i < n-1; i++){
    angles[i] = getAngleBetweenPoints(points[i-1], points[i], points[i+1]);  
  }
  angles[n-1] = getAngleBetweenPoints(points[n-2], points[n-1], points[0]);
}


// void printAngleForEachPoint(){
//   for(int i = 0; i<n; i++){
//     Serial.println(String(angles[i]));
//   }
// }

// // void printProcessedAngleForEachPoint(){
// //   for(int i = 0; i<n; i++){
// //     Serial.println(String(processedAngles[i]));
// //   }
// // }

// void calculateDecelerationFactor(){
//   for (int i = 0; i < n; i++) {
//     // decelerationFactor[i] = 1 - pow(angles[i]/M_PI, 2);
//     decelerationFactor[i] = pow(angles[i]/M_PI, 2);
//     // Serial.println(angle*180/M_PI);
//     // Serial.println(String(i) + " " + String(angle*180/M_PI) + " " + String(n));
//     // Serial.println(String(i) + " " + String(decelerationFactor[i]) );
//   }
//   // decelerationFactor[1] = 0.9;
//   // decelerationFactor[2] = 0.9;
//   // decelerationFactor[3] = 0.9;
//   // decelerationFactor[4] = 0.9;


// }


float truncate(float x) {
  if (x == 0.0) {
    return 0.0;
  }
  else {
    return fabs(x)*floor(fabs(x))/x;
  }
}


void getStepForEachMotor(struct Point *point, int *stepSizes, float l1Preva, float l2Preva, float l3Preva, float l4Preva){
    // Wire length for given coordinates
    L[0] = sqrt(pow(sqrt(pow(boxLength - (point->x + effectorLength/2.0), 2.0) + pow(boxWidth - (point->y + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(point->z, 2.0));
    L[1] = sqrt(pow(sqrt(pow(point->x - effectorLength/2.0, 2.0)               + pow(boxWidth - (point->y + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(point->z, 2.0));
    L[2] = sqrt(pow(sqrt(pow(point->x - effectorLength/2.0, 2.0)               + pow(point->y - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(point->z, 2.0));
    L[3] = sqrt(pow(sqrt(pow(boxLength - (point->x + effectorLength/2.0), 2.0) + pow(point->y - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(point->z, 2.0));

    // stepSizes[0] = -(int) truncate((l1 - l1Prev)/stepAmount);
    // stepSizes[1] = -(int) truncate((l2 - l2Prev)/stepAmount);
    // stepSizes[2] = -(int) truncate((l3 - l3Prev)/stepAmount);
    // stepSizes[3] = -(int) truncate((l4 - l4Prev)/stepAmount);


    // Set previous wire length to the current length
      
    // Serial.println("(ls IN) \t" + String(l1Preva) + " " + String(l2Preva) + " " + String(l3Preva) + " " + String(l4Preva));
    // l1Prev = l1;
    // // l2Prev = l2;
    // l3Prev = l3;
    // l4Prev = l4;
    
    // Serial.print("(COORDINATE)\t");
    // Serial.print("x: " + String(point.x) + "\ty: " + String(point.y));
    // Serial.println("(STEPS) " + String(stepSizes[0]) + "\t" + String(stepSizes[1]));
}

// void updateLPrev(struct Point point){
//   l1Prev = sqrt(pow(sqrt(pow(boxLength - (point.x + effectorLength/2.0), 2.0) + pow(boxWidth - (point.y + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(point.z, 2.0));
//   l2Prev = sqrt(pow(sqrt(pow(point.x - effectorLength/2.0, 2.0)               + pow(boxWidth - (point.y + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(point.z, 2.0));
//   l3Prev = sqrt(pow(sqrt(pow(point.x - effectorLength/2.0, 2.0)               + pow(point.y - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(point.z, 2.0));
//   l4Prev = sqrt(pow(sqrt(pow(boxLength - (point.x + effectorLength/2.0), 2.0) + pow(point.y - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(point.z, 2.0));
// }

void calculateStepsForEachPoint(){
  lastPosition.x = boxWidth/2.0;
  lastPosition.y = boxHeight/2.0;
  lastPosition.z = zBias;


 getStepForEachMotor(&points[0], steps[0], l1Prev, l2Prev, l3Prev, l4Prev);
  // for(int i = 0; i < n; i++){
  //   getStepForEachMotor(&points[0], steps[i], l1Prev, l2Prev, l3Prev, l4Prev);
  //   // Serial.println(*steps[i]);
  //   // Serial.println("OUT FUNCTION " + String(steps[i][1]) + "\t");
  // }
}


// void printStepsForEachPoint(){
//   for (int i = 0; i < n; i++) {
//     Serial.print("(steps " + String(i) + ")\t");
//     Serial.print(String(steps[i][0]));
//     Serial.print(" ");
//     Serial.print(String(steps[i][1]));
//     Serial.print(" ");
//     Serial.print(String(steps[i][2]));
//     Serial.print(" ");
//     Serial.println(String(steps[i][3]));
//   }
// }

// void mergePathForFirstIteration(){
//   updateLPrev(points[n-1]);
//   getStepForEachMotor(points[0], steps[0]);
// }


// void smoothingAccUsingMovingAvg(const float* data, int dataSize, int windowSize, float* movingAverages) {
//     const float avgData = average(data);
//     for (int i = 0; i < dataSize; i++) {
//         double sum = 0;
//         int count = 0;

//         // Calculate the sum of elements within the window
//         for (int j = i - windowSize / 2; j <= i + windowSize / 2; j++) {
//             int index = (j + dataSize) % dataSize; // Handle wrapping around the array
//             sum += data[index];
//             count++;
//         }

//         // Calculate the average and store it in the result array
//         double average = sum / count;
//         movingAverages[i] = (average)*SMOOTHING_MULTIPLIER ;
//     }
// }

// float average (float *array) {
//   float sum = 0L ;
//   for (int i = 0 ; i < n ; i++){
//     sum += array [i] ;
//   }
//   return  ((float) sum) / n ; 
// }




#include <HX711_ADC.h>
#include <EEPROM.h>


const int calVal_eepromAdress1 = 0;
const int calVal_eepromAdress2 = 32;
const int calVal_eepromAdress3 = 64;
const int calVal_eepromAdress4 = 96;

unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long t3 = 0;
unsigned long t4 = 0;


HX711_ADC LoadCell1(A6, A7);
HX711_ADC LoadCell2(A4, A5);
HX711_ADC LoadCell3(A2, A3);
HX711_ADC LoadCell4(A0, A1);


void initRectanglePath(float a, float b){
  for (int i = 0; i < n; i++) {
    points[i].x = a*cosf(i/(float)n*2*PI)/fmaxf(fabs(cosf(i/(float)n*2*PI)), fabs(sinf(i/(float)n*2*PI))) + boxLength/2.0;
    points[i].y = b*sinf(i/(float)n*2*PI)/fmaxf(fabs(cosf(i/(float)n*2*PI)), fabs(sinf(i/(float)n*2*PI))) + boxWidth/2.0;
    points[i].z = zBias+200;
  }
}

void initCirclePath(float R){
  for (int i = 0; i < n; i++) {
    points[i].x = R*cosf(i/(float)n*2*PI) + boxLength/2.0;
    points[i].y = R*sinf(i/(float)n*2*PI) + boxWidth/2.0;
    points[i].z = zBias;

    // Serial.print("(COORDINATE)\t");
    // Serial.println("x: " + String(points[i].x) + "\ty: " + String(points[i].y));
  }
}

void initEllipsePath(float a, float b){
  for (int i = 0; i < n; i++) {
    float angle = i / static_cast<float>(n) * 2 * M_PI;
    float x = a * cosf(angle) + boxLength/2.0;
    float y = b * sinf(angle) + boxWidth/2.0;
    points[i].x = x;
    points[i].y = y;
    points[i].z = zBias;
  }
}

void initFlowerPath(float R, int petalCount){
  for (int i = 0; i < n; i++) {
    float angle = i / static_cast<float>(n) * 2 * M_PI;
    float x = R * cosf(angle) * cosf(petalCount * angle) + boxLength/2.0;
    float y = R * sinf(angle) * cosf(petalCount * angle) + boxWidth/2.0;
    points[i].x = x;
    points[i].y = y;
    points[i].z = zBias+200;
  }
}

void initEightPath(float R) {
  for (int i = 0; i < n; i++) {
    float angle = i / static_cast<float>(n) * 2 * M_PI;
    float x = R * 1.5 * cosf(angle) + boxLength / 2.0;
    float y = R * sinf(2 * angle) / 1.5 + boxWidth / 2.0;
    points[i].x = x;
    points[i].y = y;
    points[i].z = zBias+200;
  }
}

void printCoordinateForEachPoint(){
  for(int i = 0; i < n; i++){
    Serial.println(String(i) + " x: " + String(points[i].x) + " y: " + String(points[i].y) + " z: " + points[i].z);
  }
}


void initSpiralPath(float startRadius, float finalRadius, float numTurns) {
    float spacing = (startRadius - finalRadius) / (n - 1); // Calculate the spacing between each turn
    float deltaAngle = numTurns * 2 * PI / (n - 1); // Calculate the angle increment for each point

    for (int i = 0; i < n; i++) {
        float angle = deltaAngle * i; // Calculate the angle for each point along the spiral
        float radius = startRadius - spacing * i; // Decrease the radius with each turn
        points[i].x = radius * cosf(angle) + boxLength / 2.0;
        points[i].y = radius * sinf(angle) + boxWidth / 2.0;
        points[i].z = zBias;
    }
}

#include <digitalWriteFast.h>

const float maxSpeed = 1.0;
const int minDelayPerStep = 100; // us
const int maxDelayPerStep = 250;
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

void gerakStepper(int stepSizes[], float speed) {
  // Maximum step
  int stepMax = max(max(abs(stepSizes[0]), abs(stepSizes[1])), max(abs(stepSizes[2]), abs(stepSizes[3])));

  // Set the spinning direction:
  digitalWriteFast(dirPin3, (stepSizes[0] >= 0));
  digitalWriteFast(dirPin1, (stepSizes[1] >= 0));
  digitalWriteFast(dirPin2, (stepSizes[2] >= 0));
  digitalWriteFast(dirPin4, (stepSizes[3] >= 0));
  
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

char programStart;
bool parsing = false;
String sData, data[10];
bool isModified = false;
void setup() {
  // put your setup code here, to run once:
  initCirclePath(150.0);
    calculateStepsForEachPoint();
  // mergePathForFirstIteration();

  // calculateAngleForEachPoint();

  // calculateDecelerationFactor();
  // smoothingAccUsingMovingAvg(decelerationFactor, n, SMOOTHING_WINDOW_SIZE, processedDecelerationFactor);

}

void loop() {
  // put your main code here, to run repeatedly:

}
