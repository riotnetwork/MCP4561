 #ifndef MCP4561_H
    #define MCP4561_H
   //  #include <Wire.h>
	 #if ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
		#include "WConstants.h"
	#endif
	
	
	class MCP4561
	{
	public:
			MCP4561(char dev_address);
            ~MCP4561();
			// declare all our functions here
			unsigned int read(char mem_addr);	 // mem addr 0x00 - 0x0f ( 0-16 ), returns 9 bit value at that adress
			int write(char mem_addr,int setValue); // mem_addr is 00-0F, setvalue is 0-257 returns 1if succesful
			int openCircuit();
			int enableOutput();
	private:
	char dev_ADDR;
	};
	#endif
