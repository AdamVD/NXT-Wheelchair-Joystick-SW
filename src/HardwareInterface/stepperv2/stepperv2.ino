// author: ARTHUR THOMPSON JOHNSON
// nov 9 2019

byte dirPinR = 6;
byte stepPinR = 3;
byte dirPinL = 7;
byte stepPinL = 4;
byte enablePin = 8;
int stepLen = 2000; // 'speed' of steppers
float curX = 1; // current x
float curY = 3; // current y
float newX = 1; // recieved x
float newY = 1; // recieved y
float inPerstep = 0.00096; // inches per step, might not be accurate needs testing
long stepsR = 0; // calculated steps to move right stepper
long stepsL = 0; // calculated steps to move left stepper
int res = 40; // arbitrary num determing resolution of movement

void setup() {
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  pinMode(dirPinR, OUTPUT);
  pinMode(stepPinR, OUTPUT);
  pinMode(dirPinL, OUTPUT);
  pinMode(stepPinL, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.println("loop");
  stepCalc();
  moveCntrl();
  delay(1000000);


}

void stepMoveR(int steps) { // moves right motor 'steps' num of steps
  for (int i = 0; i < steps; i++) {
      digitalWrite(stepPinR, HIGH);
      digitalWrite(stepPinR, LOW);
      delayMicroseconds(stepLen);
   
  }
}

void stepMoveL(int steps) { // moves left motor 'steps' num of steps
  for (int i = 0; i < steps; i++) {
      digitalWrite(stepPinL, HIGH);
      delayMicroseconds(200);
      digitalWrite(stepPinL, LOW);
      delayMicroseconds(stepLen);
  }
}

void stepCalc() { // calcultes how much x and y need to change, calculates how many total steps R and L need to step
  Serial.println("stepCalc");

  long newstepsX = newX / inPerstep; // converts x to steps
  long newstepsY = newY / inPerstep; // converts y to steps
  long oldstepsX = curX / inPerstep;
  long oldstepsY = curY / inPerstep;
  long c = 2/inPerstep;
  Serial.print("newStepsX = ");
  Serial.println(newstepsX);
  Serial.print("newStepsY = ");
  Serial.println(newstepsY);
  
  Serial.print("oldStepsX = ");
  Serial.println(oldstepsX);
  Serial.print("oldStepsY = ");
  Serial.println(oldstepsY);
  
  long xsq = sq(newstepsX);
  long xsq2 = sq(c - newstepsX);
  long ysq = sq(newstepsY);

  long oldxsq = sq(oldstepsX);
  long oldxsq2 = sq(c - oldstepsX);
  long oldysq = sq(oldstepsY);


  long oldstepsR = sqrt(oldxsq2 + oldysq);
  long oldstepsL = sqrt(oldxsq + oldysq);
  
  stepsR = sqrt(xsq2 + ysq); // calc R steps
  stepsL = sqrt(xsq + ysq); // calc L steps
  
  if (oldstepsR > stepsR){
    digitalWrite(dirPinR, LOW); // right reverse
    Serial.println("Right Reverse");
  }
  if (oldstepsR < stepsR){
    digitalWrite(dirPinR, HIGH); // right forward
    Serial.println("Right Forward");
  }
  if (oldstepsL > stepsL){
    digitalWrite(dirPinL, LOW); // left reverse
    Serial.println("Left Reverse");
  }
  if (oldstepsL < stepsL){
    digitalWrite(dirPinL, HIGH); //left forward
    Serial.println("Left Forward");
  }
  stepsR = abs(oldstepsR - stepsR);
  stepsL = abs(oldstepsL - stepsL);
}


void moveCntrl(){ // divides total movement into small steps for smoother movement, calls the move functions.
  int numR = stepsR/res; // R step lenghts
  int numL = stepsL/res; // L step lengths
  int r = 0;
  int l = 0;
  Serial.print("stepsR = ");
  Serial.println(stepsR);
  Serial.print("stepsL = ");
  Serial.println(stepsL);
  while (r < stepsR || l < stepsL){
    Serial.print("r = ");
    Serial.println(r);
    Serial.print("l = ");
    Serial.println(l);
    delay(4);
    if (r < stepsR){
      stepMoveR(numR);
      r += numR;
    }
    delay(4);
    if (l < stepsL){
      stepMoveL(numL);
      l += numL;
    }
  }
  curX = newX;
  Serial.print("NEW X = ");
  Serial.println(newX);
  curY = newY;
  Serial.print("NEW Y = ");
  Serial.print(newY);
  
}
