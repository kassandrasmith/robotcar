#define LEFTBUMPER A0
#define RIGHTBUMPER A1
#define LEFTPHOTO A4
#define CENTERPHOTO A3
#define RIGHTPHOTO A2
#define FRONTBUMPER A5
#define ENABLE1 7
#define ENABLE2 12
#define FORWARD1 11
#define FORWARD2 10
#define BACKWARD1 9
#define BACKWARD2 8

#define LED 13

#define STEP_DELAY 600
#define DANGEROUS_BUMPER_LEVEL 280
// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(ENABLE1, OUTPUT);
  pinMode(ENABLE2, OUTPUT);
  pinMode(FORWARD1, OUTPUT);
  pinMode(FORWARD2, OUTPUT);
  pinMode(BACKWARD1, OUTPUT);
  pinMode(BACKWARD2, OUTPUT);
  digitalWrite(ENABLE1, HIGH);
  digitalWrite(ENABLE2, HIGH);
 Serial.begin(9600);
}

int leftPhotoReading;
int rightPhotoReading;
int middlePhotoReading;
int frontBumper;
int rightBumper;
int leftBumper;
// the loop function runs over and over again forever
void loop() {

  //BEGIN MEASUREMENTS
  leftPhotoReading = analogRead(LEFTPHOTO);
  middlePhotoReading = analogRead(CENTERPHOTO);
  rightPhotoReading = analogRead(RIGHTPHOTO); 
  leftBumper = analogRead(LEFTBUMPER);
  rightBumper = analogRead(RIGHTBUMPER);
  frontBumper = analogRead(FRONTBUMPER);
  logReadings();
 
  enableMotors();
  //BEGIN ALGORITHM 
  
  //If the car cannot move forward it will stop. The user must remove the obstacle
  //manually.
  if(frontIsObstructed()){
    allStop();
  }
  //If both the left and right are obstructed, there's only one direction we
  //can go in
  else if(leftIsObstructed() && rightIsObstructed()){
    forward();
  }
  //We're going to assume the line continues in the same direction.
  else if(lineDetected()){
    forward();
    //we only want to leave the forward motion engaged for a short time.
    
  }
  else if(shouldTurnLeft()){
    left();
  }
  else{
    right();
  }
  //No matter what state we ended up in, we want to allow it to persist
  //for a few milliseconds.
  delay(STEP_DELAY);
  //After the delay, we'll stop...
  allStop();
  //and give the ADC a breather so we can get fresh readings for the next loop.
  delay(600);
}

void forward(){
  digitalWrite(FORWARD1, HIGH);
  digitalWrite(FORWARD2, HIGH);
  digitalWrite(BACKWARD1, LOW);
  digitalWrite(BACKWARD2, LOW);
}

void backward(){
  digitalWrite(FORWARD1, LOW);
  digitalWrite(FORWARD2, LOW);
  digitalWrite(BACKWARD1, HIGH);
  digitalWrite(BACKWARD2, HIGH);
}

void left(){
  digitalWrite(FORWARD1, LOW);
  digitalWrite(FORWARD2, HIGH);
  digitalWrite(BACKWARD1, HIGH);
  digitalWrite(BACKWARD2, LOW);
}

void right(){
  digitalWrite(FORWARD1, HIGH);
  digitalWrite(FORWARD2, LOW);
  digitalWrite(BACKWARD1, LOW);
  digitalWrite(BACKWARD2, HIGH);
}
void allStop(){
  digitalWrite(FORWARD1, LOW);
  digitalWrite(FORWARD2, LOW);
  digitalWrite(BACKWARD1, LOW);
  digitalWrite(BACKWARD2, LOW);
  digitalWrite(ENABLE1, LOW);
  digitalWrite(ENABLE2, LOW);
}
void enableMotors(){
  digitalWrite(ENABLE1, HIGH);
  digitalWrite(ENABLE2, HIGH); 
}

boolean lineDetected(){
  boolean detected = (middlePhotoReading < rightPhotoReading) && (middlePhotoReading < leftPhotoReading);
  //We'll set this indicator so that we can tell the state of the 
  //detection externally.
  if(detected){
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
  return detected;
}
boolean frontIsObstructed(){
  return frontBumper > DANGEROUS_BUMPER_LEVEL;
}
boolean leftIsObstructed(){
  return leftBumper > DANGEROUS_BUMPER_LEVEL;
}
boolean rightIsObstructed(){
  return rightBumper > DANGEROUS_BUMPER_LEVEL;
}
boolean shouldTurnLeft(){
     return ((leftPhotoReading < middlePhotoReading) && (leftPhotoReading < rightPhotoReading));
}
boolean shouldTurnRight(){
    return ((rightPhotoReading < middlePhotoReading) && (rightPhotoReading < leftPhotoReading));
}
void logReadings(){
  Serial.println("***");
  Serial.println("PHOTORESISTORS: L M R");
  Serial.println(leftPhotoReading);
  Serial.println(middlePhotoReading);
  Serial.println(rightPhotoReading);
  Serial.println("BUMPERS: L F R");
  Serial.println(leftBumper);
  Serial.println(frontBumper);
  Serial.println(rightBumper);
  delay(100);
}
