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
  motor.handleSysex(MOTOR_DATA, 3, message);

  assertEqual(Firmata.getPinState(pin1), 255);
  assertEqual(Firmata.getPinState(pin2), 255);
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
