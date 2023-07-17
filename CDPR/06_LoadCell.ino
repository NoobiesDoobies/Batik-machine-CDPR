#include <HX711_ADC.h>
#include <EEPROM.h>

//pins:
const int HX711_dout = A0; //mcu > HX711 dout pin
const int HX711_sck = A1; //mcu > HX711 sck pin

const int calVal_eepromAdress = 0;
unsigned long t = 0;

HX711_ADC LoadCell(HX711_dout, HX711_sck);

void initLoadCell(){
  LoadCell.begin();

  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    float newCalibrationValue;
    EEPROM.get(calVal_eepromAdress, newCalibrationValue);
    LoadCell.setCalFactor(newCalibrationValue); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete");
  }
  while (!LoadCell.update());
}


void readLoadCell(){
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }
}