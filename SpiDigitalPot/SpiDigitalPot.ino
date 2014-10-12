/*
 * SpiDigitalPot - the purpose of this sketch is control the brightness of an LED
 *                 by configuring a digital potentiometer to set the brightness.
 *
 * potCS pin is the pin I am using as the chip select for the potentiometer.
 *
 * spi
 *    - spClk - is the pin that the SPI clock is configured for
 *    - spiMosi - is the pin that the master device (Arduino) transmits data to the slave device
 *    - spiMiso - is the pin that the slave device sends data to the master device (Arduino)
 *    - spiCS - is the chip select for the SPI circuit in the Arduino
 *            - I could use the spiCS chip select also for my POT given I only
 *              I only have one SPI device in use at this time.  Various shields will
 *              use specific pins as the chip select which most likely will not be pin 10
 *
 * tested with MCP4261 digital potentiometer from Microchip
 */
 
// include the SPI library:
#include <SPI.h>

/* constant defines */
const int potCS = 6;
const int spiClk = 13;
const int spiMosi = 11;
const int spiMiso = 12;
const int spiCS = 10;

/* Variables for this sketch */
byte g_potValue = 0;

/*
 * Data structures to send commands to
 * the digital potentiometer
 */
 
 /* The command Byte is the main address/data bits to send to the pot */
typedef struct _CommandByte
{
  byte Address:4;  /* the address to select */
  byte Command:2;  /* the actual command to send */
  byte DataBits:2; /* the upper data bits */
} CommandByte;

/* The combination command byte with data to send */
typedef struct _CommandData
{
  union
  {
    CommandByte cmd;  /* The command byte as defined above */
    byte cmdByte;     /* The cmd byte in standard byte format for easy access */
  } command;
  byte        data;  /* The data to be sent with the command as needed. */
} CommandData;

/*
 * Method declarations
 */
void UpdatePot(int selectPin, struct _CommandData *pData);
int ReadPot(int selectPin, struct _CommandData *pData);

/*
 * UpdatePot - sends commands and data to the potentiometer
 *             which is activated by the specified chip select
 *
 * Params
 *  selectPin - the pin to use as the select pin
 *  pData     - pointer to the command data stucture to be sent
 *
 * Return
 *  none
 */
void UpdatePot(int selectPin, struct _CommandData *pData)
{  
  digitalWrite(selectPin, LOW);
  
  delay(100); // required delay
  
  SPI.transfer(pData->command.cmdByte);
  SPI.transfer(pData->data);
  
  digitalWrite(selectPin, HIGH);
}


/*
 * ReadPot   - sends commands and data to the potentiometer
 *             which is activated by the specified chip select
 *
 * Params
 *  selectPin - the pin to use as the select pin
 *  pData     - pointer to the command data stucture to be sent
 *
 * Return
 *  int       - the value read back from the pot
 */
int ReadPot(int selectPin, struct _CommandData *pData)
{
  int dataRead = 0;
  
  digitalWrite(selectPin, LOW);
  delay(100);
  
  dataRead = SPI.transfer(pData->command.cmdByte) << 8;
  dataRead |= SPI.transfer(pData->data);
  digitalWrite(selectPin, HIGH);
  
  dataRead &= 0x1FF;
  
  return dataRead;
}

/*
 * Enumerations for clarity of code
 */

/*
 *  This enum is a definition of each address that can
 *  be accessed on the potentiometer
 */
typedef enum
{
  Wiper0 = 0,
  Wiper1,
  NVWiper0,
  NVWiper1,
  TConReg,
  StatusReg,
  Data0,
  Data1,
  Data2,
  Data3,
  Data4,
  Data5,
  Data6,
  Data7,
  Data8,
  Data9,
} Addresses;

/*
 *  This enum is a definition of each command that can
 *  be sent to the potentiometer
 */
typedef enum
{
  WriteData = 0,
  Increment,
  Decrement,
  ReadData
} Commands;

/*
 * Setup - main configuration point of our sketch to configure
 *         the Arduino for SPI communications with the potentiometer
 *
 * Params - none
 *
 * Returns - nothing
 */
void setup()
{
  Serial.begin(9600);
  
  pinMode(spiCS, OUTPUT);
  pinMode(potCS, OUTPUT);
  
  digitalWrite(potCS, HIGH);
  
  /* Configure SPI */
  SPI.begin();
  
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  
  // Configure pot outputs
  CommandData command;

  command.command.cmd.Command = WriteData;
  command.command.cmd.Address = TConReg;
  command.command.cmd.DataBits = 0;
  command.data = 0xFF;
  
  // Configure the pot to enable each output
  UpdatePot(potCS, &command);
  
  command.command.cmd.Command = ReadData;
  
  // Read back in the tcon register
  int tconValue = ReadPot(potCS, &command);

  Serial.print("TCON: ");
  Serial.println(tconValue);
  Serial.println("Setup completed");
}

/*
 * loop - main method which will be called each time the
 *        Arduino goes through its main loop which will set
 *        the value of the digital potentiometer which
 *        will vary the intensity of the LED based on the
 *        value written to the potentiometer.  The larger
 *        the value written to the potentiometer, the dimmer
 *        the LED as the transistor saturation is lower.
 *
 * Params - none
 *
 * Returns - nothing
 *
 * Note:  The pot is 9 bits but we are only using the lower 8
 */
void loop()
{
  CommandData command;
  
  command.command.cmd.Command = WriteData;
  command.command.cmd.Address = Wiper0;
  command.command.cmd.DataBits = 0; // clear the upper bit(s)
  command.data = g_potValue;

  // Send the command to the pot updating the value
  UpdatePot(potCS, &command);

  // Note the current value that was sent to the pot
  Serial.println(g_potValue);

  // Delay 250 ms so we can see the changes
  delay(250);    

  // let it roll
  g_potValue += 25;
}
