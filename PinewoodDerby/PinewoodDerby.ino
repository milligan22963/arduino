/*
 * PinewoodDerby - the purpose of this sketch is to control two 7 segment displays and two leds
 *                  which will be used as headlights.  The 7 segment display will be the number
 *                  of the pinewood derby car that it is attached to.
 *
 *  JP1 is three pins representing the data pin, the shift clock, and the storage clock
 *  JP2 is three pins representing the data pin, the shift clock, and the storage clock
 *  JP3 is two pins representing each of the LED headlights.
 *  Because we are using pins 2 and 3 as interrupts, we do not need to declare them prior to use
 */

/*
  JP1 - Pin1 - To D8
  JP1 - Pin2 - To D9
  JP1 - Pin3 - To D10
*/
const int dataPinA = 8;
const int shiftClockA = 9;
const int storageClockA = 10;

/*
  JP2 - Pin1 - To D5
  JP2 - Pin2 - To D6
  JP2 - Pin3 - To D7
*/
const int dataPinB = 5;
const int shiftClockB = 6;
const int storageClockB = 7;

/*
  JP3 - Pin1 - ledA
  JP3 - Pin1 - ledB
*/
const int ledA = 11;
const int ledB = 12;


/* Segments - to value representing each 7 segment display LED mapped to a binary value
  Decimal Point: 0x01
  Top:           0x02
  Upper Right:   0x04
  Lower Right:   0x08
  Bottom:        0x10
  Lower Left:    0x20
  Center:        0x40
  Upper Left:    0x80
*/

//--------------------------------------------------------------------------------------------
// The following are the hexidecimal value for each of the digits based on the segments above
//------------------------  0     1     2     3     4    5      6     7     8     9
const byte digits[10] = { 0xBE, 0x0C, 0x76, 0x5E, 0xCC, 0xDA, 0xFA, 0x0E, 0xFE, 0xDE};
const byte decimalPoint = 0x1; // defined however unused in this sketch

/*
 * Variables used by this sketch
 */
volatile int g_leftDigit = 3;  // the left digit of the display
volatile int g_rightDigit = 1; // the right digit of the display
boolean g_ledFlash = false;    // to track whether my led should be on or off


/*
 * Method declarations
 */
void togglePin(int pinId);
void writeValue(int value);
void HandleLeftDigitIncrement(void);
void HandleRightDigitIncrement(void);


/*
 * Setup - main configuration point of our sketch to configure
 *         the Arduino for writing data to our shift registers
 *         and to be able to turn the headlights on and off.
 *         In addition I will preset the shift clocks to a known
 *         state.
 *         Finally I will configure my interrupts to catch when
 *         the buttons indicate the numbers should be incremented.
 *
 * Params - none
 *
 * Returns - nothing
 */
void setup()
{
  pinMode(dataPinA, OUTPUT);
  pinMode(dataPinB, OUTPUT);
  pinMode(shiftClockA, OUTPUT);
  pinMode(shiftClockB, OUTPUT);
  pinMode(storageClockA, OUTPUT);
  pinMode(storageClockB, OUTPUT);
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  
  // Set the initial values of my shift and storage clocks
  digitalWrite(shiftClockA, LOW);
  digitalWrite(shiftClockB, LOW);
  digitalWrite(storageClockA, LOW);
  digitalWrite(storageClockB, LOW);
  
  attachInterrupt(0, HandleLeftDigitIncrement, RISING);
  attachInterrupt(1, HandleRightDigitIncrement, RISING);
}


/*
 * togglePin - toggles the passed in pin value from HIGH
 *              to LOW
 *
 * Params - pinId - representing the pin to toggle
 *
 * Returns - nothing
 */
void togglePin(int pinId)
{
  digitalWrite(pinId, HIGH);
  digitalWrite(pinId, LOW);
}

/*
 * writeValue - writes the passed in value to the
 *              the two 7 segment displays
 *
 * Params - value - which is the decimal value to write out encoded
 *                  using the digits array defined above.
 *
 * Returns - nothing
 */
void writeValue(int value)
{
  // Separate the value into two digits
  byte aValue = (value >> 8);
  byte bValue = value & 0xFF;
  
  // For every bit in each value i.e. aValue and bValue
  // write out HIGH if it is set i.e. a '1' and write out
  // LOW if it is not set i.e. a '0'
  for (int bitIndex = 0; bitIndex < 8; bitIndex++)
  {
    if (aValue & (1 << bitIndex))
    {
      digitalWrite(dataPinA, HIGH);
    }
    else
    {
      digitalWrite(dataPinA, LOW);
    }
    if (bValue & (1 << bitIndex))
    {
      digitalWrite(dataPinB, HIGH);
    }
    else
    {
      digitalWrite(dataPinB, LOW);
    }
    
    // Toggle the two clocks which shift the bits
    // down the shift register
    togglePin(shiftClockA);
    togglePin(shiftClockB);
  }
  
  // Finally toggle the storage clocks in order to show
  // the newly set digits.
  togglePin(storageClockA);
  togglePin(storageClockB);
}

/*
 * HandleLeftDigitIncrement - called when the button to
 *                            increment the left digit is
 *                            pressed.
 *
 * Params - none
 *
 * Returns - nothing
 */
void HandleLeftDigitIncrement(void)
{
  g_leftDigit++;     // Increment to the next digit
  g_leftDigit %= 10; // roll at 10 so we have 0 - 9 total
}

/*
 * HandleRightDigitIncrement - called when the button to
 *                             increment the right digit is
 *                             pressed.
 *
 * Params - none
 *
 * Returns - nothing
 */
void HandleRightDigitIncrement(void)
{
  g_rightDigit++;     // Increment to the next digit
  g_rightDigit %= 10; // roll at 10 so we have 0 - 9 total
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
  if (g_ledFlash)
  {
    digitalWrite(ledA, HIGH);
    digitalWrite(ledB, LOW);
  }
  else
  {
    digitalWrite(ledA, LOW);
    digitalWrite(ledB, HIGH);
  }
  
  //---------------------------------------------
  // If ledFlash is currently true
  // then set it to false otherwise make it true
  //---------------------------------------------
  g_ledFlash = g_ledFlash == false ? true : false;
  
  //---------------------------------------------
  // Concatenate two 8 bit values into one 16 bit
  // integer.  In order to ensure I knew what the
  // size of an integer was on this platfrom, I did
  // the following:
  // Serial.print("Size of int: ");
  // Serial.println(sizeof(int));
  //
  // I first shift the value 8 bits into the upper "byte"
  // of the integer and then "or" in the same value into
  // the lower 8 bits.  I could have also added the value
  // to the current value that was shifted resulting
  // in the same value to display however I perfer to
  // use the "or" so it is clear I am merging the two
  // values opposed to adding.
  //---------------------------------------------
  int valueToDisplay = (digits[g_leftDigit] << 8) | digits[g_rightDigit];
  writeValue(valueToDisplay);
    
  // Pause for 500 milliseconds i.e. 1/2 second.
  delay(500);
}

