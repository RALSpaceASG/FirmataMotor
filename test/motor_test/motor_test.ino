#include <ArduinoUnit.h>
#include <ConfigurableFirmata.h>
#include "FirmataMotor.h"

FakeStream stream;

void setup()
{
  Firmata.begin(stream); // use fake stream

  Serial.begin(57600); // must match port configuration

  while (!Serial) {}; // wait for serial - needed for Leonardo
}

test(attachMotor)
{
  FirmataMotor motor;
  byte motorNum = 0, pin1 = 7, pin2 = 13;

  assertFalse(motor.isMotorAttached(motorNum));

  motor.attachMotor(motorNum, pin1, pin2);

  assertTrue(motor.isMotorAttached(motorNum));
}

test(requiredMemoryPerInstance)
{
  assertTestPass(attachMotor);
  FirmataMotor motor;
  byte motorNum = 0, pin1 = 7, pin2 = 13;
  int initialMemory = freeMemory();
  motor.attachMotor(motorNum, pin1, pin2);
  int afterInitializationMemory = freeMemory();
  int requiredMemory = initialMemory - afterInitializationMemory;
  Serial.print(requiredMemory, DEC);
  Serial.println(" mem req. per instance");
  assertTrue(requiredMemory < 16);
}

test(handleAttachMotorMessage)
{
  FirmataMotor motor;
  byte motorNum = 0, pin1 = 7, pin2 = 13;

  assertFalse(motor.isMotorAttached(motorNum));
  byte message[]={MOTOR_ATTACH, motorNum, pin1, pin2};
  motor.handleSysex(MOTOR_DATA, 4, message);

  assertTrue(motor.isMotorAttached(motorNum));
}

test(detachMotor)
{
  FirmataMotor motor;
  byte motorNum = 0, pin1 = 7, pin2 = 13;
  motor.attachMotor(motorNum, pin1, pin2);

  motor.detachMotor(motorNum);
  assertFalse(motor.isMotorAttached(motorNum));
}

test(handeDetachMotorMessage)
{
  FirmataMotor motor;
  byte motorNum = 0, pin1 = 7, pin2 = 13;
  motor.attachMotor(motorNum, pin1, pin2);

  byte message[]={MOTOR_DETACH, motorNum};
  motor.handleSysex(MOTOR_DATA, 2, message);
  assertFalse(motor.isMotorAttached(motorNum));
}

test(brake)
{
  FirmataMotor motor;
  byte motorNum = 0, pin1 = 7, pin2 = 13;
  motor.attachMotor(motorNum, pin1, pin2);

  motor.brake(motorNum);

  assertEqual(Firmata.getPinState(pin1), 255);
  assertEqual(Firmata.getPinState(pin2), 255);
}

test(handleBrakeMotorMessage)
{
  FirmataMotor motor;
  byte motorNum = 0, pin1 = 7, pin2 = 13;
  motor.attachMotor(motorNum, pin1, pin2);

  byte message[]={MOTOR_BRAKE, motorNum};
  motor.handleSysex(MOTOR_DATA, 2, message);

  assertEqual(Firmata.getPinState(pin1), 255);
  assertEqual(Firmata.getPinState(pin2), 255);
}

test(setSpeed)
{
  FirmataMotor motor;
  byte motorNum = 0, pin1 = 7, pin2 = 13;
  motor.attachMotor(motorNum, pin1, pin2);

  // forward
  motor.setSpeed(motorNum, 0, 32);
  assertEqual(Firmata.getPinState(pin1), 32 << 1);
  assertEqual(Firmata.getPinState(pin2), 0);

  // coast
  motor.setSpeed(motorNum, 0, 0);
  assertEqual(Firmata.getPinState(pin1), 0);
  assertEqual(Firmata.getPinState(pin2), 0);

  // reverse
  motor.setSpeed(motorNum, 1, 32);
  assertEqual(Firmata.getPinState(pin1), 0);
  assertEqual(Firmata.getPinState(pin2), 32 << 1);

  // brake
  motor.brake(motorNum);
  assertEqual(Firmata.getPinState(pin1), 255);
  assertEqual(Firmata.getPinState(pin2), 255);
}

test(handleSetSpeedMessage)
{

  FirmataMotor motor;
  byte motorNum = 0, pin1 = 7, pin2 = 13;
  motor.attachMotor(motorNum, pin1, pin2);

  // forward
  byte fMessage[]={MOTOR_SPEED, (0 << 6) | motorNum, 32};
  motor.handleSysex(MOTOR_DATA, 3, fMessage);
  assertEqual(Firmata.getPinState(pin1), 32 << 1);
  assertEqual(Firmata.getPinState(pin2), 0);

  // brake
  byte bMessage[]={MOTOR_BRAKE, motorNum};
  motor.handleSysex(MOTOR_DATA, 2, bMessage);
  assertEqual(Firmata.getPinState(pin1), 255);
  assertEqual(Firmata.getPinState(pin2), 255);
}

test(handleSetSpeedsMessage)
{
  FirmataMotor motor;
  byte motor1 = 1, pin1 = 4, pin2 = 9;
  byte motor2 = 2, pin3 = 7, pin4 = 13;

  motor.attachMotor(motor1, pin1, pin2);
  motor.attachMotor(motor2, pin3, pin4);

  assertTrue(motor.isMotorAttached(motor1));
  assertTrue(motor.isMotorAttached(motor2));

  // forward
  byte message[]={MOTOR_SPEEDS, (0 << 6) | motor1, 20, (0 << 6) | motor2, 30};
  motor.handleSysex(MOTOR_DATA, 5, message);
  assertEqual(Firmata.getPinState(pin1), 20 << 1);
  assertEqual(Firmata.getPinState(pin2), 0);
  assertEqual(Firmata.getPinState(pin3), 30 << 1);
  assertEqual(Firmata.getPinState(pin4), 0);

  motor.detachMotor(motor1);
  motor.detachMotor(motor2);
  assertFalse(motor.isMotorAttached(motor1));
  assertFalse(motor.isMotorAttached(motor2));
}

test(startZero)
{
  FirmataMotor motor;
  byte motorNum = 0, pin1 = 7, pin2 = 13;
  motor.attachMotor(motorNum, pin1, pin2);

  assertEqual(Firmata.getPinState(pin1), 0);
  assertEqual(Firmata.getPinState(pin2), 0);
}

void loop()
{
  Test::run();
}
