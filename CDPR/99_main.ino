
char programStart;
bool parsing = false;
String sData, data[10];
bool isModified = false;

void setup() {
  // initWiFi();
  initFirstPoint();
  initCirclePath(150.0);
  // initFlowerPath(150.0, 3);
  // initRectanglePath(150.0, 150.0);

  initStepper();
  Serial.begin(9600);
  // handleGetRoot();
  initLoadCell();

  // server.addHandler(&events);

  // Start server
  // server.begin();
  calculateDecelerationFactor();
  calculateStepsForEachPoint();
}


void loop() {
  while (Serial.available()) {
    programStart = Serial.read();
  }
  switch(programStart){
    case '0':
    {
      Point point = {boxLength/2.0, boxWidth/2.0, zBias};

      int *tempSteps = getStepForEachMotor(point);
      gerakStepper(tempSteps, 0.5, 0.5);

      currentPosition[0] = boxLength/2.0;
      currentPosition[1] = boxWidth/2.0;
      currentPosition[2] = zBias;
      break;
    }
    case '1':
    {   
      int i = 0;
      while(!Serial.available()){
        currentPosition[0] = points[i].x;
        currentPosition[1] = points[i].y;
        currentPosition[2] = points[i].z;
        gerakStepper(steps[i], decelerationFactor[i], decelerationFactor[i+1]);
        i++;
        if(i == n){
          i = 0;
        }
        // Serial.println("x: " + String(currentPosition[0]) + " y: " + String(currentPosition[1]) + " z: " + currentPosition[2]);
        // readLoadCell();
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
      int *tempSteps = getStepForEachMotor(point);
      Serial.println(String(x) + " " + String(y));

      gerakStepper(tempSteps, 0.5, 0.5);

      currentPosition[0] = x;
      currentPosition[1] = y;
      currentPosition[2] = zBias;

      programStart = -999;
      break;
    }

    default:{
      break;
    }

  }
 
  // Serial.println("x: " + String(currentPosition[0]) + " y: " + String(currentPosition[1]) + " z: " + currentPosition[2]);
  // readLoadCell();
}




