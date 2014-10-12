/*
 * This sketch is designed to interface with SH1106 based OLED LCD over
 * an I2C interface
 *
 * This is a simple sketch to validate the library routines of the SH1106
 * controlled LCD display.
 *
 * Based in part off of code from: http://wenku.baidu.com/view/43ebc40aba1aa8114431d940.html
 */

// Pull in the wire library to communicate with the LCD over I2C
#include <Wire.h>
#include <sh1106.h>

/*
 * Variables used in this sketch
 */
sh1106_lcd *g_pLCD = NULL;

/*
 * Setup - main configuration point of our sketch to configure
 *         the Arduino for working with the SH1106 LCD module
 *
 * Params - none
 *
 * Returns - nothing
 */
void setup()
{  
  pinMode(13, OUTPUT);
  
  g_pLCD = sh1106_lcd::getInstance();

  if (g_pLCD != NULL)
  {
    g_pLCD->ClearScreen();
  }
}

/*
 * loop - main method which will be called when the Arduino goes
 *        through its main loop.  We will not return form this
 *        as we want to be able to see the results on the screen
 *
 * Params - none
 *
 * Returns - nothing
 */
void loop()
{
  digitalWrite(13, HIGH);
  // Draw a couple of rectangles, one with a 2px border, the other filled
  g_pLCD->DrawRectangle(0, 0, 100, 48, 2);
  g_pLCD->FillRectangle(25, 25, 30, 30);
  g_pLCD->Show();
  delay(500);
  
  // Print some lines of text out which can test out what we have
  // we will swap lower case for upper case to minimize memory usage
  // of our "font" set.
  g_pLCD->PrintLine("0123456789");
  g_pLCD->PrintLine("aBcDEF GHIJK");
  g_pLCD->PrintLine("LMNOP QRSTUV");
  g_pLCD->PrintLine("WXYZ");
  g_pLCD->PrintLine("AAAA");
  g_pLCD->PrintLine("EEEE");
  g_pLCD->PrintLine("FFFF");
  g_pLCD->PrintLine("GGGG");
  delay(250);
  
  // Extra line to see if the line scroll feature works
  g_pLCD->PrintLine("HHHH");
  
  // Draw a pixel in each corner of the screen
  g_pLCD->DrawPixel(0, 0, true);
  g_pLCD->DrawPixel(127, 0, true);
  g_pLCD->DrawPixel(127, 63, true);
  g_pLCD->DrawPixel(0, 63, true);
  g_pLCD->Show();
  delay(2000);
  

  // Clear the screen and draw a few lines in different directions to see
  // if the line drawing is working right
  g_pLCD->ClearScreen();
  g_pLCD->DrawLine(20, 31, 64, 31);
  g_pLCD->DrawLine(0, 0, 50, 50);
  g_pLCD->DrawLine(50, 50, 20, 20);
  g_pLCD->DrawLine(0, 50, 0, 20);
  
  g_pLCD->Show();

  // stop here so we can see what we did
  digitalWrite(13, LOW);
  delay(500);
}
