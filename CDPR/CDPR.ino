/*
  Code written by: Alfisena and Carlios
*/
#define PROGRAM_NAME "CDPR"

const int n = 100; // Jumlah titik
const int stepper_count = 4;
const float force_window = 0.0;
const float KP_auto_calibrate = 0.05;

const float e=2.71828;

const float SMOOTHING_MULTIPLIER = 1/5;
const int SMOOTHING_WINDOW_SIZE = n/20;

const float maxSpeed = 1.0;
const int minDelayPerStep = 10; // us
const int maxDelayPerStep = 300;

float average (float *array, int size) {
  float sum = 0L ;
  for (int i = 0 ; i < size ; i++){
    sum += array[i] ;
  }
  return  ((float) sum) / size ; 
}

