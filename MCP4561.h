 #ifndef __MCP4561_H
    #define __MCP4561_H
   //  #include <Wire.h>
	 #if ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
		#include "WConstants.h"
	#endif
	
   /*
   ADDR is 0101 110x ( for the MCP 4561, 0101 aaax for ones with more than 1 adress pin)


   x is the R/!W bit set by the wire library

   COMMAND char :  AAAA CC DD
   AAAA        : Adress in memory to do things
   CC     : What to do at AAAA ( R,W,Inc,Decr )
   00      : Write
   01      : increment (NV only)
   10      : Decrement (NV only)
   11      : Read
   DD  : D9 and D8 ( MSB of the value we are sending)


   Datachar     : DDDDDDDD ( 0-255 : 00-FF ), this gets used with D8 to give a full range of 00-257 : 0x00-0x100
   8 bit data ( 8 LSB of the 10 bit data, 7 or 8 bit devices will only use the lower 7 or 8 bits )
   memory map
   Data memory is only 8 bits wide

   ADDR    |FUNCTION    |CMDs
   00      |VOL WP 0    |R,W,Inc,Decr
   01      |VOL WP 1    |R,W,Inc,Decr
   02      |NV WP 0     |R,W,HVInc,HVDecr
   03      |NV WP 1     |R,W,HVInc,HVDecr
   04      |VOL TCON    |R,W
   05      |STATUS      |R
   06 - 0E |EEPROM      |R,W
   0F      |EEPROM      |R,W,HV_WPD,HV_RPD


   to send/recieve something:
   IIC addr + RW : 0101 110x

   COMMAND       : AAAA CCDD
   DATA          : DDDD DDDD * only read and write commands have data

   datasheet : http://ww1.microchip.com/downloads/en/DeviceDoc/22107B.pdf
   */

#define WIPER_0_VOLATILE 00
#define WIPER_1_VOLATILE 01
#define WIPER_0_NON_VOLATILE 02
#define WIPER_1_NON_VOLATILE 03

	class MCP4561
	{
	public:
			MCP4561(char dev_address);
            ~MCP4561();
			// declare all our functions here
			uint16_t read(char mem_addr);	 // mem addr 0x00 - 0x0f ( 0-16 ), returns 9 bit value at that adress
			uint8_t write(char mem_addr, uint16_t setValue); // mem_addr is 00-0F, setvalue is 0-257 returns 1 if succesful, 0 otherwise
			uint8_t openCircuit();
			uint8_t enableOutput();
	private:
	char dev_ADDR;
	};


	#endif //__MCP4561_H
