
char programStart;
bool parsing = false;
String sData, data[10];
bool isModified = false;

void setup() {
    
//  initWiFi();
  initXYZ();
  initStepper();
  Serial.begin(9600);
//  handleGetRoot();


//  server.addHandler(&events);

  // Start server
//  server.begin();
}


void loop() {
  while (Serial.available()) {
    programStart = Serial.read();
  }
  if (programStart == '1') { 
    // Inverse kinematics
    for (int i = 0; i < n; i++) {
      calculateIK(i);
  
      // Run the stepper
      gerakStepper(step1, step2, step3, step4);
    }
  }
}
