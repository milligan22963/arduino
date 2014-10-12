/*
 *  Skeleton Sketch showing basic configuration and application processing
 *
 *  D. W. Milligan
 *
 *  Version: 1.0.0
 *
 *  February 5, 2013
 */
 
 /*
  * Setup - main configuration point of our sketch to configure
  *         the Arduino for operation
  *
  * Params - none
  *
  * Returns - nothing
  */
void setup()
{
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
}

/*
 * loop - main method which will be called each time the
 *        Arduino goes through its main loop
 *
  * Params - none
  *
  * Returns - nothing
 */
void loop()
{
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  Serial.println("Led 13 has been toggled on and off.");
}
