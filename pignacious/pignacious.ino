/*
 * Pinewood derby pignacious
 */
 
// 8 bit pwm for each eye color
const int Red = 0;
const int Green = 1;
const int Blue = 2;

const int leftEye[] = {3, 5, 6};
const int rightEye[] = {9, 10, 11};

const int leftNostril = 2;
const int rightNostril = 4;
const int leftEar = 7;
const int rightEar = 8;

void setup()
{
  pinMode(leftNostril, OUTPUT);
  pinMode(rightNostril, OUTPUT);
  pinMode(leftEar, OUTPUT);
  pinMode(rightEar, OUTPUT);
}

enum eEyeState
{
  RED_EYE = 0,
  BLUE_EYE,
  GREEN_EYE,
  WHITE_EYE,
  SWIRL_COLOR,
  END_COLOR_EYE
};

byte redColor = 255;
byte blueColor = 255;
byte greenColor = 255;

int state = LOW;
eEyeState currentEyeState = RED_EYE;
int decrementAmount = 16;

void loop()
{
  int left = state == LOW ? HIGH : LOW;
  int right = state;
  bool resetColors = false;
  
  digitalWrite(leftNostril, left);
  digitalWrite(rightNostril, right);
  
  switch (currentEyeState)
  {
    case RED_EYE:
    {
      if (redColor > decrementAmount)
      {
        redColor -= decrementAmount;
      }
      else
      {
        redColor = 0;
        currentEyeState = BLUE_EYE;
      }
      blueColor = 255;
      greenColor = 255;      
    }
    break;
    case BLUE_EYE:
    {
      redColor = 255;
      if (blueColor > decrementAmount)
      {
        blueColor -= decrementAmount;
      }
      else
      {
        blueColor = 0;
        currentEyeState = GREEN_EYE;
      }
      greenColor = 255;
    }
    break;
    case GREEN_EYE:
    {
      redColor = 255;
      blueColor = 255;
      if (greenColor > decrementAmount)
      {
        greenColor -= decrementAmount;
      }
      else
      {
        greenColor = 0;
        currentEyeState = WHITE_EYE;
        resetColors = true;
      }
    }
    break;
    case WHITE_EYE:
    {
      if (redColor < decrementAmount)
      {
        currentEyeState = SWIRL_COLOR;
      }
      redColor -= decrementAmount;
      blueColor -= decrementAmount;
      greenColor -= decrementAmount;
    }
    break;
    case SWIRL_COLOR:
    {
      blueColor = 255;
      redColor = 255;
      greenColor = 255;

      analogWrite(leftEye[Red], redColor);
      analogWrite(leftEye[Green], greenColor);
      analogWrite(leftEye[Blue], blueColor);
    
      analogWrite(rightEye[Red], redColor);
      analogWrite(rightEye[Green], greenColor);
      analogWrite(rightEye[Blue], blueColor);
      delay(200);
      
      digitalWrite(leftEar, right);
      digitalWrite(rightEar, right);
      for (int swirl = 0; swirl < 12; swirl++)
      {
        digitalWrite(leftNostril, left);
        digitalWrite(rightNostril, left);
        for (int color = 0; color < 3; color++)
        {
          switch (color)
          {
            case 0:
            {
              analogWrite(leftEye[Red], 255 - (swirl * 15));
              analogWrite(rightEye[Red], 255 - (swirl * 15));
              analogWrite(leftEye[Green], greenColor);
              analogWrite(leftEye[Blue], blueColor);
              analogWrite(rightEye[Green], greenColor);
              analogWrite(rightEye[Blue], blueColor);
            }
            break;
            case 1:
            {
              analogWrite(leftEye[Green], 255 - (swirl * 15));
              analogWrite(rightEye[Green], 255 - (swirl * 15));
              analogWrite(leftEye[Red], redColor);
              analogWrite(leftEye[Blue], blueColor);
              analogWrite(rightEye[Red], redColor);
              analogWrite(rightEye[Blue], blueColor);
            }
            break;
            case 2:
            {
              analogWrite(leftEye[Blue], 255 - (swirl * 15));
              analogWrite(rightEye[Blue], 255 - (swirl * 15));
              analogWrite(leftEye[Green], greenColor);
              analogWrite(leftEye[Red], redColor);
              analogWrite(rightEye[Green], greenColor);
              analogWrite(rightEye[Red], redColor);
            }
            break;
          }
        
          
          delay(200);
        }
        digitalWrite(leftNostril, right);
        digitalWrite(rightNostril, right);
        delay(200);
      }
      resetColors = true;
      currentEyeState = RED_EYE;
    }
    break;
    default:
    {
      resetColors = true;
      currentEyeState = RED_EYE;
    }
  }
  analogWrite(leftEye[Red], redColor);
  analogWrite(leftEye[Green], greenColor);
  analogWrite(leftEye[Blue], blueColor);

  analogWrite(rightEye[Red], redColor);
  analogWrite(rightEye[Green], greenColor);
  analogWrite(rightEye[Blue], blueColor);
  
  digitalWrite(leftEar, right);
  digitalWrite(rightEar, left);
  
  state = left;
  
  if (resetColors == true)
  {
    blueColor = 255;
    redColor = 255;
    greenColor = 255;
  }
  delay(200);
}
