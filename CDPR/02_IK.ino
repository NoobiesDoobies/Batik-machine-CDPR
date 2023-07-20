struct Point {
  float x;
  float y;
  float z;
};
const int n = 4;

struct Point points[n];
float decelerationFactor[n-2];

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
float l1 = 0.0;
float l2 = 0.0;
float l3 = 0.0;
float l4 = 0.0;
int steps[n][4];

float l1Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));
float l2Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));
float l3Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));
float l4Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(zBias, 2.0));


float currentPosition[3] = {boxLength/2.0, boxWidth/2.0, zBias};

float getAngleBetweenPoints(struct Point point1, struct Point point2, struct Point point3){
    float a1 = point3.x - point2.x;
    float a2 = point3.y - point2.y;
    float a3 = point3.z - point2.z;
    float b1 = point1.x - point2.x;
    float b2 = point1.y - point2.y;
    float b3 = point1.z - point2.z;

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

float getDecelerationFactor(float angle){
  float factor = 1.0 - angle/M_PI;
  return factor;
}

void calculateDecelerationFactor(){
  decelerationFactor[0] = 0.5;
  decelerationFactor[n-1] = 0.5;

  for (int i = 1; i < n - 1; i++) {
    float angle = getAngleBetweenPoints(points[i-1], points[i], points[i+1]);
    decelerationFactor[i] = getDecelerationFactor(angle);
    // Serial.println(angle*180/M_PI);
    // Serial.println(String(i) + " " + String(angle*180/M_PI) + " " + String(n));
    // Serial.println(String(i) + " " + String(decelerationFactor[i]) );

  }
}

int* getStepForEachMotor(struct Point point){
    // Wire length for given coordinates
    l1 = sqrt(pow(sqrt(pow(boxLength - (point.x + effectorLength/2.0), 2.0) + pow(boxWidth - (point.y + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(point.z, 2.0));
    l2 = sqrt(pow(sqrt(pow(point.x - effectorLength/2.0, 2.0)               + pow(boxWidth - (point.y + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(point.z, 2.0));
    l3 = sqrt(pow(sqrt(pow(point.x - effectorLength/2.0, 2.0)               + pow(point.y - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(point.z, 2.0));
    l4 = sqrt(pow(sqrt(pow(boxLength - (point.x + effectorLength/2.0), 2.0) + pow(point.y - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(point.z, 2.0));
    
    // Set previous wire length to the current length
    l1Prev = l1;
    l2Prev = l2;
    l3Prev = l3;
    l4Prev = l4;

    // Step size needed for the new wire length
    int tempSteps[4];
    tempSteps[0] = -(int) truncate((l1 - l1Prev)/stepAmount);
    tempSteps[1] = -(int) truncate((l2 - l2Prev)/stepAmount);
    tempSteps[2] = -(int) truncate((l3 - l3Prev)/stepAmount);
    tempSteps[3] = -(int) truncate((l4 - l4Prev)/stepAmount);

    return tempSteps;
}

void calculateStepsForEachPoint(){
  for(int i = 0; i < n; i++){
    *steps[i] = getStepForEachMotor(points[i]);
  }
}

void initFirstPoint(){
  points[0].x = 0.0;
  points[0].y = 0.0;
  points[0].z = 0.0;
}

void initRectanglePath(float a, float b){
  for (int i = 0; i < n; i++) {
    points[i].x = a*cosf(i/(float)n*2*PI)/fmaxf(fabs(cosf(i/(float)n*2*PI)), fabs(sinf(i/(float)n*2*PI))) + boxLength/2.0;
    points[i].y = b*sinf(i/(float)n*2*PI)/fmaxf(fabs(cosf(i/(float)n*2*PI)), fabs(sinf(i/(float)n*2*PI))) + boxWidth/2.0;
    points[i].z = zBias;
  }
}

void initCirclePath(float R){
  for (int i = 0; i < n; i++) {
    points[i].x = R*cosf(i/(float)n*2*PI) + boxLength/2.0;
    points[i].y = R*sinf(i/(float)n*2*PI) + boxWidth/2.0;
    points[i].z = zBias;
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
    float x = R * cosf(petalCount * angle) * cosf(petalCount * angle) + boxLength/2.0;
    float y = R * sinf(angle) * cosf(petalCount * angle) + boxWidth/2.0;
    points[i].x = x;
    points[i].y = y;
    points[i].z = zBias;
  }
}

void initEightPath(float R) {
  for (int i = 0; i < n; i++) {
    float angle = i / static_cast<float>(n) * 2 * M_PI;
    float x = R * 1.5 * cosf(angle) + boxLength / 2.0;
    float y = R * sinf(2 * angle) / 1.5 + boxWidth / 2.0;
    points[i].x = x;
    points[i].y = y;
    points[i].z = zBias;
  }
}

float truncate(float x) {
  if (x == 0.0) {
    return 0.0;
  }
  else {
    return fabs(x)*floor(fabs(x))/x;
  }
}

