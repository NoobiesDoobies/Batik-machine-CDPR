struct Point {
  float x;
  float y;
  float z;
};

struct Point points[n];
float decelerationFactor[n];
float processedDecelerationFactor[n];
float angles[n];
float speeds[n];

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

// const float boxLength = 980.0;     // mm
// const float boxWidth = 480.0;      // mm
// const float boxHeight = 600.0;     // mm
// const float effectorLength = 0; // mm
// const float effectorWidth = 0;  // mm
// const float effectorHeight = 40.0; // mm
// const float vertexRadius = 60.0;   // mm
// const float zBias = 170.0;


// Setup constants
float l1 = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));
float l2 = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));
float l3 = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));
float l4 = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));
int steps[n][4];

float l1Prev = l1;
float l2Prev = l2;
float l3Prev = l3;
float l4Prev = l4;


Point lastPosition;



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


void printAngleForEachPoint(){
  for(int i = 0; i<n; i++){
    Serial.println(String(angles[i]));
  }
}

// void printProcessedAngleForEachPoint(){
//   for(int i = 0; i<n; i++){
//     Serial.println(String(processedAngles[i]));
//   }
// }

void calculateDecelerationFactor(){
  for (int i = 0; i < n; i++) {
    // decelerationFactor[i] = 1 - pow(angles[i]/M_PI, 2);
    decelerationFactor[i] = pow(angles[i]/M_PI, 2);
    // Serial.println(angle*180/M_PI);
    // Serial.println(String(i) + " " + String(angle*180/M_PI) + " " + String(n));
    // Serial.println(String(i) + " " + String(decelerationFactor[i]) );
  }
  // decelerationFactor[1] = 0.9;
  // decelerationFactor[2] = 0.9;
  // decelerationFactor[3] = 0.9;
  // decelerationFactor[4] = 0.9;


}


float truncate(float x) {
  if (x == 0.0) {
    return 0.0;
  }
  else {
    return fabs(x)*floor(fabs(x))/x;
  }
}


void getStepForEachMotor(struct Point point, int *stepSizes){
    // Wire length for given coordinates
    l1 = sqrt(pow(sqrt(pow(boxLength - (point.x + effectorLength/2.0), 2.0) + pow(boxWidth - (point.y + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(point.z, 2.0));
    l2 = sqrt(pow(sqrt(pow(point.x - effectorLength/2.0, 2.0)               + pow(boxWidth - (point.y + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(point.z, 2.0));
    l3 = sqrt(pow(sqrt(pow(point.x - effectorLength/2.0, 2.0)               + pow(point.y - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(point.z, 2.0));
    l4 = sqrt(pow(sqrt(pow(boxLength - (point.x + effectorLength/2.0), 2.0) + pow(point.y - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(point.z, 2.0));
    


    stepSizes[0] = -(int) truncate((l1 - l1Prev)/stepAmount);
    stepSizes[1] = -(int) truncate((l2 - l2Prev)/stepAmount);
    stepSizes[2] = -(int) truncate((l3 - l3Prev)/stepAmount);
    stepSizes[3] = -(int) truncate((l4 - l4Prev)/stepAmount);

    // Set previous wire length to the current length
      
    // Serial.println("(ls IN) \t" + String(l1Prev) + " " + String(l2Prev) + " " + String(l3Prev) + " " + String(l4Prev));
    l1Prev = l1;
    l2Prev = l2;
    l3Prev = l3;
    l4Prev = l4;

    // Serial.print("(COORDINATE)\t");
    // Serial.print("x: " + String(point.x) + "\ty: " + String(point.y));
    // Serial.println("(STEPS) " + String(stepSizes[0]) + "\t" + String(stepSizes[1]));
}

void updateLPrev(struct Point point){
  l1Prev = sqrt(pow(sqrt(pow(boxLength - (point.x + effectorLength/2.0), 2.0) + pow(boxWidth - (point.y + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(point.z, 2.0));
  l2Prev = sqrt(pow(sqrt(pow(point.x - effectorLength/2.0, 2.0)               + pow(boxWidth - (point.y + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(point.z, 2.0));
  l3Prev = sqrt(pow(sqrt(pow(point.x - effectorLength/2.0, 2.0)               + pow(point.y - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(point.z, 2.0));
  l4Prev = sqrt(pow(sqrt(pow(boxLength - (point.x + effectorLength/2.0), 2.0) + pow(point.y - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(point.z, 2.0));
}

void calculateStepsForEachPoint(){
  lastPosition.x = boxWidth/2.0;
  lastPosition.y = boxHeight/2.0;
  lastPosition.z = zBias;

  for(int i = 0; i < n; i++){
    getStepForEachMotor(points[i], steps[i]);
    // Serial.println(*steps[i]);
    // Serial.println("OUT FUNCTION " + String(steps[i][1]) + "\t");
  }
}


void printStepsForEachPoint(){
  for (int i = 0; i < n; i++) {
    Serial.print("(steps " + String(i) + ")\t");
    Serial.print(String(steps[i][0]));
    Serial.print(" ");
    Serial.print(String(steps[i][1]));
    Serial.print(" ");
    Serial.print(String(steps[i][2]));
    Serial.print(" ");
    Serial.println(String(steps[i][3]));
  }
}

void mergePathForFirstIteration(){
  updateLPrev(points[n-1]);
  getStepForEachMotor(points[0], steps[0]);
}


void smoothingAccUsingMovingAvg(const float* data, int dataSize, int windowSize, float* movingAverages) {
    const float avgData = average(data, dataSize);
    for (int i = 0; i < dataSize; i++) {
        double sum = 0;
        int count = 0;

        // Calculate the sum of elements within the window
        for (int j = i - windowSize / 2; j <= i + windowSize / 2; j++) {
            int index = (j + dataSize) % dataSize; // Handle wrapping around the array
            sum += data[index];
            count++;
        }

        // Calculate the average and store it in the result array
        double average = sum / count;
        movingAverages[i] = (average)*SMOOTHING_MULTIPLIER ;
    }
}






