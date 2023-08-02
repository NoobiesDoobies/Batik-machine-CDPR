/*
  Code written by: Alfisena and Carlios
*/
#define PROGRAM_NAME "CDPR"

const int n = 100;
const int stepper_count = 4;
const float force_window = 0.0;
const float KP_auto_calibrate = 0.05;


const float SMOOTHING_MULTIPLIER = 0.2;
const int SMOOTHING_WINDOW_SIZE = n/4;

float average (float *array, int size) {
  float sum = 0L ;
  for (int i = 0 ; i < size ; i++){
    sum += array[i] ;
  }
  return  ((float) sum) / size ; 
}