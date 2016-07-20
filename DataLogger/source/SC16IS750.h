/* 
Description:
This is a example code for Sandbox Electronics' I2C/SPI to UART bridge module.
You can get one of those products on 
http://sandboxelectronics.com

Version:
V0.1

Release Date:
2014-02-16

Author:
Tiequan Shao          info@sandboxelectronics.com

Lisence:
CC BY-NC-SA 3.0

Please keep the above information when you use this code in your project. 
*/

//#include "MicroBitTouchDevelop.h"
#include "MicroBit.h"
#include "global.h"
#include "I2CCommon.h"

#ifndef _SC16IS750_H_
#define _SC16IS750_H_

namespace sacsik {
namespace SC16IS750 {
    

//Device Address

//A:VDD
//B:GND
//C:SCL
//D:SDA
enum
{
  SC16IS750_ADDRESS_AA     =0X90,
  SC16IS750_ADDRESS_AB     =0X92,
  SC16IS750_ADDRESS_AC     =0X94,
  SC16IS750_ADDRESS_AD     =0X96,
  SC16IS750_ADDRESS_BA     =0X98,
  SC16IS750_ADDRESS_BB     =0X9A,
  SC16IS750_ADDRESS_BC     =0X9C,
  SC16IS750_ADDRESS_BD     =0X9E,
  SC16IS750_ADDRESS_CA     =0XA0,
  SC16IS750_ADDRESS_CB     =0XA2,
  SC16IS750_ADDRESS_CC     =0XA4,
  SC16IS750_ADDRESS_CD     =0XA6,
  SC16IS750_ADDRESS_DA     =0XA8,
  SC16IS750_ADDRESS_DB     =0XAA,
  SC16IS750_ADDRESS_DC     =0XAC,
  SC16IS750_ADDRESS_DD     =0XAE,


//General Registers
  SC16IS750_REG_RHR        =0x00,
  SC16IS750_REG_THR        =0X00,
  SC16IS750_REG_IER        =0X01,
  SC16IS750_REG_FCR        =0X02,
  SC16IS750_REG_IIR        =0X02,
  SC16IS750_REG_LCR        =0X03,
  SC16IS750_REG_MCR        =0X04,
  SC16IS750_REG_LSR        =0X05,
  SC16IS750_REG_MSR        =0X06,
  SC16IS750_REG_SPR        =0X07,
  SC16IS750_REG_TCR        =0X06,
  SC16IS750_REG_TLR        =0X07,
  SC16IS750_REG_TXLVL      =0X08,
  SC16IS750_REG_RXLVL      =0X09,
  SC16IS750_REG_IODIR      =0X0A,
  SC16IS750_REG_IOSTATE    =0X0B,
  SC16IS750_REG_IOINTENA   =0X0C,
  SC16IS750_REG_IOCONTROL  =0X0E,
  SC16IS750_REG_EFCR       =0X0F,

//Special Registers
  SC16IS750_REG_DLL        =0x00,
  SC16IS750_REG_DLH        =0X01,

//Enhanced Registers
  SC16IS750_REG_EFR        =0X02,
  SC16IS750_REG_XON1       =0X04,
  SC16IS750_REG_XON2       =0X05,
  SC16IS750_REG_XOFF1      =0X06,
  SC16IS750_REG_XOFF2      =0X07,

//
  SC16IS750_INT_CTS        =0X80,
  SC16IS750_INT_RTS        =0X40,
  SC16IS750_INT_XOFF       =0X20,
  SC16IS750_INT_SLEEP      =0X10,
  SC16IS750_INT_MODEM      =0X08,
  SC16IS750_INT_LINE       =0X04,
  SC16IS750_INT_THR        =0X02,
  SC16IS750_INT_RHR        =0X01,

//Application Related 

  SC16IS750_CRYSTCAL_FREQ  =14745600UL, 
//#define 	SC16IS750_CRYSTCAL_FREQ (1843200UL)	  
//#define     SC16IS750_CRYSTCAL_FREQ (16000000UL)    
//#define     SC16IS750_DEBUG_PRINT   (0)
  SC16IS750_PROTOCOL_I2C  =0,
  SC16IS750_PROTOCOL_SPI  =1
};
  class SC16IS750 {
    public:
        SC16IS750(uint8_t protocol, int addr_sspin);
        void begin(uint32_t baud);                               
        int read();
        void write(uint8_t val);
        uint8_t ReadRegister(uint8_t reg_addr);
        void WriteRegister(uint8_t reg_addr, uint8_t val);
        int available();
    //    void pinMode(uint8_t pin, uint8_t io);
    //    void digitalWrite(uint8_t pin, uint8_t value);
    //    uint8_t digitalRead(uint8_t pin);
		uint8_t ping();
	//	void setTimeout(uint32_t);
	//	size_t readBytes(char *buffer, size_t length);
		int peek();
		void flush();
      
    
        uint8_t device_address_sspin;
        uint8_t protocol;
	//	uint32_t timeout;
        int16_t SetBaudrate(uint32_t baudrate);

        void    SetLine(uint8_t data_length, uint8_t parity_select, uint8_t stop_length );

        void    ResetDevice(void);
        
     
        void    __isr(void);
        void    FIFOEnable(uint8_t fifo_enable);
        void    FIFOReset(uint8_t rx_fifo);
        void    FIFOSetTriggerLevel(uint8_t rx_fifo, uint8_t length);
        uint8_t FIFOAvailableData(void);
        uint8_t FIFOAvailableSpace(void);
        void    WriteByte(uint8_t val);
        int     ReadByte(void);
        void    EnableTransmit(uint8_t tx_enable);
	//	int16_t readwithtimeout();
		int 	peek_buf;
		uint8_t peek_flag;

        char addr;
  };
  
} // namespace SC16IS750
} // namespace touch_develop

#endif





    
    
