#define LEFTBUMPER A0
#define RIGHTBUMPER A1
#define LEFTPHOTO A2
#define CENTERPHOTO A3
#define RIGHTPHOTO A4
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
  digitalWrite(ENABLE1, HIGH);
  digitalWrite(ENABLE2, HIGH);

  leftPhotoReading = analogRead(LEFTPHOTO);
  middlePhotoReading = analogRead(CENTERPHOTO);
  rightPhotoReading = analogRead(RIGHTPHOTO); 
  leftBumper = analogRead(LEFTBUMPER);
  rightBumper = analogRead(RIGHTBUMPER);
  frontBumper = analogRead(FRONTBUMPER);
  logReadings();
  if(frontIsObstructed()){
    allStop();
  }
  else if(leftIsObstructed() && rightIsObstructed()){
    forward();
  }
  else if(lineDetected()){
    forward();
    delay(STEP_DELAY);
  }
  else if(shouldTurnLeft()){
    left();
  }
  else{
    right();
  }
  delay(1);
  allStop();
  delay(600);
}

void forward(){
    digitalWrite(FORWARD1, HIGH);
  digitalWrite(FORWARD2, HIGH);
  digitalWrite(BACKWARD1, LOW);
  digitalWrite(BACKWARD2, LOW);
}
void stop(){}

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
}

boolean lineDetected(){
  boolean detected = (middlePhotoReading < rightPhotoReading) && (middlePhotoReading < leftPhotoReading);
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
  Serial.println(leftPhotoReading);
  Serial.println(middlePhotoReading);
  Serial.println(rightPhotoReading);
  Serial.println("BUMPERS");
  Serial.println(leftBumper);
  Serial.println(rightBumper);
  Serial.println(frontBumper);
  delay(100);
}
