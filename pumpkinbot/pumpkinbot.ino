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

#define TIBIA_UP 80
#define TIBIA_DOWN 100
#define TIBIA_CLOSED 45

#define FEMUR_UP 50
#define FEMUR_DOWN 80
#define FEMUR_CLOSED 10

#define COXA_FWD 70
#define COXA_BWD 45
#define COXA_CLOSED 90

#define DELAY 300

/*
                        ~face~
  Leg Tibia   Femur   Coxa    Coxa    Femur   Tibia   Leg
  L1  pmw1.0  pmw2.7  pmw2.15 pmw2.8  pmw2.0  pmw1.7  R1
  L2  pmw1.1  pmw2.6  pmw2.14 pmw2.9  pmw2.1  pmw1.6  R2
  L1  pmw1.2  pmw2.5  pmw2.13 pmw2.10 pmw2.2  pmw1.5  R1
  L2  pmw1.3  pmw2.4  pmw2.12 pmw2.11 pmw2.3  pmw1.4  R2
                        ~butt~
*/

void setup() {
  Serial.begin(9600);
  pwm1.begin();
  pwm1.setOscillatorFrequency(OSC_HRZ);
  pwm1.setPWMFreq(SERVO_FREQ);
  pwm2.begin();
  pwm2.setOscillatorFrequency(OSC_HRZ);
  pwm2.setPWMFreq(SERVO_FREQ);
  stand();
}

void loop() {
  if (Serial.available() > 0) {
    int cmd = Serial.parseInt();
    if (cmd > 0) {
      Serial.println(cmd);
      //   L1_coxa(cmd);
      switch (cmd) {
        case 1:
          walkfwd();
          break;
        case 5:
          sit();
          break;
        case 6:
          stand();
          break;
        default:
          stand();
      }
    }
  }
}

void stand() {
  Serial.println("Standing");
  L1_coxa(COXA_CLOSED);
  L2_coxa(COXA_CLOSED);
  R1_coxa(COXA_CLOSED);
  R2_coxa(COXA_CLOSED);
  L1_femur(FEMUR_DOWN);
  R2_femur(FEMUR_DOWN);
  R1_femur(FEMUR_DOWN);
  L2_femur(FEMUR_DOWN);
  L1_tibia(TIBIA_DOWN);
  R2_tibia(TIBIA_DOWN);
  R1_tibia(TIBIA_DOWN);
  L2_tibia(TIBIA_DOWN);
}

void sit() {
  Serial.println("Sitting");
  L1_coxa(COXA_CLOSED);
  L2_coxa(COXA_CLOSED);
  R1_coxa(COXA_CLOSED);
  R2_coxa(COXA_CLOSED);
  L1_femur(FEMUR_CLOSED);
  R2_femur(FEMUR_CLOSED);
  R1_femur(FEMUR_CLOSED);
  L2_femur(FEMUR_CLOSED);
  L1_tibia(FEMUR_CLOSED);
  R2_tibia(FEMUR_CLOSED);
  R1_tibia(FEMUR_CLOSED);
  L2_tibia(FEMUR_CLOSED);
}

void walkfwd() {
  // l1 swing
  L1_tibia(TIBIA_UP);
  L1_femur(FEMUR_UP);
  L1_coxa(COXA_FWD);
  // l2 stance
  L2_tibia(TIBIA_DOWN);
  L2_femur(FEMUR_DOWN);
  // r1 stance
  R1_tibia(TIBIA_DOWN);
  R1_femur(FEMUR_DOWN);
  // r2 swing
  R2_tibia(TIBIA_UP);
  R2_femur(FEMUR_UP);
  R2_coxa(COXA_BWD);
  // delay
  delay(delay);
  // l1 stance
  L1_tibia(TIBIA_DOWN);
  L1_femur(FEMUR_DOWN);
  // l2 swing
  L2_tibia(TIBIA_UP);
  L2_femur(FEMUR_UP);
  L2_coxa(COXA_FWD);
  // r1 swing
  R1_tibia(TIBIA_UP);
  R1_femur(FEMUR_UP);
  R1_coxa(COXA_BWD);
  // r2 stance
  R2_tibia(TIBIA_DOWN);
  R2_femur(FEMUR_DOWN);
  // delay
  delay(delay);
}

void L1_tibia(uint16_t degrees) {
  pwm1.setPWM(0, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm1.setPWM(2, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
void L2_tibia(uint16_t degrees) {
  pwm1.setPWM(1, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm1.setPWM(3, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
void R1_tibia(uint16_t degrees) {
  pwm1.setPWM(7, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm1.setPWM(5, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
void R2_tibia(uint16_t degrees) {
  pwm1.setPWM(6, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm1.setPWM(4, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
void L1_femur(uint16_t degrees) {
  pwm2.setPWM(7, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm2.setPWM(5, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
void L2_femur(uint16_t degrees) {
  pwm2.setPWM(6, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm2.setPWM(4, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
void R1_femur(uint16_t degrees) {
  pwm2.setPWM(0, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm2.setPWM(2, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
void R2_femur(uint16_t degrees) {
  pwm2.setPWM(1, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm2.setPWM(3, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
void L1_coxa(uint16_t degrees) {
  pwm2.setPWM(15, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm2.setPWM(13, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
void L2_coxa(uint16_t degrees) {
  pwm2.setPWM(14, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm2.setPWM(12, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
void R1_coxa(uint16_t degrees) {
  pwm2.setPWM(8, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm2.setPWM(10, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
void R2_coxa(uint16_t degrees) {
  pwm2.setPWM(9, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  pwm2.setPWM(11, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}

void test(uint16_t degrees) {
  Serial.println(degrees);
  pwm2.setPWM(8, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
}
