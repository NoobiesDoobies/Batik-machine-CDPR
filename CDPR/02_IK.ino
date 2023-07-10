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


// Setup constants
float l1 = 0.0;
float l2 = 0.0;
float l3 = 0.0;
float l4 = 0.0;
int step1;
int step2;
int step3;
int step4;
float l1Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(170.0, 2.0));
float l2Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(170.0, 2.0));
float l3Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(170.0, 2.0));
float l4Prev = sqrt(pow(sqrt(pow(boxLength/2.0 - effectorLength/2.0, 2.0) + pow(boxWidth/2.0 - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0) + pow(170.0, 2.0));



float X[100] = {0};
float Y[100] = {0};
float Z[100] = {0};

void initXYZ(){
  for (int i = 0; i < 100; i++) {
    X[i] = 150.0*cosf(i/100.0*2*PI)/fmaxf(fabs(cosf(i/100.0*2*PI)), fabs(sinf(i/100.0*2*PI))) + boxLength/2.0;
    Y[i] = 150.0*sinf(i/100.0*2*PI)/fmaxf(fabs(cosf(i/100.0*2*PI)), fabs(sinf(i/100.0*2*PI))) + boxWidth/2.0;
    Z[i] = 170.0;
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

void calculateIK(int i){
    // Wire length for given coordinates
    l1 = sqrt(pow(sqrt(pow(boxLength - (X[i] + effectorLength/2.0), 2.0) + pow(boxWidth - (Y[i] + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(Z[i], 2.0));
    l2 = sqrt(pow(sqrt(pow(X[i] - effectorLength/2.0, 2.0)               + pow(boxWidth - (Y[i] + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(Z[i], 2.0));
    l3 = sqrt(pow(sqrt(pow(X[i] - effectorLength/2.0, 2.0)               + pow(Y[i] - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(Z[i], 2.0));
    l4 = sqrt(pow(sqrt(pow(boxLength - (X[i] + effectorLength/2.0), 2.0) + pow(Y[i] - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(Z[i], 2.0));

    // Step size needed for the new wire length
    step1 = -(int) truncate((l1 - l1Prev)/stepAmount);
    step2 = -(int) truncate((l2 - l2Prev)/stepAmount);
    step3 = -(int) truncate((l3 - l3Prev)/stepAmount);
    step4 = -(int) truncate((l4 - l4Prev)/stepAmount);



    // Set previous wire length to the current length
    l1Prev = l1;
    l2Prev = l2;
    l3Prev = l3;
    l4Prev = l4;
}