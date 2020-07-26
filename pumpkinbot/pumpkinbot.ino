#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

#define SERVOMIN  150
#define SERVOMAX  600
#define USMIN  600
#define USMAX  2400
#define SERVO_FREQ 50
#define OSC_HRZ 27000000

// put all tibias on pwm1
#define L1Tibia 0
#define L2Tibia 1
#define L3Tibia 2
#define L4Tibia 3
#define R4Tibia 4
#define R3Tibia 5
#define R2Tibia 6
#define R1Tibia 7

// put the rest on pwm2
#define L1Femur 10
#define L2Femur 12
#define L3Femur 8
#define L4Femur 7
#define R4Femur 0
#define R3Femur 9
#define R2Femur 11
#define R1Femur 6

#define L1Coxa 5
#define L2Coxa 13
#define L3Coxa 3
#define L4Coxa 2
#define R4Coxa 15
#define R3Coxa 1
#define R2Coxa 4
#define R1Coxa 14

#define Delay 300

int CoxaSit = 90;
int FemurSit = 5;
int TibiaSit = 10;
int CoxaStand = 90;
int FemurStand = 80;
int TibiaStand = 90;
int FemurRaise = 35;
int TibiaRaise = 85;
int LSwingForward = 85;
int RSwingForward = 95;
int LSwingBackward = RSwingForward;
int RSwingBackward = LSwingForward;

void setup() {
  Serial.begin(9600);
  pwm1.begin();
  pwm1.setOscillatorFrequency(OSC_HRZ);
  pwm1.setPWMFreq(SERVO_FREQ);
  pwm2.begin();
  pwm2.setOscillatorFrequency(OSC_HRZ);
  pwm2.setPWMFreq(SERVO_FREQ);
  Sit();
}

void loop() {
  if (Serial.available() > 0) {
    int cmd = Serial.parseInt();
    if (cmd > 0) {
      Serial.println(cmd);
      switch (cmd) {
        case 1:
          WalkForward();
          break;
        case 2:
          WalkBackward();
          break;
        case 3:
          TurnLeft();
          break;
        case 4:
          TurnRight();
          break;
        case 5:
          Stand();
          break;
        case 6:
          Sit();
          break;
        default:
          Stand();
          break;
      }
    }
  }
}

void WalkForward() {
  Serial.println("Walking Forward");
  Stand();
  Tripod1Up();
  Tripod1SwingForward();
  Tripod2SwingBackward();
  Tripod1Stance();
  Tripod2Up();
  Tripod2SwingForward();
  Tripod1SwingBackward();
  Tripod2Stance();
  delay(Delay);
}

void WalkBackward() {
  Serial.println("Walking Backward");
  Stand();
  Tripod1Up();
  Tripod1SwingBackward();
  Tripod2SwingForward();
  Tripod1Stance();
  Tripod2Up();
  Tripod2SwingBackward();
  Tripod1SwingForward();
  Tripod2Stance();
  delay(Delay);
}

void TurnLeft() {
  Serial.println("Turning Left");
  Stand();
  Tripod1Up();
  Tripod1SwingBackward();
  Tripod2SwingForward();
  Tripod1Stance();
  delay(Delay);
}

void TurnRight() {
  Serial.println("Turning Right");
  Stand();
  Tripod2Up();
  Tripod2SwingForward();
  Tripod1SwingBackward();
  Tripod2Stance();
  delay(Delay);
}

void Stand() {
  Serial.println("Standing");
  SetAllCoxas(CoxaStand);
  SetAllFemurs(FemurStand);
  SetAllTibias(TibiaStand);
}

void Sit() {
  Serial.println("Sitting");
  SetAllCoxas(CoxaSit);
  SetAllFemurs(FemurSit);
  SetAllTibias(TibiaSit);
}

void Tripod1Up() {
  Tripod1Femur(FemurRaise);
  Tripod1Tibia(TibiaRaise);
}

void Tripod1SwingForward() {
  SetPwm2(L1Coxa, LSwingForward);
  SetPwm2(L3Coxa, LSwingForward);
  SetPwm2(R4Coxa, RSwingForward);
  SetPwm2(R2Coxa, RSwingForward);
}

void Tripod1SwingBackward() {
  SetPwm2(L1Coxa, LSwingBackward);
  SetPwm2(L3Coxa, LSwingBackward);
  SetPwm2(R4Coxa, RSwingBackward);
  SetPwm2(R2Coxa, RSwingBackward);
}

void Tripod1Stance() {
  Tripod1Femur(FemurStand);
  Tripod1Tibia(FemurStand);
}

void Tripod2Up() {
  Tripod2Femur(FemurRaise);
  Tripod2Tibia(TibiaRaise);
}

void Tripod2SwingForward() {
  SetPwm2(L2Coxa, LSwingForward);
  SetPwm2(L4Coxa, LSwingForward);
  SetPwm2(R3Coxa, RSwingForward);
  SetPwm2(R1Coxa, RSwingForward);
}

void Tripod2SwingBackward() {
  SetPwm2(L2Coxa, LSwingBackward);
  SetPwm2(L4Coxa, LSwingBackward);
  SetPwm2(R3Coxa, LSwingBackward);
  SetPwm2(R1Coxa, LSwingBackward);
}

void Tripod2Stance() {
  Tripod2Femur(FemurStand);
  Tripod2Tibia(TibiaStand);
}

void SetAllFemurs(int degrees) {
  Tripod1Femur(degrees);
  Tripod2Femur(degrees);
}

void SetAllTibias(int degrees) {
  Tripod1Tibia(degrees);
  Tripod2Tibia(degrees);
}

void SetAllCoxas(int degrees) {
  SetPwm2(L1Coxa, degrees);
  SetPwm2(L2Coxa, degrees);
  SetPwm2(L3Coxa, degrees);
  SetPwm2(L4Coxa, degrees);
  SetPwm2(R4Coxa, degrees);
  SetPwm2(R3Coxa, degrees);
  SetPwm2(R2Coxa, degrees);
  SetPwm2(R1Coxa, degrees);
}

void Tripod1Femur(int degrees) {
  SetPwm2(L1Femur, degrees);
  SetPwm2(L3Femur, degrees);
  SetPwm2(R4Femur, degrees);
  SetPwm2(R2Femur, degrees);
}

void Tripod1Tibia(int degrees) {
  SetPwm1(L1Tibia, degrees);
  SetPwm1(L3Tibia, degrees);
  SetPwm1(R4Tibia, degrees);
  SetPwm1(R2Tibia, degrees);
}

void Tripod2Femur(int degrees) {
  SetPwm2(L2Femur, degrees);
  SetPwm2(L4Femur, degrees);
  SetPwm2(R3Femur, degrees);
  SetPwm2(R1Femur, degrees);
}

void Tripod2Tibia(int degrees) {
  SetPwm1(L2Tibia, degrees);
  SetPwm1(L4Tibia, degrees);
  SetPwm1(R3Tibia, degrees);
  SetPwm1(R1Tibia, degrees);
}

void SetPwm1(int servoNumber, int degrees) {
  pwm1.setPWM(servoNumber, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}

void SetPwm2(int servoNumber, int degrees) {
  pwm2.setPWM(servoNumber, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
