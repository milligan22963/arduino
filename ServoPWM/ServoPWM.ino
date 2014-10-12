/*
 * ServoPWM - the purpose of this sketch is to control a servo in order to do some work.
 *            the led is used to indicate which direction the servo is going with it being
 *            off when the servo is going forward and on when it is going backward.
 *
 * The ledPin is used to drive the LED to turn it on or off.
 *
 * The servoPin is used to drive the servo motor.
 *
 */
const int ledPin = 8;
const int servoPin = 11;

/*
 *  The frequency of the arduino PWM signal is around 490 Hz
 *  This translates into 2 milliseconds per cycle.
 */
const int pwmFrequency = 490;

typedef enum
{
  FORWARD = 0,
  BACKWARD = 1
} eDirection;

eDirection servoDirection = FORWARD;

/*
 * Motor speeds for the LS-3006
 * which are defined in microseconds.
 */
typedef enum
{
  FORWARD_SPEED = 128,  // (1000 microseconds) which is equal to half of our cycle
  BACKWARD_SPEED = 250, // (2000) microseconds which is basically the full cycle
  NEUTRAL_SPEED = 192,  // (1500 microseconds) which is equal to three quarters of the cycle
} eMotorSpeed;

eMotorSpeed motorSpeed = FORWARD_SPEED;

/*
 * Method declarations
 */

/* No additional methods in this sketch */

/*
 * Setup - main configuration point of our sketch to configure
 *         the Arduino for PWM output
 *
 * Params - none
 *
 * Returns - nothing
 */
void setup()
{
  pinMode(ledPin, OUTPUT);  
}

/*
 * loop - main method which will be called each time the
 *        Arduino goes through its main loop to change the
 *        direction of the servo
 *
 * Params - none
 *
 * Returns - nothing
 */
void loop()
{
  // Set our LED to the proper state based on the direction
  if (servoDirection == FORWARD)
  {
    motorSpeed = FORWARD_SPEED;
    digitalWrite(ledPin, LOW);
    servoDirection = BACKWARD;  // set it for next time
  }
  else
  {
    motorSpeed = BACKWARD_SPEED;
    digitalWrite(ledPin, HIGH);
    servoDirection = FORWARD;  // set it for next time
  }
  
  analogWrite(servoPin, motorSpeed);
  
  // Rest for a second and then change directions
  delay(1000);
}

