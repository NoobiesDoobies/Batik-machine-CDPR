// Define stepper motor connections and steps per revolution:
#define dirPin1 9
#define stepPin1 8
#define dirPin2 7
#define stepPin2 6
#define dirPin3 5
#define stepPin3 4
#define dirPin4 3
#define stepPin4 2

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

char programStart;
bool parsing = false;
String sData, data[10];
bool isModified = false;

void setup() {
  // Declare pins as output:
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(dirPin3, OUTPUT);
  pinMode(stepPin4, OUTPUT);
  pinMode(dirPin4, OUTPUT);

  for (int i = 0; i < 100; i++) {
    X[i] = 150.0*cosf(i/100.0*2*PI)/fmaxf(fabs(cosf(i/100.0*2*PI)), fabs(sinf(i/100.0*2*PI))) + boxLength/2.0;
    Y[i] = 150.0*sinf(i/100.0*2*PI)/fmaxf(fabs(cosf(i/100.0*2*PI)), fabs(sinf(i/100.0*2*PI))) + boxWidth/2.0;
    Z[i] = 170.0;
  }
    
  initWiFi();
  initXYZ();
  initStepper();
  Serial.begin(9600);
  handleGetRoot();


  server.addHandler(&events);

  // Start server
  server.begin();
}

float trunc(float x) {
  if (x == 0.0) {
    return 0.0;
  }
  else {
    return fabs(x)*floor(fabs(x))/x;
  }
}

void loop() {
  while (Serial.available()) {
    programStart = Serial.read();
  }
  if (programStart == '1') { 
    // Inverse kinematics
    for (int i = 0; i < 100; i++) {
      // Wire length for given coordinates
      l1 = sqrt(pow(sqrt(pow(boxLength - (X[i] + effectorLength/2.0), 2.0) + pow(boxWidth - (Y[i] + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(Z[i], 2.0));
      l2 = sqrt(pow(sqrt(pow(X[i] - effectorLength/2.0, 2.0)               + pow(boxWidth - (Y[i] + effectorWidth/2.0), 2.0)) - vertexRadius, 2.0) + pow(Z[i], 2.0));
      l3 = sqrt(pow(sqrt(pow(X[i] - effectorLength/2.0, 2.0)               + pow(Y[i] - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(Z[i], 2.0));
      l4 = sqrt(pow(sqrt(pow(boxLength - (X[i] + effectorLength/2.0), 2.0) + pow(Y[i] - effectorWidth/2.0, 2.0)) - vertexRadius, 2.0)              + pow(Z[i], 2.0));
  
      // Step size needed for the new wire length
      step1 = -(int) trunc((l1 - l1Prev)/stepAmount);
      step2 = -(int) trunc((l2 - l2Prev)/stepAmount);
      step3 = -(int) trunc((l3 - l3Prev)/stepAmount);
      step4 = -(int) trunc((l4 - l4Prev)/stepAmount);
  
      // Run the stepper
      gerakStepper(step1, step2, step3, step4);
  
      // Set previous wire length to the current length
      l1Prev = l1;
      l2Prev = l2;
      l3Prev = l3;
      l4Prev = l4;
    }
  }
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
