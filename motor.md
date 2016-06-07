# Motor Feature

Provide motor support using a H-bridge. See [this article](https://www.pololu.com/product/2135) for more information on how this works.

Uses [PJRC's SoftPWM](https://www.pjrc.com/teensy/td_libs_SoftPWM.html) to allow the H-bridge to be connected to any pin.

Example files :
 * MotorFeature is a feature for [ConfigurableFirmata.ino](https://github.com/firmata/arduino/blob/configurable/examples/ConfigurableFirmata/ConfigurableFirmata.ino).
 * A dedicated example is available. See [SimpleMotorFirmata.ino](FirmataMotor/examples/SimpleMotorEncoder).

## Compatible client librairies
TODO : Update this

## Tested boards
This feature has been tested on :
 * Arduino Leonardo

## Protocol details
The protocol below use exclusively SysEx queries and SysEx responses.

### Attach encoder query
Query :
```c
 /* -----------------------------------------------------
 * 0 START_SYSEX                (0xF0)
 * 1 MOTOR_DATA                 (0x61)
 * 2 MOTOR_ATTACH               (0x00)
 * 3 motor #                    ([0 - MAX_MOTORS-1])
 * 4 pin A #                    (first pin)
 * 5 pin B #                    (second pin)
 * 6 END_SYSEX                  (0xF7)
 * -----------------------------------------------------
 */
```
No response.

### Set motor speed
Query
```c
 /* -----------------------------------------------------
 * 0 START_SYSEX                (0xF0)
 * 1 MOTOR_DATA                 (0x61)
 * 2 MOTOR_SPEED                (0x01)
 * 3 Motor #  &  DIRECTION      [= (direction << 6) | (#)]
 * 4 speed, 7-bits              (0-127)
 * 5 END_SYSEX                  (0xF7)
 * -----------------------------------------------------
 */
```
No response.

Note :
Byte #4 contains both motor's number (i.e. channel) and motor's direction.
Direction is stored on the seventh bit,  0 (LOW) for positive and 1 (HIGH) for negative.
```c
directionMask = 0x40; // B01000000
channelMask   = 0x3F; // B00111111

//ex direction is negative and motor is on index 2
direction = 1;
channel = 2;
bytes[2] =  (direction << 6) | (channel);
```

### Set all motor speeds
Query
```c
 /* -----------------------------------------------------
 * 0 START_SYSEX                (0xF0)
 * 1 MOTOR_DATA                 (0x61)
 * 2 MOTOR_SPEEDS               (0x02)
 * 3 first motor # & first motor direction
 * 4 first motor speed
 * 5 second motor # & second motor direction
 * ...
 * N END_SYSEX                  (0xF7)
 * -----------------------------------------------------
 */
```

No response.

### Brake motor
Query
```c
 /* -----------------------------------------------------
 * 0 START_SYSEX                (0xF0)
 * 1 MOTOR_DATA                 (0x61)
 * 2 MOTOR_BRAKE                (0x03)
 * 3 motor #                    ([0 - MAX_MOTORS-1])
 * 4 END_SYSEX                  (0xF7)
 * -----------------------------------------------------
 */
```

No response.

### Detach motor
Query
```c
 /* -----------------------------------------------------
 * 0 START_SYSEX                (0xF0)
 * 1 MOTOR_DATA                 (0x61)
 * 2 MOTOR_DETACH               (0x05)
 * 3 motor #                    ([0 - MAX_MOTORS-1])
 * 4 END_SYSEX                  (0xF7)
 * -----------------------------------------------------
 */
```

No response.
