/*************************************************** 
  This is an example for the Microchip MCP4561 8 bit digital pot

  These displays use I2C/TWI to communicate,
  2 pins are required to interface

  Written by Daniel de kock
  BSD license, all text above must be included in any redistribution
 ****************************************************/
 

 //---------------------------------------------------------------------------------------------------------
// Functions and returns
//---------------------------------------------------------------------------------------------------------
/*
#include <MCP4561.h>
#include <Wire.h>

MCP4561  digiPot(46);                                            //declare an instance of MAX4561 with Device addr 46 ( 0101 110 ), ie A0 pin pulled low

 Wire.begin();                                                   // begin IIC communication
 
 unsigned int read(char mem_addr);				 // mem addr 0x00 - 0x0f ( 0-16 ), returns 9 bit value at that adress ( 0- 257 )
 int write(char mem_addr,int setValue);                          // mem_addr is 00-0F, setvalue is 0-257 returns 1 if succesful and 0 otherwise, tthis will set the volatile and nonvolatile values
 int openCircuit();                                              // creates and open circuit where the pot is ( disconnects the internals) , returns 1 if sucess, 0 otherwise
 int enableOutput();                                             // enables the pot output after an open circuit, returns 1 if sucess, 0 otherwise
*/

//---------------------------------------------------------------------------------------------------------
// MEMORY adresses 
//---------------------------------------------------------------------------------------------------------
 /* 
 ADDR    |FUNCTION    |CMDs
 00      |VOLATILE WP 0        |R,W,Inc,Decr      Wiper 0 value
 01      |VOLATILE WP 1        |R,W,Inc,Decr      ** Wiper 1 value dual output version only
 02      |NonVOLATILE WP 0     |R,W,HVInc,HVDecr  Wiper 0 value
 03      |NonVOLATILE WP 1     |R,W,HVInc,HVDecr  ** Wiper 1 value dual output version only
 04      |VOLATILE TCON        |R,W
 05      |STATUS               |R
 06 - 0E |EEPROM               |R,W
 0F      |EEPROM               |R,W,HV_WPD,HV_RPD
 
 R     :  Read-able
 W     :  Write-able
 Inc   : increment value ( check datasheet for data value to be sent to pot to use this function)
 Decr  : Decrement value ( check datasheet for data value to be sent to pot to use this function)
 
 */
 
//---------------------------------------------------------------------------------------------------------
// External includes
//---------------------------------------------------------------------------------------------------------

#include <MCP4561.h>
#include <Wire.h>


MCP4561 digiPot(46);                              //declare an instance of MAX4561 with Device addr 46 ( 0101 110 ), ie, A0 pin lulled low

//---------------------------------------------------------------------------------------------------------
// Setup loop
//---------------------------------------------------------------------------------------------------------

void setup()
{
  Wire.begin();                                   // join i2c bus as master
  Serial.begin(19200);                            // used for serial control
  displayMenu();                                  // display a UI on serial
} 

//---------------------------------------------------------------------------------------------------------
// Main loop
//---------------------------------------------------------------------------------------------------------

void loop()
{
  int c;

  if (Serial.available() > 0)
  {
    c= Serial.read();
  }

  delay(5);
  switch(c)
  {
  case 'r':
    char addr;
    unsigned int readValue;
    addr = Serial.parseInt();
    readValue = digiPot.read(addr);                 // this is the example method, all the rest is formatting, it will return the unsigned value of the requested memory adress, 4096 if the memory does not exist
    Serial.print(" MCP4561 memory at 0x0");
    Serial.print(addr,HEX);
    Serial.print(" : ");
    Serial.println(readValue);
    Serial.flush();
    break;

  case 's':
  int setOutput;
  setOutput = Serial.parseInt();
  int success;
  success = digiPot.write(0,setOutput);             // this is the example method, all the rest is formatting, this method will return 1 if the write was succesful, 0 otherwise
  
  success = digiPot.write(2,setOutput);
    if (success)             
   {
     Serial.print(" MCP4561 set to : ");
     Serial.println(setOutput);
   } 
   else{Serial.println(" MCP4561 setting unsuccesful ");}  
    Serial.flush();
    break;

  case 'm':
    displayMenu();
    Serial.flush();
    break;
    case 'd':
    success = digiPot.openCircuit();
    if (success)             
   {
     Serial.println(" MCP4561 output open circuit");
   } 
    break;
    case 'e':
    success digiPot.enableOutput();
    if (success)             
   {
     int output;
     output = digiPot.read(0); 
     Serial.print(" MCP4561 output to  : ");
     Serial.println(output);
   } 
    break;
  }
}



//---------------------------------------------------------------------------------------------------------
// Serial menu display function
//---------------------------------------------------------------------------------------------------------


void displayMenu()
{
  Serial.println( "Microchip MCP4561 library example");
  Serial.println( "m     : Menu");
  Serial.println( "rxx   : read memory at xx ( 0-16 )");
  Serial.println( "sXXX  : set output 0 to 0-257");
  Serial.println( "d     : open circuit output");
  Serial.println( "e     : enable output");
  Serial.println( "----------------------------------");
}
