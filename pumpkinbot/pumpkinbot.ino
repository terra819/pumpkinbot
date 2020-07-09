
/***************************************************
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These drivers use I2C to communicate, 2 pins are required to
  interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;

void setup() {
  Serial.begin(9600);

  pwm1.begin();
  // In theory the internal oscillator is 25MHz but it really isn't
  // that precise. You can 'calibrate' by tweaking this number till
  // you get the frequency you're expecting!
  pwm1.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz
  pwm1.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  Serial.println("pwm1 setup");
  pwm2.begin();
  // In theory the internal oscillator is 25MHz but it really isn't
  // that precise. You can 'calibrate' by tweaking this number till
  // you get the frequency you're expecting!
  pwm2.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz
  pwm2.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  Serial.println("pwm2 setup");
  stand();
}

void loop() {
  if (Serial.available() > 0) {
    int cmd = Serial.read();
    switch (cmd) {
      case '1':
        stand();
        break;
      case '2':
        sit();
        break;
    }
  }
}

void stand() {
  Serial.println("Standing");
  uint16_t degrees = 90;
  double pulseLen = map(degrees, 0, 180, SERVOMIN, SERVOMAX);
  setCoxaDegrees(degrees);
  setTibiaDegrees(degrees);
  setFemurDegrees(degrees);
}

void sit() {
  Serial.println("Sitting");
  setCoxaDegrees(90);
  setTibiaDegrees(1);
  setFemurDegrees(1);
}

void setCoxaDegrees(uint16_t degrees) {
  for (uint16_t servonum = 0; servonum < 7; servonum++) {
    pwm1.setPWM(servonum, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  }
}

void setFemurDegrees(uint16_t degrees) {
  for (uint16_t servonum = 7; servonum < 15; servonum++) {
    pwm1.setPWM(servonum, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  }
}

void setTibiaDegrees(uint16_t degrees) {
  for (uint16_t servonum = 0; servonum < 7; servonum++) {
    pwm2.setPWM(servonum, 0, map(degrees, 0, 180, SERVOMIN, SERVOMAX));
  }
}
