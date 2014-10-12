/*
 * The temperature pin is where the input from the LM335 will be read from.
 *
 * The redLedPin is the where the PWM output of the Arduino will connect to
 * the circuit.
 *
 * The greenLedPin is the where the PWM output of the Arduino will connect to
 * the circuit.
 *
 * The blueLedPin is the where the PWM output of the Arduino will connect to
 * the circuit.
 *
 * The base voltage is what the device (LM335) will measure at 0°C
 * The voltage reference is the max voltage that can be read in on the
 * analog input pin
 * The max sample count is the highest value that could be read in on the
 * analog input pin which will equal the voltage reference
 *
 */
const int temperaturePin = 0;
const int redLedPin = 9;
const int greenLedPin = 10;
const int blueLedPin = 11;
const float baseVoltage = 2.730f; /* 2.730V at 0°C */
const float voltageReference = 5.0f;
const int maxSampleCount = 1023;

/*
 * Method declarations
 */

/* No additional methods in this sketch */

/*
 * Setup - main configuration point of our sketch to configure
 *         the Arduino for analog input and PWM output
 *
 * Params - none
 *
 * Returns - nothing
 */
void setup()
{
  analogReference(DEFAULT);
  
  // Because we are using digital pins as analog outputs i.e. PWM
  // we do not need to configure them using pinMode.
}

/*
 * loop - main method which will be called each time the
 *        Arduino goes through its main loop to read the
 *        current temperature value and then turn on the
 *        three LED's based on the value of said temperature
 *
 * Params - none
 *
 * Returns - nothing
 */
void loop()
{
  int temperatureValue = 0;
  int red = 0;
  int green = 0;
  int blue = 0;
  float colorPercent = 0.0f;
  
  // Read in the temperature value which will be in a range from 0 to 1023
  temperatureValue = analogRead(temperaturePin);
  
  // The closer we are to 0, the bluer it will be
  colorPercent = 100.0f - (((float)temperatureValue / (float)maxSampleCount) * 100.0f);
  blue = (int)(colorPercent * 255.0f);
  
  // The closer to 1023 i.e. max of device, the redder it will be
  colorPercent = ((float)temperatureValue / (float)maxSampleCount) * 100.0f;
  red = (int)(colorPercent * 255.0f);
  
  // And green will vary accordingly peeking @ midpoint
  if (temperatureValue > (maxSampleCount / 2))
  {
    colorPercent = (float)(maxSampleCount - temperatureValue) * 100.0f;
  }
  else
  {
    colorPercent = ((float)temperatureValue / (float)(maxSampleCount / 2)) * 100.0f;
  }
  green = (int)(colorPercent * 255.0f);
  
  analogWrite(redLedPin, red);
  analogWrite(greenLedPin, green);
  analogWrite(blueLedPin, blue);
  
  // Rest for a second and check again
  delay(1000);
}

