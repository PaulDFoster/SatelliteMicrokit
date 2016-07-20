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


#include <SC16IS750.h>
namespace sacsik {
namespace SC16IS750 {

    //using namespace ::touch_develop::i2c;

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

} // namespace SC16IS750
} // namespace touch_develop