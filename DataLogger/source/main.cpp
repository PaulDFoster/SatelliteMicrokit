#include "MicroBit.h"
#include "global.h"
//#include "camera_basic.h"
#include "SDFileSystem.h"
#include "I2CCommon.h"

    
//SDFileSystem sd(MICROBIT_PIN_P13, MICROBIT_PIN_P14, MICROBIT_PIN_P15, MICROBIT_PIN_P16, "sd"); // MOSI, MISO, SCLK, SSEL
SDFileSystem sd(MICROBIT_PIN_P13, MICROBIT_PIN_P14, MICROBIT_PIN_P15, MICROBIT_PIN_P16, "sd"); // MOSI, MISO, SCLK, SSEL

MicroBit uBit;

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

void LogDataHeader(FILE *fp)
{
    fprintf(fp,"SatID,IR,UV,Visible,Temp,Heading,X,Y,Z,Pitch,Roll\n\r");
}


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
// Satellite Microkit Temperature
// *****************************************************************************
//
// TMP 102 temperature sensor
// Based on ADAFruit Library tmp102

    
I2CSimple tmp102_i2c(72);

int ReadTemp102(){
  uint8_t firstbyte, secondbyte; //these are the bytes we read from the TMP102 temperature registers
  int val; /* an int is capable of storing two bytes, this is where we "chuck" the two bytes together. */ 
  float convertedtemp; /* We then need to multiply our two bytes by a scaling factor, mentioned in the datasheet. */ 
  float correctedtemp; 
  
  tmp102_i2c.write8(72,0x00);
  wait_ms(5);
  firstbyte = tmp102_i2c.read8(72);
  secondbyte = tmp102_i2c.read8(72+1);

/*The first byte contains the most significant bits, and 
 the second the less significant */
    val = ((firstbyte) << 4);  
 /* MSB */
    val |= (secondbyte >> 4);    
 /* LSB is ORed into the second 4 bits of our byte.
 Bitwise maths is a bit funky, but there's a good tutorial on the playground*/
    convertedtemp = val*0.0625;
    correctedtemp = convertedtemp - 5; 
    /* See the above note on overreading */
    /* See the above note on overreading */
    return (int) correctedtemp;
}

// *****************************************************************************
// Satellite Microkit Ligth Sensor
// *****************************************************************************
// returns the UV index * 100 (divide by 100 to get the index)

uint16_t readUV() {
    uint16_t r = SI1145_i2c.read16(0x2C); 
    r /= 100;
    return r;
}

// returns visible+IR light levels
uint16_t readVisible() {
 return SI1145_i2c.read16(0x22); 
}

// returns IR light levels
uint16_t readIR() {
 return SI1145_i2c.read16(0x24); 
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
}

void LogData(FILE *fp)
{
    ReadSensors();
    
    //fprintf(fp,"{N:%02d,I:%06u,U:%03u,V:%06u,T:%03d,X:%05d,Y:%05d,Z:%05d,H:%03d,P:%03d,R:%03d}\n", satelliteId,IR,UV,V,Temp,X,Y,Z,Heading,Pitch,Roll);
    
    //fflush(fp);

}

int main()
{
    uBit.init();
    //uBit.serial.baud(115200);
    uBit.sleep(2500);

    uBit.display.scroll("On");
    uBit.serial.printf("Microbit on\n\r");
    FILE *fp = fopen("/sd/out.txt", "w");  // Open "out.txt" on the local file system for writing
    uBit.serial.printf("File open\n\r");
    fprintf(fp, "Hello World!\n\r");
    fprintf(fp, "Hello World!2\n\r");
    fprintf(fp, "Hello World!3\n\r");
    fprintf(fp, "Hello World!4\n\r");
    fprintf(fp, "Hello World!4\n\r");
    fclose(fp);
    uBit.serial.printf("File closed\n\r");
    
    DIR *d = opendir("/sd");               // Opens the root directory of the local file system
    struct dirent *p;
    while((p = readdir(d)) != NULL) {         // Print the names of the files in the local file system
       printf("%s\n\r", p->d_name);              // to stdout.
    }
    closedir(d);
    while (1)
    {
        uBit.sleep(1000);
    }

    //remove("/sd/out.txt");                 // Removes the file "out.txt" from the local file system
    
    //mkdir("/sd/mydir2", 0777); // This works ok
        
    //return 1;
    
    /*
    //Init(7);
    
    //SatBegin(1);
    char buffer[30];
    int fileincrement = 1;
    int ret = sprintf (buffer, "/sd/data%d.csv", fileincrement);
    uBit.serial.printf("Filename:%s\n", buffer);

    FILE *fp = fopen(buffer, "w+");  // Open "out.txt" on the local file system for writing "/sd/sdtest.txt"
    uBit.serial.printf("File opened");
    LogDataHeader(fp);
    uBit.serial.printf("header written");
    fclose(fp);
    
    while (1)
    {
        uBit.serial.printf(".");
        uBit.display.scrollAsync("Reading",100);
        FILE *fp = fopen(buffer, "a"); 
        uBit.serial.printf("File opened");
        //LogData(fp);
        uBit.serial.printf("Data logged");
        fclose(fp);            
        uBit.sleep(5000);
    }
    */
}