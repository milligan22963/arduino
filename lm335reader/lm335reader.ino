 /*
  * The temperature pin is where the input from the LM335 will be read from.
  * The base voltage is what the device (LM335) will measure at 0°C
  * The voltage reference is the max voltage that can be read in on the
  * analog input pin
  * The max sample count is the highest value that could be read in on the
  * analog input pin which will equal the voltage reference
  * The c to f conversion is the well known formula of multiplying the
  * C temp by 9 then dividing by 5
  * The f to c conversion is the well known formula of multiplying the
  * F temp by 5 then dividing by 9
  * The c to f delta is the difference between C and F at the freezing temperature
  */
 const int temperaturePin = 0;
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
   analogReference(DEFAULT);
   Serial.begin(9600);
 }

 /*
  * loop - main method which will be called each time the
  *        Arduino goes through its main loop to read the
  *        current temperature value
  *
  * Params - none
  *
  * Returns - nothing
  */
 void loop()
 {
   int    temperatureValue = 0;
   float  currentTempVoltage = 0.0f;
   float  currentTemperature = 0.0f;
  
   // Read in the temperature value which will be in a range from 0 to 1023
   temperatureValue = analogRead(temperaturePin);
  
   // Convert our incoming voltage representation to a value we can utilize
   currentTempVoltage = ((float)temperatureValue * voltageReference) / maxSampleCount;
  
   // We now have the current temperature expressed in celsius
   currentTemperature = getTemperature(currentTempVoltage);
  
   // Print out the current temperature in both C and F
   Serial.print("The current temperature is: ");
   Serial.print(currentTemperature);
   Serial.print("C which is ");
   Serial.print(convertTemperatureToF(currentTemperature));
   Serial.println("F");

   // Rest for a second and check again
   delay(1000);
 }

