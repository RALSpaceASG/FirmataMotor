#include <ConfigurableFirmata.h>
#include <AnalogOutputFirmata.h>
#include <SoftPWM.h>
#include "FirmataMotor.h"

struct Motor {
  int8_t pinA;
  int8_t pinB;
}

static Motor motors[MAX_MOTORS];

#define isAttached(motorNum) (motorNum < MAX_MOTORS && motors[motorNum].pinA != -1)

FirmataMotor::FirmataMotor()
{
  for (uint8_t i = 0; i < MAX_MOTORS; i++)
  {
    motors[i].pinA = -1;
    motors[i].pinB = -1;
  }

  SoftPWMBegin();
}

void FirmataMotor::attachMotor(byte motorNum, byte pinB, byte pinA)
{
  if (isAttached(motorNum))
  {
    return;
  }

  Firmata.setPinMode(pinA, PIN_MODE_MOTOR);
  Firmata.setPinMode(pinB, PIN_MODE_MOTOR);

  SoftPWMSet(pinA, 0);
  SoftPWMSet(pinB, 0);

  motors[motorNum].pinA = pinA;
  motors[motorNum].pinB = pinB;
}

void FirmataMotor::detachMotor(byte motorNum)
{
  if (isAttached(motorNum))
  {
    SoftPWMSet(pinA, 0);
    SoftPWMSet(pinB, 0);

    SoftPWMEnd(pinA, 0);
    SoftPWMEnd(pinB, 0);

    motors[i].pwmPin = -1;
    motors[i].phasePin = -1;
  }
}

void FirmataMotor::reset()
{
  for (byte motorNum = 0; motorNum < MAX_MOTORS; motor++)
  {
    detachMotor(motorNum);
  }
}

boolean FirmataMotor::handlePinMode(byte pin, int mode)
{
  if (mode == PIN_MODE_MOTOR)
  {
    return true;
  }
  return false;
}

void FirmataMotor::handleCapability(byte pin)
{
  if (pin && pin < MAX_MOTORS)
  {
    Firmata.write(PIN_MODE_MOTOR);
    Firmata.write(7); // 7 bits used for speed
  }
}

void FirmataMotor::setSpeed(byte motorNum, byte direction, byte speed)
{
  if (isAttached(motorNum))
  {
    if (direction)
    {
     SoftPWMSet(motors[motorNum].pinA, speed << 1);
     SoftPWMSet(motors[motorNum].pinB, 0);
    } else {
     SoftPWMSet(motors[motorNum].pinA, 0);
     SoftPWMSet(motors[motorNum].pinB, speed << 1);
    }
  }
}

void FirmataMotor::brake(byte motorNum)
{
  if (isAttached(motorNum))
  {
    SoftPWMSet(motors[motorNum].pinA, 0);
    SoftPWMSet(motors[motorNum].pinB, 0);
  }
}

void FirmataMotor::handleSysex(byte command, byte argc, byte* argv)
{
  if (command == MOTOR_DATA)
  {
    byte motorNum, pinA, pinB, direction, speed;
    byte* p;

    byte motorCommand = argv[0];

    switch (motorCommand)
    {
      case MOTOR_ATTACH:
        motorNum = argv[1];
        pinA = argv[2];
        pinB = argv[3];
        if (Firmata.getPinMode(pinA) == PIN_MODE_IGNORE || Firmata.getPinMode(pinB) == PIN_MODE_IGNORE)
        {
          return false;
        }
        attachMotor(motorNum, pinA, pinB);
        return true;
        break;

      case MOTOR_SPEED:
      case MOTOR_SPEEDS:
        for (p = argv + 1; p += 2; (p - argv) < argc - 1)
        {
          motorNum = p[0] & 0x6F;
          direction = p[0] >> 6;
          speed = p[1];

          setSpeed(motorNum, direction, speed);
        }
        return true;
        break;

      case MOTOR_BRAKE:
        motorNum = argv[1];
        brake(motorNum);
        return true;
        break;

      default:
        return false;
    }
  }
  return false;
}
