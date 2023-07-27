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

const float default_force_value = 0.0;

void initLoadCell(){
  LoadCell1.begin();
  LoadCell2.begin();
  LoadCell3.begin();
  LoadCell4.begin();

  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell1.start(stabilizingtime, _tare);
  if (LoadCell1.getTareTimeoutFlag() || LoadCell1.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    float newCalibrationValue;
    EEPROM.get(calVal_eepromAdress1, newCalibrationValue);
    Serial.println("Cal value1: " + String(newCalibrationValue));
    LoadCell1.setCalFactor(newCalibrationValue); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete 1");
  }
  while (!LoadCell1.update());

  LoadCell2.start(stabilizingtime, _tare);
  if (LoadCell2.getTareTimeoutFlag() || LoadCell2.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    float newCalibrationValue;
    EEPROM.get(calVal_eepromAdress2, newCalibrationValue);
    Serial.println("Cal value2: " + String(newCalibrationValue));
    LoadCell2.setCalFactor(newCalibrationValue); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete 2");
  }
  while (!LoadCell2.update());


  LoadCell3.start(stabilizingtime, _tare);
  if (LoadCell3.getTareTimeoutFlag() || LoadCell3.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    float newCalibrationValue;
    EEPROM.get(calVal_eepromAdress3, newCalibrationValue);
    Serial.println("Cal value3: " + String(newCalibrationValue));
    LoadCell3.setCalFactor(newCalibrationValue); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete 3");
  }
  while (!LoadCell3.update());

  // -- 
  LoadCell4.start(stabilizingtime, _tare);
  if (LoadCell4.getTareTimeoutFlag() || LoadCell4.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    float newCalibrationValue;
    EEPROM.get(calVal_eepromAdress4, newCalibrationValue);
    Serial.println("Cal value4: " + String(newCalibrationValue));
    LoadCell4.setCalFactor(newCalibrationValue); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete4");
  }
  while (!LoadCell4.update());
}


void readLoadCell(){
  static boolean newDataReady1 = 0;
  static boolean newDataReady2 = 0;
  static boolean newDataReady3 = 0;
  static boolean newDataReady4 = 0;

  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell1.update()) newDataReady1 = true;
  if (LoadCell2.update()) newDataReady2 = true;
  if (LoadCell3.update()) newDataReady3 = true;
  if (LoadCell4.update()) newDataReady4 = true;


  // get smoothed value from the dataset:
  if (newDataReady1 && newDataReady2 && newDataReady3 && newDataReady4) {
    if (millis() > t1 + serialPrintInterval) {
      // Serial.print("Load_cell output val: ");
      float i = LoadCell1.getData();
      // Serial.print("(1)");
      force[0] = i;
      newDataReady1 = 0;
      t1 = millis();

      i = LoadCell2.getData();
      // Serial.print("(2)");
      force[1] = i;
      newDataReady2 = 0;
      t2 = millis();


      i = LoadCell3.getData();
      // Serial.print("(3)");
      force[2] = i;
      newDataReady3 = 0;
      t3 = millis();

      i = LoadCell4.getData();
      // Serial.print("(4)");
      force[3] = i;
      newDataReady4 = 0;
      t4 = millis();
    }
  }

  // // // Kalau force sensor gagal membaca pada iterasi tersebut, di-set ke sebuah value default
  // else{
    
  //   for(int i=0; i<stepper_count;i++){
  //     // Serial.print(String(force[i]) + " ");
  //     // force[i] = default_force_value;
  //   }
  // }

  
}


void printLoadCellValue(){
  for(int i=0; i<stepper_count;i++){
    Serial.print(String(force[i]) + " ");
  }
}