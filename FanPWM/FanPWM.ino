/*
 * FanPWM - the purpose of this sketch is to read in a value from a potentiometer and then
 *          use that value to determine how fast the fan should be spinning.
 *
 * The fan control pin is where the input from the potentiometer will be read from.
 *
 * fanTachPin is connected to read in the current speed of the fan
 *
 * The fan speed pin is the where the PWM output of the Arduino will connect to
 * the interface circuit in order to drive the fan.
 *
 * The voltage reference is the max voltage that can be read in on the
 * analog input pin
 *
 * The max sample count is the highest value that could be read in on the
 * analog input pin which will equal the voltage reference
 */
const int fanControlPin = 0;
const int fanTachPin = 7;
const int fanSpeedPin = 9;
const float voltageReference = 5.0f;
const int maxSampleCount = 1023;

/*
 * Method declarations
 */

/* No additional methods in this sketch */

/*
 * Setup - main configuration point of our sketch to configure
 *         the Arduino for analog input and PWM output
 *         also set up the tach pin for input along with
 *         enabling the serial port to monitor fan speed
 *
 * Params - none
 *
 * Returns - nothing
 */
void setup()
{
  analogReference(DEFAULT);
  
  pinMode(fanTachPin, INPUT);
  
  Serial.begin(9600);
}

/*
 * loop - main method which will be called each time the
 *        Arduino goes through its main loop to read the
 *        current fan control value and then turn on the
 *        fan based on the voltage across the potentiometer
 *
 * Params - none
 *
 * Returns - nothing
 */
void loop()
{
  int fanControlValue = 0;
  int fanSpeed = 0;
  int fanTach = 0;
  
  // Read in the temperature value which will be in a range from 0 to 1023
  fanControlValue = analogRead(fanControlPin);

  // Our range for output for PWM is 0 to 255 so we will divide by
  // 4 in order to get the read in value into the proper range
  // the output pin expects.
  fanSpeed = fanControlValue / 4;
  
  analogWrite(fanSpeedPin, fanSpeed);

  // Read in the fan tachometer value which will be a high signal
  // for a period X
  fanTach = pulseIn(fanTachPin, HIGH);
    
  char buffer[256];
  int rpm = 1000000 / fanTach; // 1 Million as the value is in microseconds
  
  // Target and tach shouldn't be expected to be the same
  sprintf(buffer, "Target: %d, Tach: %d", fanSpeed, rpm);
  
  Serial.println(buffer);
  
  // Rest for a second and check again
  delay(1000);
}

