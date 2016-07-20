#include "MicroBit.h"
#include "camera_basic.h"
#include "SC16IS750.h"

namespace sacsik {
namespace camera {
    
sacsik::SC16IS750::SC16IS750 i2cuart = sacsik::SC16IS750::SC16IS750(sacsik::SC16IS750::SC16IS750_PROTOCOL_I2C, sacsik::SC16IS750::SC16IS750_ADDRESS_BB);

bool menu_active = true, run_mode = false;;
long int menu_val = 0;

//Connect TX and RX with a wire and run this sketch

bool setup()
{
  i2cuart.begin(115200);               //baudrate setting
  if (i2cuart.ping() != 1) {
    //device not found
    uBit.serial.printf("i2cuart ping failed\n\r");
    
    return false;
  }
  uBit.serial.printf("i2cuart ping ok\n\r");
  
  
  //set_startup_baud(); //Permanently set baud rate. Requires power off. This works to 115k
  //wait_ms(2500);
  //set_port(115200); // Temporarily set compression doesn't work because of i2cuart
  //uBit.serial.printf("baud set\n\r");
  
  
  set_Image_Size(0x11); //0x00 640x480 0x11 320x240 0x22 160x120
  uBit.serial.printf("image set\n\r");
  system_reset(); // required to get image size set
  uBit.serial.printf("\n\rcamera reset\n\r");
  
  // Set compression
  //Set_Image_Compression(0);
  //uBit.serial.printf("\n\rImage compression set\n\r");
  
  return true;
  //Setup complete
};

bool gen_version() {
  printf("Get version\n\r");
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
      uBit.serial.printf("UART timeout\n\r");
      //uBit.display.scroll("UART F");
      wait_ms(50);
      break;
    }
    
    uBit.serial.printf("Waiting for UART %d\n\r",timeout);
  }

  if (timeout < 10) {
      //uBit.display.scroll("Gen T");
    while (i2cuart.available() > 0) {
      i2cuart.read(); // clearing data buffer
      printf("Reading response\n\r");
      //int ret_val = i2cuart.read();
      //uBit.display.scroll(ManagedString(i2cuart.read()));
      //Serial.print(char(ret_val));
      //uBit.display.scroll(".");
    }
  }
  else {
    //Timeout
    //uBit.display.scroll("C:Timeout");
    printf("Timeout Get Version failed\n\r");
    err = false;
  }
  return err;
}

void set_startup_baud()
{
  
  printf("Setting 115k\n\r");
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
    uBit.serial.printf("waiting for UART\n\r");
    wait_ms(50);
  }

  while (i2cuart.available() > 0) {
    int ret_val = i2cuart.read();
    uBit.serial.printf("Reading UART %02x\n\r", ret_val);
    }
}

void set_port(int baud) {

  printf("Setting 115k\n\r");
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
    uBit.serial.printf("waiting for UART\n\r");
    wait_ms(50);
  }

  while (i2cuart.available() > 0) {
    int ret_val = i2cuart.read();
    uBit.serial.printf("Reading UART %02x\n\r", ret_val);
    }
   
  i2cuart.ResetDevice();
  i2cuart.begin(baud);
  
  if (i2cuart.ping() != 1) {
    uBit.serial.printf("i2cuart ping failed\n\r");
  }
  else
    uBit.serial.printf("i2cuart ping ok\n\r");
    
}

void system_reset() {

  i2cuart.write(0x56);
  i2cuart.write(0x00);
  i2cuart.write(0x26);
  i2cuart.write(0x00);

  while (i2cuart.available() == 0)
  {
    uBit.serial.printf(".");
    wait_ms(100);
  }
  //Serial.println();
  while (i2cuart.available() > 0) {
    int ret_val = i2cuart.read();
    uBit.serial.printf("%02x ",ret_val);
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
    uBit.serial.printf(".");
    wait_ms(100);
  }

  while (i2cuart.available() > 0) {
    int ret_val = i2cuart.read();
    uBit.serial.printf("%02x",ret_val);
    }
}

void read_fbuf(unsigned int len, int imageincrement) {

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

void set_Image_Size(uint8_t size)
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
  
}

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
    uBit.serial.printf("%02x ",ret_val[i]);
  }

  len = (ret_val[7] << 8) | ret_val[8];

  return len;
}

} //satellite_kit
} // touch_develop

