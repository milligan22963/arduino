/*
 * Power Indicator - the purpose of this script/circuit is to visually display
 *                   the health of the system via a tricolor RGB LED.
 *
 * The redPin will be used to control the red portion of the LED.
 *     Note: this pin will act different then the other two as
 *           we will be turning this pin high to turn off the LED.
 *           To make the code cleaner, this change is only
 *           handled in the LightLed routine.
 * The greenPin will be used to control the green portion of the LED.
 * The bluePin will be used to control the blue portion of the LED.
 *
 * For this circuit, there will not be any additional inputs or outputs as this circuit is
 * intended as the basis for a project opposed to a complete project.
 */

/*
 * Enumeration of values to turn on/off the 3 channels of the tricolor LED
 */
typedef enum
{
  RED_ON = 0x1,
  GREEN_ON = 0x2,
  BLUE_ON = 0x4
} eLedSettings;

/*
 * Constants used in this sketch
 */
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

const int POWER_OFF = RED_ON;
const int POWER_ON = GREEN_ON;
const int SETUP = GREEN_ON | RED_ON;  // Turn both red and green on to get yellow
const int BLANK = 0; // no lED's lit

/*
 * Method declarations
 */
void LightLed(int colors);

/*
 *  LightLed
 *
 * This method checks the passed in value to determine which parts of the LED should be on and
 * which ones shouldn't be.  Only the lower 3 bits are used to indicate the colors starting with
 * red as the LSB (least significant bit) and blue the MSB (most significant bit)
 *
 * Params - colors - a set of bits (bit mask) indicating which LED colors should be on and
 *                   which ones should not be.
 *
 * Returns - none
 */
void LightLed(int colors)
{
  /*
   *  Red acts differently where a HIGH output turns off the LED and a LOW output
   *  turns it on.
   */
  if (colors & RED_ON)
  {
    digitalWrite(redPin, LOW);
  }
  else
  {
    digitalWrite(redPin, HIGH);
  }
  
  if (colors & GREEN_ON)
  {
    digitalWrite(greenPin, HIGH);
  }
  else
  {
    digitalWrite(greenPin, LOW);
  }
  
  if (colors & BLUE_ON)
  {
    digitalWrite(bluePin, HIGH);
  }
  else
  {
    digitalWrite(bluePin, LOW);
  }
}

/*
 * Setup - main configuration point of our sketch to configure
 *         the Arduino for writing digital values out on the defined pins
 *
 * Params - none
 *
 * Returns - nothing
 */
void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  LightLed(SETUP);
  delay(1000);
}

/*
 * loop - main method which will be called each time the
 *        Arduino goes through its main loop to turn the
 *        LED on to green and then off blinking every
 *        half second
 *
 * Params - none
 *
 * Returns - nothing
 */
void loop()
{
  LightLed(POWER_ON);
  delay(500);

  LightLed(BLANK);
  delay(500);
}
