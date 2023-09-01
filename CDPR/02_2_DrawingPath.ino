
void initRectanglePath(struct Point points[], float a, float b){
  for (int i = 0; i < n; i++) {
    points[i].x = a*cosf(i/(float)n*2*PI)/fmaxf(fabs(cosf(i/(float)n*2*PI)), fabs(sinf(i/(float)n*2*PI))) + boxLength/2.0;
    points[i].y = b*sinf(i/(float)n*2*PI)/fmaxf(fabs(cosf(i/(float)n*2*PI)), fabs(sinf(i/(float)n*2*PI))) + boxWidth/2.0;
    points[i].z = zBias+200;
  }
}

void initCirclePath(struct Point points[], float R){
  for (int i = 0; i < n; i++) {
    points[i].x = R*cosf(i/(float)n*2*PI) + boxLength/2.0;
    points[i].y = R*sinf(i/(float)n*2*PI) + boxWidth/2.0;
    points[i].z = zBias+150.0;
    // points[i].z = zBias;


    // Serial.print("(COORDINATE)\t");
    // Serial.println("x: " + String(points[i].x) + "\ty: " + String(points[i].y));
  }
}

void initEllipsePath(struct Point points[], float a, float b){
  for (int i = 0; i < n; i++) {
    float angle = i / static_cast<float>(n) * 2 * M_PI;
    float x = a * cosf(angle) + boxLength/2.0;
    float y = b * sinf(angle) + boxWidth/2.0;
    points[i].x = x;
    points[i].y = y;
    points[i].z = zBias;
  }
}

void initFlowerPath(struct Point points[], float R, int petalCount){
  for (int i = 0; i < n; i++) {
    float angle = i / static_cast<float>(n) * 2 * M_PI;
    float x = R * cosf(angle) *  cosf(petalCount * angle) + boxLength/2.0;
    float y = R * sinf(angle) * cosf(petalCount * angle) + boxWidth/2.0;
    points[i].x = x;
    points[i].y = y;
    points[i].z = zBias+200;
  }
}

void initEightPath(struct Point points[], float R) {
  for (int i = 0; i < n; i++) {
    float angle = i / static_cast<float>(n) * 2 * M_PI;
    float x = R * 1.5 * cosf(angle) + boxLength / 2.0;
    float y = R * sinf(2 * angle) / 1.5 + boxWidth / 2.0;
    points[i].x = x;
    points[i].y = y;
    points[i].z = zBias+200;
  }
}

void printCoordinateForEachPoint(struct Point points[]){
  for(int i = 0; i < n; i++){
    Serial.println(String(i) + " x: " + String(points[i].x) + " y: " + String(points[i].y) + " z: " + points[i].z);
  }
}


void initSpiralPath(struct Point points[], float startRadius, float finalRadius, float numTurns) {
    float spacing = (startRadius - finalRadius) / (n - 1); // Calculate the spacing between each turn
    float deltaAngle = numTurns * 2 * PI / (n - 1); // Calculate the angle increment for each point

    for (int i = 0; i < n; i++) {
        float angle = deltaAngle * i; // Calculate the angle for each point along the spiral
        float radius = startRadius - spacing * i; // Decrease the radius with each turn
        points[i].x = radius * cosf(angle) + boxLength / 2.0;
        points[i].y = radius * sinf(angle) + boxWidth / 2.0;
        points[i].z = zBias + 150.0;
    }
}

void initZeroPath(struct Point points[]){
  for(int i=0; i<n; i++){
    points[i].x = boxLength/2.0;
    points[i].y = boxWidth/2.0;
    points[i].z = zBias;
  }
}