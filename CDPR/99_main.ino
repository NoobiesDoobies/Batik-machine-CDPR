
char programStart;
bool parsing = false;
String sData, data[10];
bool isModified = false;

void setup() {
  Serial.begin(9600);
  // initWiFi();
}

void loop() {
  while (Serial.available()) {
    programStart = Serial.read();
  }
  
  struct Point points[n];
  float decelerationFactor[n];
  float processedDecelerationFactor[n];
  float angles[n];
  float speeds[n];
  int steps[n][4];

  // initCirclePath(points, 150.0);
  // initZeroPath(points);
  // initSpiralPath(points, 150.0, 0.00, 4);
  initFlowerPath(points, 150.0, 3);
  // initRectanglePath(points, 75.0,  75.0);
  // initEightPath(points, 150.0);

  initStepper();
  // initLoadCell();

  // handleGetRoot();
  // server.addHandler(&events);
  // Start server
  // server.begin();
  
  // printCoordinateForEachPoint(points);
  calculateStepsForEachPoint(points, steps);
  mergePathForFirstIteration(points, steps);

  calculateAngleForEachPoint(points, angles);

  calculateDecelerationFactor(decelerationFactor, angles);
  smoothingAccUsingMovingAvg(decelerationFactor, n, SMOOTHING_WINDOW_SIZE, processedDecelerationFactor);
  // smoothingAccUsingFilter(decelerationFactor, n, processedDecelerationFactor, 0.4);

  stretchAccUsingMapping(decelerationFactor, processedDecelerationFactor, n);

  // printAngleForEachPoint();
  // printStepsForEac hPoint();


  // printProcessedAngleForEachPoint();

  // integrate_array_trapezoidal(processedDecelerationFactor, n, 0.1, speeds);
  Serial.println("Sebelum\tSesudah\tAngle");
  for(int i = 0; i < n ; i++){
    Serial.println(String(decelerationFactor[i]) + "\t" + String(processedDecelerationFactor[i]) + "\t" + String(angles[i]) + "\t" + String((float)convertSpeedToDelayus(processedDecelerationFactor[i])/100.0));
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
      gerakStepper(tempSteps);
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
      gerakStepper(tempSteps);
      // Serial.print("Initial step\t");
      // Serial.println(tempSteps[0]);
      // Serial.println("Done initial step");
      
      int i = 1;
      Serial.flush();
      Serial.read();
      bool first = true;
      while(!Serial.available()){
        // autoCalibrateForce();
        if(first && i==1){
          smoothStart(processedDecelerationFactor, steps, 4);
          i=4;
          first = false;
        }
        else{
          gerakStepper(steps[i], processedDecelerationFactor[(i==0) ? n-1 : i-1], processedDecelerationFactor[i]);
        }


        // gerakStepper(steps[i], speeds[i]);

        lastPosition.x = points[i].x;
        lastPosition.y = points[i].y;
        lastPosition.z = points[i].z;

        // readLoadCell();
        // printLoadCellValue();
        // printCompensateCounter();
        // Serial.println();
        i++;
        if(i == n){
          mergePathForFirstIteration(points, steps);
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

      gerakStepper(tempSteps);

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




