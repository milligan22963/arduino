/*
 * Infrared RPM - the purpose of this sketch is to configure an interrupt to trigger
 *                when the infrared detector changes state.
 *
 * fanBlades is the number of blades that the fan being used has.  Each fan blade triggers
 *           the interrupt so to get an accurate reading we will need to divide the number
 *           we obtain by the number of blades.
 *
 * interruptId is the identifier of which interrupt we are using.  While the interrupt itself is attached to
 *             pin two (2) of the Arduino, the interrupt identifier is actually zero (0).
 *
 * updateFrequency is how often we update the display with our calculated RPM's
 *
 * speedCount is the number of times that the interrupt has been triggered since we last looked at.
 *
 * lastCycle is the value of millis() at the last cycle so we can accurately determine the speed based on a
 *           one second target
 */
const int fanBlades = 7;
const int interruptId = 0;
const int updateFrequency = 1000;

volatile int speedCount = 0; // volatile as it will be changed by both the interrupt and the main loop
unsigned long lastCycle = 0;

/*
 * Method declarations
 */
 
void infrared_trigger();


/*
 * infrared_trigger - this method is short as it is handled during an interrupt
 *                    it's sole purpose is to update the speed counter
 *
 *
 * Params - none
 *
 * Returns - nothing
 */ 
void infrared_trigger()
{
  speedCount++;
} 

/*
 * Setup - main configuration point of our sketch to configure
 *         the Arduino for serial port reporting, setting our
 *         default values, and enabling our interrupt.
 *
 * Params - none
 *
 * Returns - nothing
 */
void setup()
{
  // Setup the serial port to report our fan RPM value
  Serial.begin(9600);
  
  lastCycle = millis();  // get a starting value prior to turning the interrupt on
  
  // We will use the rising edge which correlates to the fan blade blocking the infrared detector
  attachInterrupt(interruptId, infrared_trigger, RISING);  
}

/*
 * loop - main method which will be called each time the
 *        Arduino goes through its main loop to check the
 *        amount of time since it last looked an if our
 *        update frequency has been exceeded, we will then
 *        report the rotations per minute scaled by 60
 *        since we are reporting the value every second.
 *
 * Params - none
 *
 * Returns - nothing
 */
void loop()
{
  /*
   * Grab the current millis to see if our target delta has been reached
   */
  unsigned long currentCycle = millis(); 
  unsigned long delta = currentCycle - lastCycle;
  
  // If we have passed our update frequency, update and start again
  if (delta > updateFrequency)
  {
    //--------------------------------------------
    // Have my seconds (in milliseconds)
    // convert to RPM's i.e. rotations per minute
    //
    // I am using floats to be more accurate
    //--------------------------------------------
    float variableRate = ((float)delta / 1000.0f) * 60;
    
    unsigned long currentSpeed = (speedCount * (variableRate)) / fanBlades;

    // Reset our speed count and make our current cycle the last cycle
    lastCycle = currentCycle;
    speedCount = 0;
    
    //--------------------------------------------
    // Write out the current speed once we have reset our values above
    // this is because the interrupt will be still running while we
    // are taking care of calculating the RPM's
    //--------------------------------------------
    Serial.print("Current speed is: ");
    Serial.print(currentSpeed);
    Serial.print("\n");
  }
}

