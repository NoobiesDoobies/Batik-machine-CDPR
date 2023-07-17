
char programStart;
bool parsing = false;
String sData, data[10];
bool isModified = false;

void setup() {
  initFirstPoint();
  // initWiFi();
  initCirclePath(150.0); // Radius
  initStepper();
  Serial.begin(9600);
  // handleGetRoot();
  initLoadCell();

  // server.addHandler(&events);

  // Start server
  // server.begin();
}


void loop() {
  while (Serial.available()) {
    programStart = Serial.read();
  }
  switch(programStart){
    case '0':
    {
      Point point = {boxLength/2.0, boxWidth/2.0, zBias};
      calculateIK(point);
      gerakStepper(step1, step2, step3, step4);
      currentPosition[0] = boxLength/2.0;
      currentPosition[1] = boxWidth/2.0;
      currentPosition[2] = zBias;
      break;
    }
    case '1':
    {   
      int i = 0;
      while(!Serial.available()){
        calculateIK(points[i]);
        currentPosition[0] = points[i].x;
        currentPosition[1] = points[i].y;
        currentPosition[2] = points[i].z;
        gerakStepper(step1, step2, step3, step4, speedFactor[i], speedFactor[i+1]);
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
      calculateIK(point);
      Serial.println(String(x) + " " + String(y));
      gerakStepper(step1, step2, step3, step4, 0.5, 0.5);
      programStart = -999;

      currentPosition[0] = x;
      currentPosition[1] = y;
      currentPosition[2] = zBias;
      break;
    }

    default:{
      break;
    }

  }
 
  // Serial.println("x: " + String(currentPosition[0]) + " y: " + String(currentPosition[1]) + " z: " + currentPosition[2]);
  readLoadCell();
}




