
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
#define MOTOR_DETACH     (0x03)

class FirmataMotor:public FirmataFeature
{
public:
  FirmataMotor();

  // FirmataFeature - needs to be implemented
  boolean handlePinMode(byte pin, int mode);
  void handleCapability(byte pin);
  boolean handleSysex(byte command, byte argc, byte *argv);
  void reset();

  void attachMotor(byte motorNum, byte phasePin, byte pwmPin);
  void detachMotor(byte motorNum);
  void setSpeed(byte motorNum, byte direction, byte speed);
  void brake(byte motorNum);
}

#endif
