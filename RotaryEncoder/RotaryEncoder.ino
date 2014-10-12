/*
 * RotaryEncoder -  the purpose of this sketch is to handle the input of
 *                  a rotary encoder to adjust the brightness of an LED.
 *
 * The rotary encoder output will be tied to one interupt and one normal input
 * line in order to detect movement and determine which direction.
 *
 * One output line will be connected to the LED and we will use PWM
 * to adjust the brightness based on the amount the rotary encoder is
 * turned.
 *
 * We will store the current brightness of the LED (0 - 255) so that we can
 * restore it after a reboot.
 */
const int ledOutput = 3;
const int directionPin = 4;
const int signature = 0xFACE; // some non-arbritary value

/* Module includes */
#include <EEPROM.h>

/* Structures used in this sketch */
typedef struct
{
  long signature;
  int brightness;
} EE_Structure;

/*
 * Variables used by this sketch
 */
volatile EE_Structure StoredData;


/*
 * Method declarations
 */
int ReadInteger(int offset);
void WriteInteger(int offset, int value);
long ReadLong(int offset);
void WriteLong(int offset, long value);
void HandleDecoderTrigger();


/*
 * ReadInteger - a helper function to read in an integer from the nonvolatile storage
 *
 * Params - offset - the offset into the EEPROM where data is stored
 *
 * Returns - int - the value read from the specified location
 */
int ReadInteger(int offset)
{
  int returnValue = 0;
  
  // Read in EEPROM
  for (int count = 0; count < sizeof(int); count++)
  {
    returnValue |= EEPROM.read(offset + count) << (count * 8); // read in a byte from eeprom and shift it x to build up an integer of data
  }
  
  return returnValue;
}

/*
 * WriteInteger - a helper function to write in an integer to the nonvolatile storage
 *
 * Params - offset - the offset into the EEPROM where data is to be stored
 *        - value - the value to be stored
 *
 * Returns - none
 */
void WriteInteger(int offset, int value)
{
  byte dataValue = 0;
  byte intSize = sizeof(int);
  
  for (int count = 0; count < intSize; count++)
  {
    dataValue = value >> ((intSize - 1 - count) * 8); // We need to shift from left to right so upper first then on down
    EEPROM.write(offset + count, dataValue);
  }
}

/*
 * ReadLong - a helper function to read in a long from the nonvolatile storage
 *
 * Params - offset - the offset into the EEPROM where data is stored
 *
 * Returns - long - the value read from the specified location
 */
long ReadLong(int offset)
{
  long value = 0;
  
  int storedValue = ReadInteger(offset);
  
  value = (storedValue << 16);
  
  storedValue = ReadInteger(offset + 2);
  
  value += storedValue;
  
  return value;
}

/*
 * WriteInteger - a helper function to write in a long to the nonvolatile storage
 *
 * Params - offset - the offset into the EEPROM where data is to be stored
 *        - value - the value to be stored
 *
 * Returns - none
 */
void WriteLong(int offset, long value)
{
  int upperValue = (value >> 16);
  int lowerValue = value && 0xFFFF;
  
  WriteInteger(offset, upperValue);
  WriteInteger(offset + 2, lowerValue);
}

/*
 * HandleDecoderTrigger - called when the encoder is turned
 *
 * Params - none
 *
 * Returns - nothing
 */
void HandleDecoderTrigger(void)
{
  byte directionData = 0;
  
  noInterrupts();
  
  directionData = digitalRead(directionPin);
  
  if (directionData == HIGH)
  {
    // clockwise
    if (StoredData.brightness < 256)
    {
      StoredData.brightness++;
    }
  }
  else
  {
    // counter clockwise
    if (StoredData.brightness > 0)
    {
      StoredData.brightness--;
    }
  }
  
  interrupts();
}

/*
 * Setup - main configuration point of our sketch to configure
 *         the Arduino for reading signals from a rotary encoder
 *         and setting the brightness of the LED based on the
 *         amount the rotary encoder is turned and which direction.
 *
 * Params - none
 *
 * Returns - nothing
 */
void setup()
{
  StoredData.signature = ReadLong(0); 
    
  if (StoredData.signature == signature)
  {
    // Have we been here before?
    StoredData.brightness = ReadInteger((char *)&StoredData.brightness - (char *)&StoredData.signature);
  }
  else
  {
    // Store signature and default brightness of 0
    WriteLong(0, signature);
    WriteInteger((char *)&StoredData.brightness - (char *)&StoredData.signature, StoredData.brightness);
  }
  
  pinMode(directionPin, INPUT);
    
  attachInterrupt(0, HandleDecoderTrigger, FALLING);  // trigger on falling edge
}

/*
 * loop - main method which will be called each time the
 *        Arduino goes through its main loop to change the state
 *        of the headlights and update the 7 segment displays
 *
 * Params - none
 *
 * Returns - nothing
 */
void loop()
{
  static int lastBrightness = 0;
  static int brightnessOffset = (char *)&StoredData.brightness - (char *)&StoredData.signature;
  
  if (lastBrightness != StoredData.brightness)
  {
    analogWrite(ledOutput, StoredData.brightness);
    WriteInteger(brightnessOffset, StoredData.brightness);
  }
  
  // Pause for 500 milliseconds i.e. 1/2 second.
  delay(500);
}

