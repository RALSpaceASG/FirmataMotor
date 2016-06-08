#include <ConfigurableFirmata.h>
#include <FirmataExt.h>
#include "FirmataMotor.h"

FirmataExt firmataExt;
FirmataMotor motor;

void systemResetCallback()
{
  firmataExt.reset();
}

void setup()
{
  // TODO - pass the version of this firmware rather than the Firmata protocol
  // version. Making that change now however may break compatibility.
  Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
  firmataExt.addFeature(motor);
  /* systemResetCallback is declared here */
  Firmata.attach(SYSTEM_RESET, systemResetCallback);
  Firmata.begin(57600);
  systemResetCallback();  // reset to default config
}

void loop()
{
  /* STREAMREAD - processing incoming messages as soon as possible, while still
   * checking digital inputs.  */
  while (Firmata.available()) {
    Firmata.processInput();
  }
}