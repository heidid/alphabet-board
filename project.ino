/*
 * Alphabet Board project - originally for CS 194-28 at UC Berkeley
 * 
 * Controls a polargraph with 2 stepper motors 
 *
 * code by Heidi Dong
 * May 9, 2019
 */

#include <Stepper.h>
#include <math.h>


/* -------------------------------------------------------------------------
 *  INITIALIZE VALUES
 */

const int STEPS_PER_REVOLUTION = 200;
const int RPM = 90;

const int STEPS_PER_INCH = 200;
const float DX_ADDITIONAL_INCHES = 2; // bias
const float SQUARE_SIZE_INCHES = 3;

const float DIST_BETWEEN_MOTORS_INCHES = 48;

float leftPulleyLength = 30; // inches, starting point
float rightPulleyLength = 30; // inches, starting point

// initialize the stepper library
Stepper rightStepper(STEPS_PER_REVOLUTION, 8, 9, 10, 11);
Stepper leftStepper(STEPS_PER_REVOLUTION, 2, 3, 5, 6);

int ledPin = 13;
int ledState = 0;

int powerSupplyPin = 1;
int electromagnetsState = 0;

int buttonInputPin_1 = 7;
int buttonVal_1 = 0;
int buttonPressed_1 = 1;

int buttonInputPin_2 = 4;
int buttonVal_2 = 0;
int buttonPressed_2 = 1;

int buttonInputPin_3 = 12;
int buttonVal_3 = 0;
int buttonPressed_3 = 1;

int buttonInputPin_4 = 13;
int buttonVal_4 = 0;
int buttonPressed_4 = 1;

void setup() {
  rightStepper.setSpeed(RPM);
  leftStepper.setSpeed(RPM);
  
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(powerSupplyPin, OUTPUT);
  pinMode(buttonInputPin_1, INPUT);
  pinMode(buttonInputPin_2, INPUT);
  pinMode(buttonInputPin_3, INPUT);
  pinMode(buttonInputPin_4, INPUT);
}

void blinkLED() {
  Serial.println("toggle LED");
  digitalWrite(ledPin, HIGH); 
  delay(1000);
  digitalWrite(ledPin, LOW); 
}

void toggleLED() {
  Serial.println("toggle LED");
  if (ledState == 0) {
    digitalWrite(ledPin, HIGH);
    ledState = 1;  
  } else {
    digitalWrite(ledPin, LOW);
    ledState = 0;  
  }
}



/* -------------------------------------------------------------------------
 *  ELECTROMAGNET CONTROL, INCOMPLETE
 */
void turnElectromagnetsOn() {
  Serial.println("turn electromagnets on");
  digitalWrite(powerSupplyPin, HIGH);
  electromagnetsState = 1;
}

void turnElectromagnetsOff() {
  Serial.println("turn electromagnets off");
  digitalWrite(powerSupplyPin, LOW);
  electromagnetsState = 0;
}

void toggleElectromagnets() {
  
}

void turnElectromagnetsOnReverse() {
  
}

/* -------------------------------------------------------------------------
 *  STEPPER MOTOR CONTROL
 */

const int steps = 10;

// da: amount to change left pulley length, in inches
// db: amount to change right pulley length, in inches
void stepSteppers(float da, float db) {
  Serial.println("stepSteppers");
  // convert from inches to steps
  int da_steps = (int) (da * STEPS_PER_INCH);
  int db_steps = (int) (db * STEPS_PER_INCH);

  int da_dir = (int) (abs(da) / da);
  int db_dir = (int) (abs(db) / db);
  for (int i = 0; i < abs(da_steps / steps); i++) {
    leftStepper.step((int) (da_dir * steps));
    rightStepper.step((int) -(db_dir * db / da * steps)); // negative because flipped 
    delay(10);
  }
}

// da: amount to change left pulley length, in inches
// db: amount to change right pulley length, in inches
void stepSteppersX(float da, float db) {
  Serial.println("stepSteppersX");
  // convert from inches to steps
  int da_steps = (int) (da * STEPS_PER_INCH);
  int db_steps = (int) (db * STEPS_PER_INCH);

  int da_dir = (int) (abs(da) / da);
  int db_dir = (int) (abs(db) / db);
  for (int i = 0; i < abs(da_steps / steps); i++) {
    leftStepper.step((int) (da_dir * steps));
    rightStepper.step((int) (db_dir * db / da * steps));
    delay(10);
  }
}

// dx: distance to move in inches, negative means left, positive means right
// dy: distance to move in inches, negative means up, positive means down
void _move(float dx, float dy) {
  // all variables are in inches
  // rename variables
  float a0 = leftPulleyLength;
  float b0 = rightPulleyLength;
  float w = DIST_BETWEEN_MOTORS_INCHES;
  
  // find current coordinates
  float x0 = (a0*a0 - b0*b0 + w*w) / (2*w);
  float y0 = sqrt(a0*a0 - x0*x0);
  Serial.println(x0);
  Serial.println(y0);

  // find a and b
  float a = sqrt((x0+dx)*(x0+dx) + (y0+dy)*(y0+dy));
  float b = sqrt((w-x0-dx)*(w-x0-dx) + (y0+dy)*(y0+dy));

  // find da and db
  float da = a - a0;
  float db = b - b0;

  // now actually move
  if (dy == 0.0) {
    stepSteppersX(da, db);
  } else {
    stepSteppers(da, db);
  }

  // update vars
  leftPulleyLength = a;
  rightPulleyLength = b;
}

void moveY(float dy) {
  _move(0.0, dy);
}

void moveX(float dx) {
  _move(dx, 0.0);
}



/* -------------------------------------------------------------------------
 *  "PATHFINDING" - INCOMPLETE
 */

/* matrix:
 *  QWERTYUIOPASD
   0             5
   1             6
   2 ........... 7
   3             8
   4             9
    FGHJKLZXCVBNM
 */

int charCount[256];
int matrix[26][4];
void initCharCountArr() {
  charCount['A'] = 1;
  charCount['B'] = 1;  
  charCount['C'] = 1;
  charCount['D'] = 1;
  charCount['E'] = 1;
  charCount['F'] = 1;
  charCount['G'] = 1;
  charCount['H'] = 1;
  charCount['I'] = 1;
  charCount['J'] = 1;
  charCount['K'] = 1;
  charCount['L'] = 1;
  charCount['M'] = 1;
  charCount['N'] = 2;
  charCount['O'] = 2;
  charCount['P'] = 1;
  charCount['Q'] = 1;
  charCount['R'] = 2;
  charCount['S'] = 2;
  charCount['T'] = 1;
  charCount['U'] = 1;
  charCount['V'] = 1;
  charCount['W'] = 1;
  charCount['X'] = 1;
  charCount['Y'] = 1;
  charCount['Z'] = 1;
  charCount['0'] = 4;
  charCount['1'] = 3;
  charCount['2'] = 3;
  charCount['3'] = 3;
  charCount['4'] = 3;
  charCount['5'] = 3;
  charCount['6'] = 2;
  charCount['7'] = 2;
  charCount['8'] = 2;
  charCount['9'] = 2;
  charCount[':'] = 1;
}

void findLetter() {
  
}


/* -------------------------------------------------------------------------
 *  HANDLE INPUT
 */

void handleButton1Press() {
  Serial.println("pressed 1, move up");
  moveY(-SQUARE_SIZE_INCHES);
}

void handleButton2Press() {
  Serial.println("pressed 2, move down");
  moveY(SQUARE_SIZE_INCHES);
}

void handleButton3Press() {
  Serial.println("pressed 3, move left");
  moveX(-SQUARE_SIZE_INCHES - DX_ADDITIONAL_INCHES);
}

void handleButton4Press() {
  Serial.println("pressed 4, move right");
  moveX(SQUARE_SIZE_INCHES + DX_ADDITIONAL_INCHES);
}

void loop() {
  digitalWrite(powerSupplyPin, HIGH);
    
  char data;
  if (Serial.available()) {
    data = Serial.read();
    Serial.println(data);
    if (data == '1') {
      handleButton2Press();
    } else if (data == '2') {
      handleButton1Press();
    } else if (data == '3') {
      handleButton4Press();
    } else if (data == '4') {
      handleButton3Press();
    } else if (data == ' ') {
      toggleElectromagnets();
    }
  }
  
  
  buttonVal_1 = digitalRead(buttonInputPin_1);
  if (buttonVal_1 == HIGH and buttonPressed_1 == 0) { // pressed down
    buttonPressed_1 = 1;
  } else if (buttonVal_1 == LOW and buttonPressed_1 == 1) { // released
    buttonPressed_1 = 0;
    handleButton1Press();
  }
  
  buttonVal_2 = digitalRead(buttonInputPin_2);
  if (buttonVal_2 == HIGH and buttonPressed_2 == 0) { // pressed down
    buttonPressed_2 = 1;
  } else if (buttonVal_2 == LOW and buttonPressed_2 == 1) { // released
    buttonPressed_2 = 0;
    handleButton2Press();
  }

  buttonVal_3 = digitalRead(buttonInputPin_3);
  if (buttonVal_3 == HIGH and buttonPressed_3 == 0) { // pressed down
    buttonPressed_3 = 1;
  } else if (buttonVal_3 == LOW and buttonPressed_3 == 1) { // released
    buttonPressed_3 = 0;
    handleButton3Press();
  }

  buttonVal_4 = digitalRead(buttonInputPin_4);
  if (buttonVal_4 == HIGH and buttonPressed_4 == 0) { // pressed down
    buttonPressed_4 = 1;
  } else if (buttonVal_4 == LOW and buttonPressed_4 == 1) { // released
    buttonPressed_4 = 0;
    handleButton4Press();
  }
}
