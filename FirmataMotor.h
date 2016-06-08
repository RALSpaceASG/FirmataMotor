
#ifndef FirmataMotor_h
#define FirmataMotor_h

#include <ConfigurableFirmata.h>
#include <FirmataFeature.h>

// these should really be in ConfigurableFirmata.h?
#ifndef MOTOR_DATA
#define MOTOR_DATA       (0x0F)
#endif
#ifndef PIN_MODE_MOTOR
#define PIN_MODE_MOTOR   (0x10)
#endif

#define MAX_MOTORS       4
#define MOTOR_ATTACH     (0x00)
#define MOTOR_SPEED      (0x01)
#define MOTOR_SPEEDS     (0x02)
#define MOTOR_BRAKE      (0x03)
#define MOTOR_DETACH     (0x05)

class FirmataMotor:public FirmataFeature
{
public:
  FirmataMotor();

  // FirmataFeature - needs to be implemented
  boolean handlePinMode(byte pin, int mode);
  void handleCapability(byte pin);
  boolean handleSysex(byte command, byte argc, byte *argv);
  void reset();

  boolean isMotorAttached(byte motorNum);
  void attachMotor(byte motorNum, byte phasePin, byte pwmPin);
  void detachMotor(byte motorNum);
  void setSpeed(byte motorNum, byte direction, byte speed);
  void brake(byte motorNum);

private:
  void _setPins(byte motorNum, byte valueA, byte valueB);
};

#endif
