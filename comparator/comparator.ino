 /*
  * This circuit is designed to toggle between a HIGH signal and a LOW
  * based on the current temperature in the room.  The potentiometer
  * and the resistor are used to divide the 5v signal into two values
  * giving us the comparison we need to trigger the comparator output.
  * 2.73v is 0°C so to get the comparator to trigger we will want
  * to ensure that there is 2.73v + X across the potentiometer where
  * X is the temperature we want to toggle on.
  * The temperaturePin (now called setPointPin) will again read in the 
  * temperature however it will actually be the voltage across the 
  * potentiometer so we can use that as our setpoint.
  * The temperatureTogglePin will be used to determine if we are above
  * or below our target temperature.
  */
 const int setPointPin = 0;
 const int temperatureTogglePin = 7;
 const float baseVoltage = 2.730f; /* 2.730V at 0°C */
 const float voltageReference = 5.0f;
 const float maxSampleCount = 1023.0f;
 const float CtoFConversion = 9.0f / 5.0f;
 const float FtoCConversion = 5.0f / 9.0f;
 const float CtoFDelta = 32.0f;
 
 /*
  * Method declarations
  */
  float getTemperature(float temperatureVoltage);
  float convertTemperatureToF(float temperatureInCelsius);
  float convertTemperatureToC(float temperatureInFahrenheit);
 
 /*
  * getTemperature - determines the temperature in celsius based on the
  *         incoming voltage value assuming 2.73v equals 0°C
  *
  * Params - temperatureVoltage - the incoming voltage representing the temperature
  *
  * Returns - float - the actual temperature in celsius based on the incoming voltage
  */
 float getTemperature(float temperatureVoltage)
 {
   float currentTemperature = 0.0f;
  
   // From the datasheet, every 10 mv is equal to 1 °K - 0°C is 273°K equal to 2.73v
   // Temperature is scaled by 100 to bring it into the celsius scale from kelvin
   currentTemperature = (temperatureVoltage - baseVoltage) * 100;
  
   return currentTemperature;
 }

 /*
  * convertTemperatureToF - converts the tempertature from celsius to fahrenheit
  *
  * Params - temperatureInCelsius - the incoming temperature in celsius
  *
  * Returns - float - the temperature expressed in fahrenheit
  */
 float convertTemperatureToF(float temperatureInCelsius)
 {
   float convertedTemperature = 0.0f;
  
   convertedTemperature = (temperatureInCelsius * CtoFConversion) + CtoFDelta;
  
   return convertedTemperature;
 }

 /*
  * convertTemperatureToC - converts the tempertature from fahrenheit to celsius
  *
  * Params - temperatureInCelsius - the incoming temperature in fahrenheit
  *
  * Returns - float - the temperature expressed in celsius
  */
 float convertTemperatureToC(float temperatureInFahrenheit)
 {
   float convertedTemperature = 0.0f;
  
   convertedTemperature = (temperatureInFahrenheit - CtoFDelta) * FtoCConversion;
  
   return convertedTemperature;
 }

 /*
  * Setup - main configuration point of our sketch to configure
  *         the Arduino for analog input
  *
  * Params - none
  *
  * Returns - nothing
  */
 void setup()
 {
   pinMode(temperatureTogglePin, INPUT);
   analogReference(DEFAULT);
   Serial.begin(9600);
 }

 /*
  * loop - main method which will be called each time the
  *        Arduino goes through its main loop to read the
  *        setPoint temperature value abd check our digital
  *        input pin to see if it is high or not.
  *
  * Params - none
  *
  * Returns - nothing
  */
 void loop()
 {
   int    rawSetPointValue = 0;
   float  setPointVoltage = 0.0f;
   float  temperatureSetPoint = 0.0f;
  
   // Read in the temperature value which will be in a range from 0 to 1023
   rawSetPointValue = analogRead(setPointPin);
  
   // Convert our incoming voltage representation to a value we can utilize
   setPointVoltage = ((float)rawSetPointValue * voltageReference) / maxSampleCount;
  
   // We now have the current temperature expressed in celsius
   temperatureSetPoint = getTemperature(setPointVoltage);
  
   // Print out the current temperature in both C and F
   Serial.print("The temperature set point is: ");
   Serial.print(temperatureSetPoint);
   Serial.print("C which is ");
   Serial.print(convertTemperatureToF(temperatureSetPoint));
   Serial.println("F");

  if (digitalRead(temperatureTogglePin) == HIGH)
  {
    Serial.println("We have reached our set point.");
  }
  else
  {
    Serial.println("Set point has not been reached.");
  }
   // Rest for a second and check again
   delay(1000);
 }

