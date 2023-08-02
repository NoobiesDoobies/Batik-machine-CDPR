
char programStart;
bool parsing = false;
String sData, data[10];
bool isModified = false;

void setup() {
  Serial.begin(9600);

  // initWiFi();
  // initCirclePath(150.0);
  // initZeroPath();
  // initSpiralPath(150.0, 0.00, 4);
  // initFlowerPath(150.0, 5);
  initRectanglePath(75.0,  75.0);
  // initEightPath(150.0);

  initStepper();
  // initLoadCell();

  // handleGetRoot();
  // server.addHandler(&events);
  // Start server
  // server.begin();
  
  // printCoordinateForEachPoint();
  calculateStepsForEachPoint();
  mergePathForFirstIteration();

  calculateAngleForEachPoint();

  calculateDecelerationFactor();
  smoothingAccUsingMovingAvg(decelerationFactor, n, SMOOTHING_WINDOW_SIZE, processedDecelerationFactor);

  // printAngleForEachPoint();
  // printStepsForEac hPoint();


  // printProcessedAngleForEachPoint();

  Serial.println("Sebelum\tSesudah\tAngle");
  for(int i = 0; i < n ; i++){
    Serial.println(String(decelerationFactor[i]) + "\t" + String(processedDecelerationFactor[i]) + "\t" + String(angles[i]));
  }
}


void loop() {
  while (Serial.available()) {
    programStart = Serial.read();
  }
  switch(programStart){
    case '0':
    {
      int tempSteps[4];
      updateLPrev(lastPosition);
      // Serial.print(String(l1Prev) + " ");

      Point home = {boxLength/2.0, boxWidth/2.0, zBias};

      getStepForEachMotor(home, tempSteps);
      // Serial.print(String(l1) + " ");
      // Serial.print(String(l1Prev) + " ");
      // Serial.print(String(tempSteps[0]) + " ");
      // Serial.print(String(tempSteps[1]) + " ");
      // Serial.print(String(tempSteps[2]) + " ");
      // Serial.println(String(tempSteps[3]));
      gerakStepper(tempSteps, 0.5);
      emptyCompensateCounter();

      lastPosition = home;

      programStart = -999;

      

      break;
    }
    case '1':
    {   
      int tempSteps[4];
      Point home = {boxLength/2.0, boxWidth/2.0, zBias};
      updateLPrev(home);
      getStepForEachMotor(points[0], tempSteps);
      gerakStepper(tempSteps, 0.5);
      // Serial.print("Initial step\t");
      // Serial.println(tempSteps[0]);
      // Serial.println("Done initial step");
      
      int i = 1;
      Serial.flush();
      Serial.read();
      while(!Serial.available()){
        // autoCalibrateForce();

        gerakStepper(steps[i], processedDecelerationFactor[i]);

        lastPosition.x = points[i].x;
        lastPosition.y = points[i].y;
        lastPosition.z = points[i].z;

        // readLoadCell();
        // printLoadCellValue();
        // printCompensateCounter();
        // Serial.println();

        i++;
        if(i == n){
          mergePathForFirstIteration();
          i = 0;
          // programStart = 0;
          // break;

        }
      
        // Serial.println("x: " + String(lastPosition.x) + " y: " + String(lastPosition.y) + " z: " + lastPosition.z);
      }

      break;
    }
    case '2':
    {
      Serial.print("x: y: ");
      while(!Serial.available()){}
      int x = Serial.parseInt();
      int y = Serial.parseInt();
      Serial.read();

      Point point = {x,y,zBias};
      int tempSteps[4];
      getStepForEachMotor(point, tempSteps);
      Serial.println(String(x) + " " + String(y));

      gerakStepper(tempSteps, 0.5);

      lastPosition.x = x;
      lastPosition.y = y;
      lastPosition.z = zBias;

      programStart = -999;
      break;
    }

    default:{
      break;
    }

  }
 
  // Serial.println("x: " + String(lastPosition.x) + " y: " + String(lastPosition.y) + " z: " + lastPosition.z);
  // readLoadCell();
  // printLoadCellValue();
  // printCompensateCounter();

  // Serial.println();

}




