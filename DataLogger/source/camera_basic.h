#include "MicroBit.h"
#include "global.h"
#include "I2CCommon.h"
#include "SC16IS750.h"

#ifndef __MICROBIT_CAMERA_BASIC_H
#define __MICROBIT_CAMERA_BASIC_H
namespace sacsik {
namespace camera {
   
   bool setup();
   bool gen_version();
   void set_port(int baud);
   void system_reset();
   void fbuf_ctrl(short mode);
   void read_fbuf(unsigned int len, int imageincrement);
   void read_data(short reg);
   unsigned int get_fbuf_len();
   void set_Image_Size(uint8_t size);
   void set_startup_baud();
   void Set_Image_Compression(uint8_t size);
}
}    
#endif