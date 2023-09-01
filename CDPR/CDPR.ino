/*
  Code written by: Alfisena, Naufan, and Carlios
*/
#define PROGRAM_NAME "CDPR"

const int n = 1000; // Jumlah titik
const int stepper_count = 4;
const float force_window = 0.0;
const float KP_auto_calibrate = 0.05;

const float e=2.71828;

const float SMOOTHING_MULTIPLIER = 1/5;
const int SMOOTHING_WINDOW_SIZE = max(2, n/20);

const float maxSpeed = 1.0;
const int minDelayPerStep = 250; // us
const int maxDelayPerStep = 500;

float average (float *array, int size) {
  float sum = 0L ;
  for (int i = 0 ; i < size ; i++){
    sum += array[i] ;
  }
  return  ((float) sum) / size ; 
}

float interpolate(float x, float x0, float x1, float y0, float y1) {
    // Check for division by zero to avoid errors
    if (x1 - x0 == 0.0f) {
        Serial.println("Error: Division by zero in interpolate function.");
        return 0.0f;
    }

    // Perform linear interpolation
    float y = y0 + (x - x0) * (y1 - y0) / (x1 - x0);
    return y;
}


float findMax(float arr[], int size) {
    float maxVal = arr[0]; // Initialize maxVal with the first element of the array

    for (int i = 1; i < size; i++) {
        if (arr[i] > maxVal) {
            maxVal = arr[i]; // Update maxVal if a larger value is found
        }
    }

    return maxVal;
}


float findMin(float arr[], int size) {
    float minVal = arr[0]; // Initialize maxVal with the first element of the array

    for (int i = 1; i < size; i++) {
        if (arr[i] < minVal) {
            minVal = arr[i]; // Update maxVal if a larger value is found
        }
    }

    return minVal;
}
float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}