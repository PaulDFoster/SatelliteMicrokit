#include "tmp102.h"

//namespace touch_develop {
namespace tmp102 {

 using namespace ::touch_develop::i2c;

  I2CSimple tmp102_i2c(72);
      
//#define TMP102_I2C_ADDRESS 72 /* This is the I2C address for our chip.
//This value is correct if you tie the ADD0 pin to ground. See the datasheet for some other values. */
void test ()
{
    int t = getTemp102();
    
}

int getTemp102(){
  uint8_t firstbyte, secondbyte; //these are the bytes we read from the TMP102 temperature registers
  int val; /* an int is capable of storing two bytes, this is where we "chuck" the two bytes together. */ 
  float convertedtemp; /* We then need to multiply our two bytes by a scaling factor, mentioned in the datasheet. */ 
  float correctedtemp; 
  // The sensor overreads (?) 

  /* Reset the register pointer (by default it is ready to read temperatures)
You can alter it to a writeable register and alter some of the configuration - 
the sensor is capable of alerting you if the temperature is above or below a specified threshold. */
  //MicroBitI2C I2C(MICROBIT_PIN_SDA, MICROBIT_PIN_SCL);
  
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
} // namespace tmp102
//} // namespace touch_develop