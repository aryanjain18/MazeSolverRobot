/*
   Line sensor on A0,A1,A2,A3,A4,A5,A6
   A0-left & A6 - right

*/
const int startButton = 11;

bool l = 0;
bool r = 0;
bool s = 0;
bool u = 0;
int e = 0;
int paths = 0;

bool endFound = 0;


int FT = 70;  // how much the bot moves forward before turning

int P, D, I, previousError, PIDvalue, error;
int lsp = 100;
int rsp = 100;


int lfspeed = 80;  // change this for bot average speed

int turnspeed = 50;
float Kp = 0.05;
float Kd = 1;
float Ki = 0;

String str;

#include <SparkFun_TB6612.h>

#define AIN1 4
#define BIN1 6
#define AIN2 3
#define BIN2 7
#define PWMA 9
#define PWMB 10
#define STBY 5

// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

int minValues[8], maxValues[8], threshold[8];

void setup() {
  Serial.begin(9600);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);

  red();
  delay(500);
  lightsoff();
}

void loop() {

  while (digitalRead(11)) {}
  delay(1000);
  calibrate();

  while (digitalRead(11)) {}
  delay(500);

  while (endFound == 0) {
    linefollow();
    checknode();

    botstop();
    delay(100);

    reposition();
  }
  for (int m = 0; m < 4; m++) {
    str.replace("LUL", "S");
    str.replace("SUL", "R");
    str.replace("LUS", "R");
    str.replace("RUL", "U");
  }
  int endpos = str.indexOf('E');

  while (digitalRead(11)) {}
  delay(5000);

  for (int i = 0; i <= endpos; i++) {
    char node = str.charAt(i);
    paths = 0;
    while (paths < 2) {
      linefollow();
      checknode();
      if (paths == 1) {
        botstop();
        delay(100);
        reposition();
      }
    }
    switch (node) {
      case 'L':
        botstop();
        delay(100);
        botleft();
        break;

      case 'S':
        break;

      case 'R':
        botstop();
        delay(100);
        botright();
        break;

      case 'E':
        red();
        botstop();
        delay(5000);
        break;
    }  //_________end of switch
  }    //_________end of for loop
}


void calibrate() {
  for (int i = 0; i < 7; i++) {
    minValues[i] = analogRead(i);
    maxValues[i] = analogRead(i);
  }

  for (int i = 0; i < 1500; i++) {
    motor1.drive(30);
    motor2.drive(-30);

    for (int i = 0; i < 7; i++) {
      if (analogRead(i) < minValues[i]) {
        minValues[i] = analogRead(i);
      }
      if (analogRead(i) > maxValues[i]) {
        maxValues[i] = analogRead(i);
      }
    }
  }

  for (int i = 0; i < 7; i++) {
    threshold[i] = (minValues[i] + maxValues[i]) / 2;
    Serial.print(threshold[i]);
    Serial.print("   ");
  }
  Serial.println();

  motor1.drive(0);
  motor2.drive(0);

  red();
  delay(500);
  lightsoff();
}

void checknode() {
  l = 0;
  r = 0;
  s = 0;
  u = 0;
  e = 0;
  paths = 0;

  // checks whethere bot is on node and the number of exits possible

  for (int i = 0; i < 50; i++) {
    if (analogRead(6) < threshold[6]) r = 1;
    if (analogRead(0) < threshold[0]) l = 1;
    if ((analogRead(0) > threshold[0] && (analogRead(6) > threshold[6]) && (analogRead(3) > threshold[3]))) {
      u = 1;
    }
    if ((analogRead(3) < threshold[3]) && (analogRead(0) < threshold[0]) && (analogRead(6) < threshold[6])) {
      e = 1;
    }
  }

  if (u == 0) {
    for (int i = 0; i < FT; i++) {
      PID();
      if (analogRead(6) < threshold[6]) r = 1;
      if (analogRead(0) < threshold[0]) l = 1;
    }

    for (int i = 0; i < FT; i++) {
      PID();
      if (analogRead(3) < threshold[3]) s = 1;
      if ((e == 1) && (analogRead(0) < threshold[0]) && (analogRead(6) < threshold[6]) && (analogRead(3) < threshold[3])) e = 2;
    }
    if (e == 2) {
      for (int i = 0; i < FT; i++) {
        PID();
      }
      if ((analogRead(0) < threshold[0]) && (analogRead(6) < threshold[6]) && (analogRead(3) < threshold[3])) e = 3;
    }
  }
  if (u == 1) {
    for (int i = 0; i < 5; i++) {
      botinchforward();
    }
  }

  paths = l + s + r;
}

void red() {
  digitalWrite(2, LOW);
  digitalWrite(8, LOW);
  digitalWrite(13, HIGH);  //RED
}

void lightsoff() {
  digitalWrite(2, LOW);
  digitalWrite(8, LOW);
  digitalWrite(13, LOW);
}

void linefollow() {
  paths = 0;
  while ((analogRead(0) > threshold[0]) && (analogRead(6) > threshold[6]) && (analogRead(3) < threshold[3])) {
    PID();
  }
}
void PID() {
  int error = analogRead(2) - analogRead(4);

  P = error;
  I = I + error;
  D = error - previousError;

  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previousError = error;

  lsp = lfspeed - PIDvalue;
  rsp = lfspeed + PIDvalue;

  if (lsp > 200) {
    lsp = 200;
  }
  if (lsp < 0) {
    lsp = 0;
  }
  if (rsp > 200) {
    rsp = 200;
  }
  if (rsp < 0) {
    rsp = 0;
  }

  motor1.drive(rsp);
  motor2.drive(lsp);
}

void reposition() {
  if (e == 3) {
    str += 'E';
    endFound = 1;
    red();
    botstop();
    delay(2000);
    lightsoff();
  } else if (l == 1) {
    if (paths > 1) str += 'L';
    botleft();  //take left
  }

  else if (s == 1) {
    if (paths > 1) str += 'S';
  } else if (r == 1) {
    if (paths > 1) str += 'R';
    botright();  //take right
  }

  else if (u == 1) {
    str += 'U';
    botuturn();  //take left
  }
}

void botleft() {
  motor1.drive(-1 * turnspeed);
  motor2.drive(turnspeed);
  delay(250);
  while (analogRead(3) > threshold[3]) {
    motor1.drive(-1 * turnspeed);
    motor2.drive(turnspeed);
  }
  motor1.drive(0);
  motor2.drive(0);
  delay(50);
}

void botright() {
  motor1.drive(turnspeed);
  motor2.drive(-1 * turnspeed);
  delay(250);
  while (analogRead(3) > threshold[3]) {
    motor1.drive(turnspeed);
    motor2.drive(-1 * turnspeed);
  }
  motor1.drive(0);
  motor2.drive(0);
  delay(50);
}

void botstraight() {
  motor1.drive(lfspeed);
  motor2.drive(lfspeed);
}

void botinchforward() {
  motor1.drive(turnspeed);
  motor2.drive(turnspeed);
  delay(10);
}
void botstop() {
  motor1.drive(0);
  motor2.drive(0);
}
void botuturn() {
  motor1.drive(-1 * turnspeed);
  motor2.drive(turnspeed);
  delay(300);
  while (analogRead(3) > threshold[3]) {
    motor1.drive(-1 * turnspeed);
    motor2.drive(turnspeed);
  }
  motor1.drive(0);
  motor2.drive(0);
  delay(50);
}
