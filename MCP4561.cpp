#if ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>
#include "MCP4561.h"

/**************************************************************************/

  //<<constructor>> 
  MCP4561::MCP4561(char dev_address) // define the device adress for this instance
  {
  dev_ADDR= dev_address;
  };
  //<<destructor>>
  MCP4561::~MCP4561(){/*nothing to destruct*/};
  
uint8_t MCP4561::write(char mem_addr, uint16_t setValue) // mem_addr is 00-0F, setvalue is 0-257
{
//if you set the volatile output register, the same value is put to the non-volatile register
	if(setValue <0)
	{
	setValue =0;
	}
	if(setValue >257)
	{
	setValue =257;
	}
	if(mem_addr <0x00 || mem_addr >0x0F )
	{
	return 0;
	}

  byte cmd_byte = 0x00,data_byte = 0x00;   
  cmd_byte = ((mem_addr <<4) &B11110000) | (((setValue&0x01FF)>>8)&B00000011);  //  top 4 is now address   2 command   2 data (D9,D8)
  data_byte = lowByte(setValue);          // is now D7-D0
  Wire.beginTransmission(dev_ADDR);       // transmit to device this has the read byte ( LSB) set as 1
                                          // device address                 0101110x
  Wire.write(cmd_byte);                   // sends command byte             AAAACCDD
  Wire.write(data_byte);                  // sends potentiometer value byte DDDDDDDD  (D7-D0)
  Wire.endTransmission();                 // stop transmitting
  Wire.flush();
  if (mem_addr == WIPER_0_NON_VOLATILE || mem_addr == WIPER_1_NON_VOLATILE) {
	  delay(10); // EEPROM takes 5 - 10 ms to write ( datasheet page 12 )
  }
  else {
	  delay(2); // NV memory is faster
  }
  						  // give unit time to apply the value to non volatile register
  uint8_t set_reading = read(mem_addr);
  if (set_reading == setValue)
  {
  return 1; // it has accepted our setting ( EEPROM reflects what we set it to )
  }
  return 0;
}


uint16_t MCP4561::read(char mem_addr)// mem addr 0x00 - 0x0f ( 0-16 )
{
	if(mem_addr <0x00 || mem_addr >0x0F )
	{
	return 0x0FFF; // return something that is out of the expected bounds to signify an error
	}
   byte cmd_byte =0x0F, highbyte,lowbyte;
   cmd_byte = (mem_addr<<4) | B00001100 ; 
   
  Wire.beginTransmission(dev_ADDR);
  Wire.write(cmd_byte); 
  Wire.endTransmission(); 
  Wire.requestFrom(dev_ADDR, 2); 
  Wire.endTransmission();                 // stop transmitting
  if(Wire.available())
  {
  highbyte =  Wire.read();  //high byte
  lowbyte =   Wire.read();  //low byte
  Wire.flush();
  }
  unsigned int returnValue =0;
  returnValue = (((uint16_t)highbyte<<8)|lowbyte) & 0x01FF;
  return returnValue;
}

uint8_t MCP4561::openCircuit()// disconnect teh internal connectiuon in the pot, creating an open circuit
{
return write(4,0x0088);
//returns 1 if the operation was succesful, 0 otherwise
}

uint8_t MCP4561::enableOutput()// disconnect the internal connectiuon in the pot, creating an open circuit
{
return write(4,0x00FF);
//returns 1 if the operation was succesful, 0 otherwise
}
