#include <ConfigurableFirmata.h>
#include <AnalogOutputFirmata.h>
#include "FirmataMotor.h"

struct Motor {
  int8_t pinA;
  int8_t pinB;
};

static Motor motors[MAX_MOTORS];

#define isAttached(motorNum) (motorNum < MAX_MOTORS && motors[motorNum].pinA != -1)

FirmataMotor::FirmataMotor()
{
  for (uint8_t i = 0; i < MAX_MOTORS; i++)
  {
    motors[i].pinA = -1;
    motors[i].pinB = -1;
  }
}

boolean FirmataMotor::isMotorAttached(byte motorNum)
{
  return isAttached(motorNum);
}

void FirmataMotor::_setPins(byte motorNum, byte valueA, byte valueB)
{
  analogWrite(motors[motorNum].pinA, valueA);
  analogWrite(motors[motorNum].pinB, valueB);
  Firmata.setPinState(motors[motorNum].pinA, valueA);
  Firmata.setPinState(motors[motorNum].pinB, valueB);
}

void FirmataMotor::attachMotor(byte motorNum, byte pinB, byte pinA)
{
  if (isAttached(motorNum))
  {
    return;
  }

  Firmata.setPinMode(pinA, PIN_MODE_MOTOR);
  Firmata.setPinMode(pinB, PIN_MODE_MOTOR);

  motors[motorNum].pinA = pinA;
  motors[motorNum].pinB = pinB;

  _setPins(motorNum, 0, 0);
}

void FirmataMotor::detachMotor(byte motorNum)
{
  if (isAttached(motorNum))
  {
    _setPins(motorNum, 0, 0);

    motors[motorNum].pinA = -1;
    motors[motorNum].pinB = -1;
  }
}

void FirmataMotor::reset()
{
  for (byte motorNum = 0; motorNum < MAX_MOTORS; motorNum++)
  {
    detachMotor(motorNum);
  }
}

void FirmataMotor::setSpeed(byte motorNum, byte direction, byte speed)
{
  if (isAttached(motorNum))
  {
    if (direction)
    {
      _setPins(motorNum, speed << 1, 0);
    } else
    {
      _setPins(motorNum, 0, speed << 1);
    }
  }
}

void FirmataMotor::brake(byte motorNum)
{
  if (isAttached(motorNum))
  {
    _setPins(motorNum, 255, 255);
  }
}

boolean FirmataMotor::handleSysex(byte command, byte argc, byte* argv)
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

      case MOTOR_DETACH:
        motorNum = argv[1];
        detachMotor(motorNum);
        return true;
        break;

      case MOTOR_SPEED:
      case MOTOR_SPEEDS:
        for (p = argv + 1; (p - argv) < argc - 1; p += 2)
        {
          motorNum = p[0] & 0x3F;
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
