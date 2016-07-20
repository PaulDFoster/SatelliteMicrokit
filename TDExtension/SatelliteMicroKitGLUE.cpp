//#include "BitVM.h"
//#include "MicroBit.h"
//#include "MicroBitTouchDevelop.h"
#include "I2CCommon.h"


namespace SatelliteMicrokit2
{
    static const char empty[] __attribute__ ((aligned (4))) = "\xff\xff\0\0\0";

    using namespace ::touch_develop::i2c;
    using namespace ::touch_develop;
    
// *****************************************************************************
// Satellite Microkit Light Sensor
// *****************************************************************************
/*************************************************** 
  Modified for the SI1146 and Touch Develop for the Microbit
  by Paul Foster @paulfo
  
  This is a library for the Si1145 UV/IR/Visible Light Sensor

  Designed specifically to work with the Si1145 sensor in the
  adafruit shop
  ----> https://www.adafruit.com/products/1777

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
    enum
    {
        /* COMMANDS */
         SI1145_PARAM_QUERY     =0x80,
         SI1145_PARAM_SET       =0xA0,
         SI1145_NOP             =0x0,
         SI1145_RESET           =0x01,
         SI1145_BUSADDR         =0x02,
         SI1145_PS_FORCE        =0x05,
         SI1145_ALS_FORCE       =0x06,
         SI1145_PSALS_FORCE     =0x07,
         SI1145_PS_PAUSE        =0x09,
         SI1145_ALS_PAUSE       =0x0A,
         SI1145_PSALS_PAUSE     =0xB,
         SI1145_PS_AUTO         =0x0D,
         SI1145_ALS_AUTO        =0x0E,
         SI1145_PSALS_AUTO      =0x0F,
         SI1145_GET_CAL         =0x12,

        /* Parameters */
        SI1145_PARAM_I2CADDR            =0x00,
        SI1145_PARAM_CHLIST             =0x01,
        SI1145_PARAM_CHLIST_ENUV        =0x80,
        SI1145_PARAM_CHLIST_ENAUX       =0x40,
        SI1145_PARAM_CHLIST_ENALSIR     =0x20,
        SI1145_PARAM_CHLIST_ENALSVIS    =0x10,
        SI1145_PARAM_CHLIST_ENPS1       =0x01,
        SI1145_PARAM_CHLIST_ENPS2       =0x02,
        SI1145_PARAM_CHLIST_ENPS3       =0x04,

        SI1145_PARAM_PSLED12SEL         =0x02,
        SI1145_PARAM_PSLED12SEL_PS2NONE =0x00,
        SI1145_PARAM_PSLED12SEL_PS2LED1 =0x10,
        SI1145_PARAM_PSLED12SEL_PS2LED2 =0x20,
        SI1145_PARAM_PSLED12SEL_PS2LED3 =0x40,
        SI1145_PARAM_PSLED12SEL_PS1NONE =0x00,
        SI1145_PARAM_PSLED12SEL_PS1LED1 =0x01,
        SI1145_PARAM_PSLED12SEL_PS1LED2 =0x02,
        SI1145_PARAM_PSLED12SEL_PS1LED3 =0x04,

        SI1145_PARAM_PSLED3SEL          =0x03,
        SI1145_PARAM_PSENCODE           =0x05,
        SI1145_PARAM_ALSENCODE          =0x06,

        SI1145_PARAM_PS1ADCMUX          =0x07,
        SI1145_PARAM_PS2ADCMUX          =0x08,
        SI1145_PARAM_PS3ADCMUX          =0x09,
        SI1145_PARAM_PSADCOUNTER        =0x0A,
        SI1145_PARAM_PSADCGAIN          =0x0B,
        SI1145_PARAM_PSADCMISC          =0x0C,
        SI1145_PARAM_PSADCMISC_RANGE    =0x20,
        SI1145_PARAM_PSADCMISC_PSMODE   =0x04,

        SI1145_PARAM_ALSIRADCMUX        =0x0E,
        SI1145_PARAM_AUXADCMUX          =0x0F,

        SI1145_PARAM_ALSVISADCOUNTER    =0x10,
        SI1145_PARAM_ALSVISADCGAIN      =0x11,
        SI1145_PARAM_ALSVISADCMISC      =0x12,
        SI1145_PARAM_ALSVISADCMISC_VISRANGE =0x20,

        SI1145_PARAM_ALSIRADCOUNTER     =0x1D,
        SI1145_PARAM_ALSIRADCGAIN       =0x1E,
        SI1145_PARAM_ALSIRADCMISC       =0x1F,
        SI1145_PARAM_ALSIRADCMISC_RANGE =0x20,

        SI1145_PARAM_ADCCOUNTER_511CLK  =0x70,

        SI1145_PARAM_ADCMUX_SMALLIR     =0x00,
        SI1145_PARAM_ADCMUX_LARGEIR     =0x03,



        /* REGISTERS */
        SI1145_REG_PARTID       =0x00,
        SI1145_REG_REVID        =0x01,
        SI1145_REG_SEQID        =0x02,

        SI1145_REG_INTCFG       =0x03,
        SI1145_REG_INTCFG_INTOE         =0x01,
        SI1145_REG_INTCFG_INTMODE       =0x02,

        SI1145_REG_IRQEN                =0x04,
        SI1145_REG_IRQEN_ALSEVERYSAMPLE =0x01,
        SI1145_REG_IRQEN_PS1EVERYSAMPLE =0x04,
        SI1145_REG_IRQEN_PS2EVERYSAMPLE =0x08,
        SI1145_REG_IRQEN_PS3EVERYSAMPLE =0x10,


        SI1145_REG_IRQMODE1 =0x05,
        SI1145_REG_IRQMODE2 =0x06,

        SI1145_REG_HWKEY        =0x07,
        SI1145_REG_MEASRATE0    =0x08,
        SI1145_REG_MEASRATE1    =0x09,
        SI1145_REG_PSRATE       =0x0A,
        SI1145_REG_PSLED21      =0x0F,
        SI1145_REG_PSLED3       =0x10,
        SI1145_REG_UCOEFF0      =0x13,
        SI1145_REG_UCOEFF1      =0x14,
        SI1145_REG_UCOEFF2      =0x15,
        SI1145_REG_UCOEFF3      =0x16,
        SI1145_REG_PARAMWR      =0x17,
        SI1145_REG_COMMAND      =0x18,
        SI1145_REG_RESPONSE     =0x20,
        SI1145_REG_IRQSTAT      =0x21,
        SI1145_REG_IRQSTAT_ALS  =0x01,

        SI1145_REG_ALSVISDATA0  =0x22,
        SI1145_REG_ALSVISDATA1  =0x23,
        SI1145_REG_ALSIRDATA0   =0x24,
        SI1145_REG_ALSIRDATA1   =0x25,
        SI1145_REG_PS1DATA0     =0x26,
        SI1145_REG_PS1DATA1     =0x27,
        SI1145_REG_PS2DATA0     =0x28,
        SI1145_REG_PS2DATA1     =0x29,
        SI1145_REG_PS3DATA0     =0x2A,
        SI1145_REG_PS3DATA1     =0x2B,
        SI1145_REG_UVINDEX0     =0x2C,
        SI1145_REG_UVINDEX1     =0x2D,
        SI1145_REG_PARAMRD      =0x2E,
        SI1145_REG_CHIPSTAT     =0x30,

        SI1145_ADDR             =0x60
    };
    
  bool SI1145begin();
  void reset();

  uint16_t readUV();
  uint16_t readIR();
  uint16_t readVisible();
  uint16_t readProx();

  uint8_t readParam(uint8_t p);
  uint8_t writeParam(uint8_t p, uint8_t v);

  bool boardPresent = false;
  //using namespace ::touch_develop::i2c;
  //using namespace ::touch_develop;
  //using namespace ::bitvm;

union FloatOrUInt
{
    float asFloat;
    unsigned int asUInt;
} fu;

I2CSimple SI1145_i2c(SI1145_ADDR);

bool SI1145begin() {
 
  uint8_t id = SI1145_i2c.read8(SI1145_REG_PARTID);
  if (id != 0x46 || id != 0x45) 
  {
      boardPresent = true;
  }
  else
  {
      return false; // look for SI1146 change to 0x45 for the SI1145
  }
  
  reset();
  

    /***********************************/
  // enable UVindex measurement coefficients!
  SI1145_i2c.write8(SI1145_REG_UCOEFF0, 0x29);
  SI1145_i2c.write8(SI1145_REG_UCOEFF1, 0x89);
  SI1145_i2c.write8(SI1145_REG_UCOEFF2, 0x02);
  SI1145_i2c.write8(SI1145_REG_UCOEFF3, 0x00);

  // enable UV sensor
  writeParam(SI1145_PARAM_CHLIST, SI1145_PARAM_CHLIST_ENUV |
  SI1145_PARAM_CHLIST_ENALSIR | SI1145_PARAM_CHLIST_ENALSVIS |
  SI1145_PARAM_CHLIST_ENPS1);
  // enable interrupt on every sample
  SI1145_i2c.write8(SI1145_REG_INTCFG, SI1145_REG_INTCFG_INTOE);  
  SI1145_i2c.write8(SI1145_REG_IRQEN, SI1145_REG_IRQEN_ALSEVERYSAMPLE);  

/****************************** Prox Sense 1 */

  // program LED current
  SI1145_i2c.write8(SI1145_REG_PSLED21, 0x03); // 20mA for LED 1 only
  writeParam(SI1145_PARAM_PS1ADCMUX, SI1145_PARAM_ADCMUX_LARGEIR);
  // prox sensor #1 uses LED #1
  writeParam(SI1145_PARAM_PSLED12SEL, SI1145_PARAM_PSLED12SEL_PS1LED1);
  // fastest clocks, clock div 1
  writeParam(SI1145_PARAM_PSADCGAIN, 0);
  // take 511 clocks to measure
  writeParam(SI1145_PARAM_PSADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);
  // in prox mode, high range
  writeParam(SI1145_PARAM_PSADCMISC, SI1145_PARAM_PSADCMISC_RANGE|
    SI1145_PARAM_PSADCMISC_PSMODE);

  writeParam(SI1145_PARAM_ALSIRADCMUX, SI1145_PARAM_ADCMUX_SMALLIR);  
  // fastest clocks, clock div 1
  writeParam(SI1145_PARAM_ALSIRADCGAIN, 0);
  // take 511 clocks to measure
  writeParam(SI1145_PARAM_ALSIRADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);
  // in high range mode
  writeParam(SI1145_PARAM_ALSIRADCMISC, SI1145_PARAM_ALSIRADCMISC_RANGE);



  // fastest clocks, clock div 1
  writeParam(SI1145_PARAM_ALSVISADCGAIN, 0);
  // take 511 clocks to measure
  writeParam(SI1145_PARAM_ALSVISADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);
  // in high range mode (not normal signal)
  writeParam(SI1145_PARAM_ALSVISADCMISC, SI1145_PARAM_ALSVISADCMISC_VISRANGE);


/************************/

  // measurement rate for auto
  SI1145_i2c.write8(SI1145_REG_MEASRATE0, 0xFF); // 255 * 31.25uS = 8ms
  
  // auto run
  SI1145_i2c.write8(SI1145_REG_COMMAND, SI1145_PSALS_AUTO);

  return true;
}


void reset() {
  SI1145_i2c.write8(SI1145_REG_MEASRATE0, 0);
  SI1145_i2c.write8(SI1145_REG_MEASRATE1, 0);
  SI1145_i2c.write8(SI1145_REG_IRQEN, 0);
  SI1145_i2c.write8(SI1145_REG_IRQMODE1, 0);
  SI1145_i2c.write8(SI1145_REG_IRQMODE2, 0);
  SI1145_i2c.write8(SI1145_REG_INTCFG, 0);
  SI1145_i2c.write8(SI1145_REG_IRQSTAT, 0xFF);

  SI1145_i2c.write8(SI1145_REG_COMMAND, SI1145_RESET);
  wait_ms(10);
  SI1145_i2c.write8(SI1145_REG_HWKEY, 0x17);
  
  wait_ms(10);
}

// *****************************************************************************
// Satellite Microkit Sensor Read functions
// *****************************************************************************    

// *****************************************************************************
// Satellite Microkit Temperature
// *****************************************************************************
//
// TMP 102 temperature sensor
// Based on ADAFruit Library tmp102

    
I2CSimple tmp102_i2c(72);

// GLUE      
GLUE int ReadTemp102(){
    //uint8_t data[2];
    uint8_t firstbyte, secondbyte; //these are the bytes we read from the TMP102 temperature registers

    /* Reset the register pointer (by default it is ready to read temperatures)
  You can alter it to a writeable register and alter some of the configuration - 
  the sensor is capable of alerting you if the temperature is above or below a specified threshold. */
    //MicroBitI2C I2C(MICROBIT_PIN_SDA, MICROBIT_PIN_SCL);
    
    tmp102_i2c.write8(72,0x00);
    wait_ms(5);
    firstbyte = tmp102_i2c.read8(72);
    secondbyte = tmp102_i2c.read8(72+1);
    
    // TMP102 by default returns 12 bits of temperature data in twos compliment format
    // Packed into 2x 8-bit bytes as follows:
    // First byte: T11 T10 T9 T8 T7 T6 T5 T4 Second byte: T3 T2 T1 T0 0 0 0 0
    // Last 4 bits are 0 (padding).
    
    // TEST data example
    // -430 in 12 bit twos compliment is: 111001010010 (see: http://www.exploringbinary.com/twos-complement-converter/)
    // TMP102 pads this with 4 zeros to RHS so becomes: 1110 0101 0010 0000
    // as two bytes in hex this is: 0xE5 0x20
    //data[0] = 0xE5;
    //data[1] = 0x20;
    
    // Converting to temperature in C
    // re-constitute the entire 16 bit, twos compliment temperature into a signed 16 bit integer
    // LEFT SHIFT the first byte by 8 bits (pads right hand side with 0s)
    //int16_t val1 = data[0] << 8;
    // ADD the second byte to it, unmodified
    //val1 += data[1];
    // now RIGHT SHIFT the result by 4 bits to get rid of the unwanted 4x 0 padding
    // note that the compiler knows this is a SIGNED (arithmetic) right shift (preserves sign when using GCC).
    //val1 = val1 >> 4;
    
    // or you can do this in one line
    int val = (int16_t)((firstbyte << 8) + secondbyte) >> 4;
    
    return (int) val * 0.0625;
}

// *****************************************************************************
// Satellite Microkit Ligth Sensor
// *****************************************************************************
// returns the UV index * 100 (divide by 100 to get the index)
GLUE uint16_t readUV() {
    uint16_t r = SI1145_i2c.read16(0x2C); 
    r /= 100;
    return r;
}

// returns visible+IR light levels
GLUE uint16_t readVisible() {
 return SI1145_i2c.read16(0x22); 
}

// returns IR light levels
GLUE  uint16_t readIR() {
 return SI1145_i2c.read16(0x24); 
}

// returns "Proximity" - assumes an IR LED is attached to LED
//GLUE 
uint16_t readProx() {
 return SI1145_i2c.read16(0x26); 
}

/*********************************************************************/

uint8_t writeParam(uint8_t p, uint8_t v) { 
  SI1145_i2c.write8(SI1145_REG_PARAMWR, v);
  SI1145_i2c.write8(SI1145_REG_COMMAND, p | SI1145_PARAM_SET);
  return SI1145_i2c.read8(SI1145_REG_PARAMRD);
}

uint8_t readParam(uint8_t p) {
  SI1145_i2c.write8(SI1145_REG_COMMAND, p | SI1145_PARAM_QUERY);
  return SI1145_i2c.read8(SI1145_REG_PARAMRD);
}

// *****************************************************************************
// Microbit Radio GLUE functions
// *****************************************************************************    

int satelliteId = 0;

GLUE void Init(int groupid)
{
    //uBit.init();
    int r = uBit.radio.setGroup(groupid);

    //if(r==MICROBIT_NOT_SUPPORTED)
    //    printf("Failed BLE");
    //if(r==MICROBIT_OK)
    //    printf("Passed BLE");
        
    uBit.radio.setTransmitPower(6);
}

GLUE void SetTransmittPower(int p)
{
    if(p>0 && p<8)
        uBit.radio.setTransmitPower(p);
    else
        uBit.radio.setTransmitPower(6);
}

GLUE void Enable()
{
    int r = uBit.radio.enable();
    //if(r==MICROBIT_NOT_SUPPORTED)
    //    printf("Failed BLE");
    //if(r==MICROBIT_OK)
    //    printf("Passed BLE");
}

GLUE void Disable()
{
    uBit.radio.disable();
}

GLUE int Recv()
{
    /*
    PacketBuffer buffer = uBit.radio.datagram.recv(); 
    StringData *ptr;
    ptr = (StringData*)(void*)empty;
    
    // Store the length of the new string
    ptr->len = buffer.length();
    memcpy(ptr->data, buffer.getBytes(), buffer.length());
    ptr->data[buffer.length()] = 0;
    */
    
    PacketBuffer buf = uBit.radio.datagram.recv();

    return buf[0];
}

static ManagedString SendData;
GLUE void Send(StringData *str)
{
    //printf("Attempt\n\r");
    //printf("str length: %02x",str->len);
    SendData = ManagedString(str);
    //printf("Success\n\r");
    uBit.radio.datagram.send((uint8_t *)SendData.toCharArray(), SendData.length());
}


// *******************************************************************************************************************************************
// Camera API
// *******************************************************************************************************************************************

// SC16IS750  ******************************
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

      
enum
{

  SC16IS750_ADDRESS_BB     =0X9A,

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
		uint8_t ping();
		int peek();
		void flush();    
        uint8_t device_address_sspin;
        uint8_t protocol;
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
     
    
// SC16IS750 CPP *****************************************************************************************************************************

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
/*
 Modified for micro:bit TouchDevelop
 Author:
 Paul Foster        paulfo@microsoft.com
*/

I2CSimple i2c(SC16IS750_ADDRESS_BB>>1); // Appears to be default BA

SC16IS750::SC16IS750(uint8_t prtcl, int addr_sspin)
{
    
 protocol = prtcl;
 addr = (addr_sspin>>1);
 /*
 if ( protocol == SC16IS750_PROTOCOL_I2C ) {
		device_address_sspin = (addr_sspin>>1);
	} else {
		device_address_sspin = addr_sspin;
	}
    */
	peek_flag = 0;
}

void SC16IS750::begin(uint32_t baud)
{
  
    protocol = SC16IS750_PROTOCOL_I2C;
    peek_flag = 0;
    //uBit.display.scroll("R");
    ResetDevice();
    //uBit.display.scroll("FIFO");
    FIFOEnable(1);
    //uBit.display.scroll("Baud");
	SetBaudrate(baud);
    //uBit.display.scroll("Line");
    SetLine(8,0,1);
}

int  SC16IS750::available(void)
{
    return FIFOAvailableData();
}

int  SC16IS750::read(void)
{
	if ( peek_flag == 0) {
		return ReadByte();
	} else {
		peek_flag = 0;
		return peek_buf;
	}
}

void  SC16IS750::write(uint8_t val)
{
    WriteByte(val);
}

/*void  pinMode(uint8_t pin, uint8_t i_o)
{
   // TODO: GPIOSetPinMode(pin, i_o);
}

void  digitalWrite(uint8_t pin, uint8_t value)
{
    // TODO: GPIOSetPinState(pin, value);
}

uint8_t  digitalRead(uint8_t pin)
{
   return pin;// TODO: PIOGetPinState(pin);
}
*/

uint8_t SC16IS750::ReadRegister(uint8_t reg_addr)
{
    uint8_t result = 0;
	if ( protocol == SC16IS750_PROTOCOL_I2C ) {  // register read operation via I2C
        result = i2c.read8((reg_addr<<3));
		//WIRE.beginTransmission(device_address_sspin);
		//WIRE.write((reg_addr<<3));
		//WIRE.endTransmission(0);
		//WIRE.requestFrom(device_address_sspin,(uint8_t)1);
		//result = WIRE.read();
    }
    
	return result;

}


void SC16IS750::WriteRegister(uint8_t reg_addr, uint8_t val)
{
    if ( protocol == SC16IS750_PROTOCOL_I2C ) {  // register read operation via I2C
		i2c.write8((reg_addr<<3) ,val);
        //WIRE.beginTransmission(device_address_sspin);
		//WIRE.write((reg_addr<<3));
		//WIRE.write(val);
		//WIRE.endTransmission(1);
    }

    return ;
}

int16_t  SC16IS750::SetBaudrate(uint32_t baudrate) //return error of baudrate parts per thousand
{
    uint16_t divisor;
    uint8_t prescaler;
    uint32_t actual_baudrate;
    int16_t error;
    uint8_t temp_lcr;
    if ( (ReadRegister(SC16IS750_REG_MCR)&0x80) == 0) { //if prescaler==1
        prescaler = 1;
    } else {
        prescaler = 4;
    }

    divisor = (SC16IS750_CRYSTCAL_FREQ/prescaler)/(baudrate*16);

    temp_lcr = ReadRegister(SC16IS750_REG_LCR);
    temp_lcr |= 0x80;
    WriteRegister(SC16IS750_REG_LCR,temp_lcr);
    //write to DLL
    WriteRegister(SC16IS750_REG_DLL,(uint8_t)divisor);
    //write to DLH
    WriteRegister(SC16IS750_REG_DLH,(uint8_t)(divisor>>8));
    temp_lcr &= 0x7F;
    WriteRegister(SC16IS750_REG_LCR,temp_lcr);


    actual_baudrate = (SC16IS750_CRYSTCAL_FREQ/prescaler)/(16*divisor);
    error = ((float)actual_baudrate-baudrate)*1000/baudrate;

    return error;

}

void  SC16IS750::ResetDevice(void)
{
    uint8_t reg;

    reg = ReadRegister(SC16IS750_REG_IOCONTROL);
    reg |= 0x08;
    WriteRegister(SC16IS750_REG_IOCONTROL, reg);

    return;
}



void  SC16IS750::__isr(void)
{
    uint8_t irq_src;

    irq_src = ReadRegister(SC16IS750_REG_IIR);
    irq_src = (irq_src >> 1);
    irq_src &= 0x3F;

    switch (irq_src) {
        case 0x06:                  //Receiver Line Status Error
            break;
        case 0x0c:               //Receiver time-out interrupt
            break;
        case 0x04:               //RHR interrupt
            break;
        case 0x02:               //THR interrupt
            break;
        case 0x00:                  //modem interrupt;
            break;
        case 0x30:                  //input pin change of state
            break;
        case 0x10:                  //XOFF
            break;
        case 0x20:                  //CTS,RTS
            break;
        default:
            break;
    }
    return;
}

void  SC16IS750::FIFOEnable(uint8_t fifo_enable)
{
    uint8_t temp_fcr;

    temp_fcr = ReadRegister(SC16IS750_REG_FCR);
    //uBit.display.scroll(ManagedString(temp_fcr));
    
    if (fifo_enable == 0){
        temp_fcr &= 0xFE;
    } else {
        temp_fcr |= 0x01;
    }
    //uBit.display.scroll(ManagedString(temp_fcr));
    WriteRegister(SC16IS750_REG_FCR,temp_fcr);

    return;
}

void  SC16IS750::FIFOReset(uint8_t rx_fifo)
{
     uint8_t temp_fcr;

    temp_fcr = ReadRegister(SC16IS750_REG_FCR);

    if (rx_fifo == 0){
        temp_fcr |= 0x04;
    } else {
        temp_fcr |= 0x02;
    }
    WriteRegister(SC16IS750_REG_FCR,temp_fcr);

    return;

}

void  SC16IS750::FIFOSetTriggerLevel(uint8_t rx_fifo, uint8_t length)
{
    uint8_t temp_reg;

    temp_reg = ReadRegister(SC16IS750_REG_MCR);
    temp_reg |= 0x04;
    WriteRegister(SC16IS750_REG_MCR,temp_reg); //SET MCR[2] to '1' to use TLR register or trigger level control in FCR register

    temp_reg = ReadRegister(SC16IS750_REG_EFR);
    WriteRegister(SC16IS750_REG_EFR, temp_reg|0x10); //set ERF[4] to '1' to use the  enhanced features
    if (rx_fifo == 0) {
        WriteRegister(SC16IS750_REG_TLR, length<<4); //Tx FIFO trigger level setting
    } else {
        WriteRegister(SC16IS750_REG_TLR, length);    //Rx FIFO Trigger level setting
    }
    WriteRegister(SC16IS750_REG_EFR, temp_reg); //restore EFR register

    return;
}

uint8_t  SC16IS750::FIFOAvailableData(void)
{
   return ReadRegister(SC16IS750_REG_RXLVL);
//    return i2c.read8(SC16IS750_REG_LSR) & 0x01;
}

uint8_t  SC16IS750::FIFOAvailableSpace(void)
{
   return ReadRegister(SC16IS750_REG_TXLVL);

}

void  SC16IS750::WriteByte(uint8_t val)
{
	uint8_t tmp_lsr;
	do {
		tmp_lsr = ReadRegister(SC16IS750_REG_LSR);
	} while ((tmp_lsr&0x20) ==0);

	WriteRegister(SC16IS750_REG_THR,val);
}

int  SC16IS750::ReadByte(void)
{
	volatile uint8_t val;
	if (FIFOAvailableData() == 0) {
		return -1;

	} else {
	  val = ReadRegister(SC16IS750_REG_RHR);
	  return val;
	}


}

void  SC16IS750::EnableTransmit(uint8_t tx_enable)
{
    uint8_t temp_efcr;
    temp_efcr = ReadRegister(SC16IS750_REG_EFCR);
    if ( tx_enable == 0) {
        temp_efcr |= 0x04;
    } else {
        temp_efcr &= 0xFB;
    }
    WriteRegister(SC16IS750_REG_EFCR,temp_efcr);

    return;
}

uint8_t  SC16IS750::ping()
{
    //write a value to the scratch pad register
	WriteRegister((SC16IS750_REG_SPR) ,0x55);
    
    //uBit.display.scroll(ManagedString(ReadRegister((SC16IS750_REG_SPR) )));
        
	if (ReadRegister(SC16IS750_REG_SPR) !=0x55) {
		return 0;
	}

	WriteRegister(SC16IS750_REG_SPR,0xAA);
	if (ReadRegister(SC16IS750_REG_SPR) !=0xAA) {
		return 0;
	}

	return 1;

}

void  SC16IS750::flush()
{
	uint8_t tmp_lsr;

	do {
		tmp_lsr = ReadRegister(SC16IS750_REG_LSR);
	} while ((tmp_lsr&0x20) ==0);


}

int   SC16IS750::peek()
{
	if ( peek_flag == 0 ) {
		peek_buf =ReadByte();
		if (  peek_buf >= 0 ) {
			peek_flag = 1;
		}
	}

	return peek_buf;

}

void SC16IS750::SetLine(uint8_t data_length, uint8_t parity_select, uint8_t stop_length )
{
    uint8_t temp_lcr;
    temp_lcr = ReadRegister(SC16IS750_REG_LCR);
    temp_lcr &= 0xC0; //Clear the lower six bit of LCR (LCR[0] to LCR[5]

    switch (data_length) {            //data length settings
        case 5:
            break;
        case 6:
            temp_lcr |= 0x01;
            break;
        case 7:
            temp_lcr |= 0x02;
            break;
        case 8:
            temp_lcr |= 0x03;
            break;
        default:
            temp_lcr |= 0x03;
            break;
    }

    if ( stop_length == 2 ) {
        temp_lcr |= 0x04;
    }

    switch (parity_select) {            //parity selection length settings
        case 0:                         //no parity
             break;
        case 1:                         //odd parity
            temp_lcr |= 0x08;
            break;
        case 2:                         //even parity
            temp_lcr |= 0x18;
            break;
        case 3:                         //force '1' parity
            temp_lcr |= 0x03;
            break;
        case 4:                         //force '0' parity
            break;
        default:
            break;
    }

    WriteRegister(SC16IS750_REG_LCR,temp_lcr);
}


// CAMERA CPP ********************************************************************************************************************************

    
SC16IS750 i2cuart = SC16IS750(SC16IS750_PROTOCOL_I2C, SC16IS750_ADDRESS_BB);

bool menu_active = true, run_mode = false;;
long int menu_val = 0;

//GLUE
bool gen_version() {
  //printf("Get version\n\r");
  int timeout = 0;
  bool err = true;
  i2cuart.write(0x56);
  i2cuart.write(0x00);
  i2cuart.write(0x01); //was 11
  //i2cuart.write(0x00);
  while (i2cuart.available() == 0)
  {

    wait_ms(50);
    timeout += 1;
    if (timeout > 10) 
    {
      //uBit.serial.printf("UART timeout\n\r");
      //uBit.display.scroll("UART F");
      wait_ms(50);
      break;
    }
    
    //uBit.serial.printf("Waiting for UART %d\n\r",timeout);
  }

  if (timeout < 10) {
      //uBit.display.scroll("Gen T");
    while (i2cuart.available() > 0) {
      i2cuart.read(); // clearing data buffer
      //printf("Reading response\n\r");
      //int ret_val = i2cuart.read();
      //uBit.display.scroll(ManagedString(i2cuart.read()));
      //Serial.print(char(ret_val));
      //uBit.display.scroll(".");
    }
  }
  else {
    //Timeout
    //uBit.display.scroll("C:Timeout");
    //uBit.serial.printf("Timeout Get Version failed\n\r");
    err = false;
  }
  return err;
}

void set_startup_baud()
{
  
  //printf("Setting 115k\n\r");
  i2cuart.write(0x56);
  i2cuart.write(0x00);
  i2cuart.write(0x31);
  i2cuart.write(0x06);
  i2cuart.write(0x04);
  i2cuart.write(0x02);
  i2cuart.write(0x00);
  i2cuart.write(0x08);
  // hardcoded to 115200
  i2cuart.write(0x0d);
  i2cuart.write(0xa6);
  
  while (i2cuart.available() == 0)
  {
    //uBit.serial.printf("waiting for UART\n\r");
    wait_ms(50);
  }

  while (i2cuart.available() > 0) {
    int ret_val = i2cuart.read();
    //uBit.serial.printf("Reading UART %02x\n\r", ret_val);
    }
}

void set_port(int baud) {

  //printf("Setting 115k\n\r");
  i2cuart.write(0x56);
  i2cuart.write(0x00);
  i2cuart.write(0x24);
  i2cuart.write(0x03);
  i2cuart.write(0x01);
  
  // hardcoded to 115200
  //i2cuart.write(0x0d);
  //i2cuart.write(0xa6);
  
  i2cuart.write(baud >> 8);
  i2cuart.write(baud & 0xFF);
  
  while (i2cuart.available() == 0)
  {
    //uBit.serial.printf("waiting for UART\n\r");
    wait_ms(50);
  }

  while (i2cuart.available() > 0) {
    int ret_val = i2cuart.read();
    //uBit.serial.printf("Reading UART %02x\n\r", ret_val);
    }
   
  i2cuart.ResetDevice();
  i2cuart.begin(baud);
  
  if (i2cuart.ping() != 1) {
    uBit.serial.printf("i2cuart ping failed\n\r");
  }
  //else
  //  uBit.serial.printf("i2cuart ping ok\n\r");
    
}

void system_reset() {

  i2cuart.write(0x56);
  i2cuart.write(0x00);
  i2cuart.write(0x26);
  i2cuart.write(0x00);

  while (i2cuart.available() == 0)
  {
    //uBit.serial.printf(".");
    wait_ms(100);
  }
  //Serial.println();
  while (i2cuart.available() > 0) {
    int ret_val = i2cuart.read();
    //uBit.serial.printf("%02x ",ret_val);
    wait_ms(5);
  }
  
}

void fbuf_ctrl(short mode) {
  i2cuart.write(0x56);
  i2cuart.write(0x00);
  i2cuart.write(0x36);
  i2cuart.write(0x01);
  i2cuart.write(mode);

  while (i2cuart.available() == 0)
  {
    //uBit.serial.printf(".");
    wait_ms(100);
  }

  while (i2cuart.available() > 0) {
    int ret_val = i2cuart.read();
    //uBit.serial.printf("%02x",ret_val);
    }
}

/*
void Save_fbuf(unsigned int len, int imageincrement) {

  // Calculate a file name here
  char buffer [16];
  unsigned short ret_val;
  int ret = sprintf (buffer, "/sd/image%d.jpg", imageincrement);
  FILE *fp = fopen(buffer, "wb"); 
  unsigned int batch = 48, total_read = 0 , counter = 0;
  
  // How to reclaim buffer space here
  // Noting that the SD Filesystem has just allocated 512 bytes on stack and heap!!
  
  while (total_read < len) {
    if ((len - total_read) < batch)
      batch = len - total_read;

    i2cuart.write(0x56);
    i2cuart.write(0x00);
    i2cuart.write(0x32);
    i2cuart.write(0x0C);
    i2cuart.write(0x00);
    i2cuart.write(0x0A); //0x0A
    i2cuart.write(0x00);
    i2cuart.write(0x00);
    i2cuart.write(total_read >> 8);
    i2cuart.write(total_read & 0xFF);
    i2cuart.write(0x00);
    i2cuart.write(0x00);
    i2cuart.write(batch >> 8);
    i2cuart.write(batch & 0xFF);
    i2cuart.write(0x00);
    i2cuart.write(0x10);//0x10

    while (i2cuart.available() == 0)
    {
      wait_ms(4);
    }
    
    uBit.serial.printf(".");
    counter = 0;
    
    while (counter != batch + 10) {
      if (i2cuart.available() > 0) {
        ret_val = i2cuart.read(); // moved definition
        if ((counter > 4) && (counter < batch + 5)) {
          //uBit.serial.printf("%02x",ret_val);
          //fprintf(fp,"%02x",ret_val);       
          fwrite(&ret_val, 1, 1, fp);
        }
        counter++;
      }
    }
    total_read += batch;
    //uBit.serial.printf("*");
  }
    //uBit.serial.printf("\n\r");
    
    fclose(fp);
}
*/

void read_data(short reg) {
  i2cuart.write(0x56);
  i2cuart.write(0x00);
  i2cuart.write(0x30);
  i2cuart.write(0x04);
  i2cuart.write(0x04);
  i2cuart.write(0x02);
  i2cuart.write(0x00);
  i2cuart.write(reg);
  while (i2cuart.available() == 0)
  {
    wait_ms(100);
  }

  while (i2cuart.available() > 0) {
    int ret_val = i2cuart.read();
    if (ret_val < 16) {
      //Serial.print("0");
    }
    //Serial.print(ret_val, HEX);

  }

}

unsigned int get_fbuf_len() {
  unsigned int len;
  int ret_val[9];

  i2cuart.write(0x56);
  i2cuart.write(0x00);
  i2cuart.write(0x34);
  i2cuart.write(0x01);
  i2cuart.write(0x00);

  while (i2cuart.available() == 0)
  {
    wait_ms(50);
  }

  for (int i = 0; i < 9; i++) {
    ret_val[i] = i2cuart.read();
    //uBit.serial.printf("%02x ",ret_val[i]);
  }

  len = (ret_val[7] << 8) | ret_val[8];

  return len;
}



// *****************************************************************************************************************************************************************************************
// *****************************************************************************************************************************************************************************************
// Satellite Microkit State, sensor reading and picture taking
// *****************************************************************************************************************************************************************************************  
// *****************************************************************************************************************************************************************************************

char buffer[30];

uint16_t UV=0;
uint16_t IR=0;
uint16_t V=0;
int Temp=0;
int X=0;
int Y=0;
int Z =0;
int Heading=0;
int Pitch =0;
int Roll =0;
unsigned int PictureSize = 0;


void Send_fbuf(bool radio) {
  int n;
  unsigned short ret_val;
  unsigned int batch = 48, total_read = 0 , counter = 0;
  
  if(radio)
        {
            n=sprintf(buffer,"{N:%02d,C:[", satelliteId);
            if(n==0) return;
            uBit.radio.datagram.send((uint8_t *) buffer,9);
        }
  else
        uBit.serial.printf("[");
  
  while (total_read < PictureSize) {
    if ((PictureSize - total_read) < batch)
      batch = PictureSize - total_read;

    i2cuart.write(0x56);
    i2cuart.write(0x00);
    i2cuart.write(0x32);
    i2cuart.write(0x0C);
    i2cuart.write(0x00);
    i2cuart.write(0x0A); //0x0A
    i2cuart.write(0x00);
    i2cuart.write(0x00);
    i2cuart.write(total_read >> 8);
    i2cuart.write(total_read & 0xFF);
    i2cuart.write(0x00);
    i2cuart.write(0x00);
    i2cuart.write(batch >> 8);
    i2cuart.write(batch & 0xFF);
    i2cuart.write(0x00);
    i2cuart.write(0x10);//0x10

    while (i2cuart.available() == 0)
    {
      wait_ms(4);
    }
    
    counter = 0;
    
    while (counter != batch + 10) {
      if (i2cuart.available() > 0) {
        ret_val = i2cuart.read(); // moved definition
        if ((counter > 4) && (counter < batch + 5)) {
              if(radio)
                    {
                        n=sprintf(buffer,"%02x",ret_val);
                        if(n==0) return;
                        uBit.radio.datagram.send((uint8_t *) buffer,n);
                        wait_ms(6); // Have to pause to allow radio transmission to occur
                    }
               else
                    uBit.serial.printf("%02x",ret_val);
        }
        counter++;
      }
    }
    total_read += batch;
    //uBit.serial.printf("*");
  }
   if(radio)
   {
        n=sprintf(buffer,"]}\n",ret_val);
        if(n==0) return;
        uBit.radio.datagram.send((uint8_t *) buffer,3);    
   }
   else
        uBit.serial.printf("]\n");
    
}

void ReadSensors()
{
    if(boardPresent)
    {
        UV = readUV();
        IR = readIR();
        V =  readVisible();
        Temp = ReadTemp102();
    }
    else
    {
        UV = 0;
        IR = 0;
        V = 0;
        Temp = 0;
    }    
    X = uBit.accelerometer.getX();
    Y = uBit.accelerometer.getY();
    Z = uBit.accelerometer.getZ();
    Heading = uBit.compass.heading();
    Pitch = uBit.accelerometer.getPitch();
    Roll = uBit.accelerometer.getRoll();
    wait_ms(3); // gives sensors time to breath in tight loops
}


// NOT A GLUE API JUST HERE FOR DEFINITION OF IMAGE SIZE
void set_Image_Size(uint8_t size);

bool CameraSetup()
{
  i2cuart.begin(115200);               //baudrate setting
  if (i2cuart.ping() != 1) {
    //device not found
    return false;
  }
  
  //set_startup_baud(); //Permanently set baud rate. Requires power off. This works to 115k
  //wait_ms(2500);
  //set_port(115200); // Temporarily set compression doesn't work because of i2cuart
  //uBit.serial.printf("baud set\n\r");
  
  set_Image_Size(0x11); //0x00 640x480 0x11 320x240 0x22 160x120
  
  // Set compression
  //Set_Image_Compression(0);
  //uBit.serial.printf("\n\rImage compression set\n\r");
  
  return true;
  //Setup complete
};

void SendLightSensor(bool radio)
{
    ReadSensors();
        
    // SatelliteID length 2, I:00000,U:00000,V:00000
    //char buffer[28];
    int n;
    if(satelliteId==0)
    {
        uBit.display.scroll("No id set",100);
    }
    else
    {
        if(radio)
        {
            n=sprintf(buffer,"{N:%02d,I:%06u,U:%03u,V:%06u", satelliteId,IR,UV,V);
            if(n==0) return;
            uBit.radio.datagram.send((uint8_t *) buffer,30);
            n=sprintf(buffer,"}\n");
            if(n==0) return;
            uBit.radio.datagram.send((uint8_t *) buffer,2);
        }
        else
        {
            uBit.serial.printf("{N:%02d,I:%06u,U:%03u,V:%06u}\n", satelliteId,IR,UV,V);
        }
    }
}

void SendTemperatureSensor(bool radio)
{
    ReadSensors();
    
    // SatelliteID length 2, I:00000,U:00000,V:00000
    //char buffer[10];
    int n;
    if(satelliteId==0)
    {
        uBit.display.scroll("No id set",100);
    }
    else
    {
        if(radio)
        {
            n=sprintf(buffer,"{N:%02d,T:%03d}\n", satelliteId,Temp);
            if(n==0) return;
            uBit.radio.datagram.send((uint8_t *) buffer,13);
        }
        else
        {
            // Serial
            uBit.serial.printf("{N:%02d,T:%03d}\n", satelliteId,Temp);
        }
    } 
}

void SendAccelerometerSensor(bool radio)
{
    ReadSensors();
    
    // SatelliteID length 2, I:00000,U:00000,V:00000
    //char buffer[30];
    int n;
    if(satelliteId==0)
    {
        uBit.display.scroll("No id set",100);
    }
    else
    {
        if(radio)
        {
        n=sprintf(buffer,"{N:%02d,X:%05d,Y:%05d,Z:%05d", satelliteId,X,Y,Z);
        if(n==0) return;
        uBit.radio.datagram.send((uint8_t *) buffer,30);
        n=sprintf(buffer,"}\n");
        if(n==0) return;
        uBit.radio.datagram.send((uint8_t *) buffer,2);
        }
        else
        {
            // Serial
            uBit.serial.printf("{N:%02d,X:%05d,Y:%05d,Z:%05d}\n", satelliteId,X,Y,Z);
        }
    }    
}

void SendCompassSensor(bool radio)
{
    ReadSensors();
    
    // SatelliteID length 2, I:00000,U:00000,V:00000
    //char buffer[10];
    int n;
    if(satelliteId==0)
    {
        uBit.display.scroll("No id set",100);
    }
    else
    {
        if(radio)
        {
            n=sprintf(buffer,"{N:%02d,H:%03d}\n", satelliteId,Heading);
            if(n==0) return;
            uBit.radio.datagram.send((uint8_t *) buffer,13);
        }
        else
        {
            // Serial
            uBit.serial.printf("{N:%02d,H:%03d}\n", satelliteId,Heading);
        }
    }    
}

void SendPitchAndRoll(bool radio)
{
    ReadSensors();    
    // SatelliteID length 2, I:00000,U:00000,V:00000
    //char buffer[16];
    int n;
    if(satelliteId==0)
    {
        uBit.display.scroll("No id set",100);
    }
    else
    {
        if(radio)
        {
            n=sprintf(buffer,"{N:%02d,P:%03d,R:%03d}\n", satelliteId,Pitch,Roll);
            if(n==0) return;
            uBit.radio.datagram.send((uint8_t *) buffer,19);
        }
        else
        {
            // Serial
            uBit.serial.printf("{N:%02d,P:%03d,R:%03d}\n", satelliteId,Pitch,Roll);
        }
    }    
}

void SendAllSensors(bool radio)
{
    ReadSensors();

    int n;

    if(satelliteId==0)
    {
        uBit.display.scroll("No id set",100);
    }
    else
    {
        if(radio)
        {
            //printf("Send\n\r");
            n=sprintf(buffer,"{N:%02d,I:%06u,U:%03u,V:%06u", satelliteId,IR,UV,V);
            if(n==0) return;
            uBit.radio.datagram.send((uint8_t *) buffer,29);
            n = sprintf(buffer,",T:%03d,",Temp);
            if(n==0) return;
            uBit.radio.datagram.send((uint8_t *) buffer,7);
            n = sprintf(buffer,"X:%05d,Y:%05d,Z:%05d",X,Y,Z);
            if(n==0) return;
            uBit.radio.datagram.send((uint8_t *) buffer,23);
            n = sprintf(buffer,",H:%03d,P:%03d,R:%03d}\n",Heading,Pitch,Roll);
            if(n==0) return;
            uBit.radio.datagram.send((uint8_t *) buffer,20);
        }
        else
        {
            // Serial
            uBit.serial.printf("{N:%02d,I:%06u,U:%03u,V:%06u,T:%03d,X:%05d,Y:%05d,Z:%05d,H:%03d,P:%03d,R:%03d}\n", satelliteId,IR,UV,V,Temp,X,Y,Z,Heading,Pitch,Roll);
        }
    }  
}

// *****************************************************************************************************************************************************************************************
// *****************************************************************************************************************************************************************************************
// Satellite Microkit Radio library GLUE functions for TD 
// *****************************************************************************************************************************************************************************************  
// *****************************************************************************************************************************************************************************************

GLUE void set_Image_Size(uint8_t size)
{
  i2cuart.write(0x56);
  i2cuart.write(0x00);
  i2cuart.write(0x31);
  i2cuart.write(0x05);
  i2cuart.write(0x04);
  i2cuart.write(0x01);
  i2cuart.write(0x00);
  i2cuart.write(0x19);
  i2cuart.write(size); // small. Size: 640x480 =0x00, 320x240 = 0x11, 160x120 = 0x22

  while (i2cuart.available() == 0)
  {
    uBit.serial.printf(".");
    wait_ms(50);
  }

  while (i2cuart.available() > 0) {
    int ret_val = i2cuart.read();
    uBit.serial.printf("%02x ",ret_val);
  }
  
  system_reset(); // required to get image size set
  uBit.serial.printf("\n\rcamera reset\n\r");
}

//GLUE
void Set_Image_Compression(uint8_t size)
{
  i2cuart.write(0x56);
  i2cuart.write(0x00);
  i2cuart.write(0x31);
  i2cuart.write(0x05);
  i2cuart.write(0x01);
  i2cuart.write(0x01);
  i2cuart.write(0x12);
  i2cuart.write(0x04);
  i2cuart.write(size); // compression 00 to FF

  while (i2cuart.available() == 0)
  {
    uBit.serial.printf(".");
    wait_ms(5);
  }

  while (i2cuart.available() > 0) {
    int ret_val = i2cuart.read();
    uBit.serial.printf("%02x ",ret_val);
  }
  
}

GLUE void SetId(int id)
{
    satelliteId = id;
}

GLUE void SendRadioLightSensor()
{
    SendLightSensor(true);
}
GLUE void SendRadioTemperatureSensor()
{
    SendTemperatureSensor(true);
}
GLUE void SendRadioAccelerometerSensor()
{
    SendAccelerometerSensor(true);
}
GLUE void SendRadioCompassSensor()
{
    SendCompassSensor(true);
}
GLUE void SendRadioPitchAndRollSensor()
{
    SendPitchAndRoll(true);
}
GLUE void SendRadioAllSensors()
{
    SendAllSensors(true);
}
GLUE void SendSerialLightSensor()
{
    SendLightSensor(false);
}
GLUE void SendSerialTemperatureSensor()
{
    SendTemperatureSensor(false);
}

GLUE void SendSerialAccelerometerSensor()
{
    SendAccelerometerSensor(false);
}

GLUE void SendSerialCompassSensor()
{
    SendCompassSensor(false);
}

GLUE void SendSerialPitchAndRollSensor()
{
    SendPitchAndRoll(false);
}

GLUE void SendSerialAllSensors()
{
    SendAllSensors(false);
}

GLUE int TakePicture()
{

    if(CameraSetup())
        {
            fbuf_ctrl(0x02);
            fbuf_ctrl(0x00);
            PictureSize = get_fbuf_len();
            return 1;
        }
     else
     {
        return 0;
     }
        
}

int SavePicture()
{
    return 0;
}

GLUE int SendRadioPicture()
{
    Send_fbuf(true);
    return 1;
}

GLUE int SendSerialPicture()
{
    Send_fbuf(false);
    return 1;
}


// *******************************************************************************************************************************************
// Satellite Begin
// *******************************************************************************************************************************************

GLUE bool SatBegin(int Satid)
{
        satelliteId = Satid;
        
        Enable();
        
        bool r = CameraSetup();
        
        return SI1145begin();    
}

} // SARadio