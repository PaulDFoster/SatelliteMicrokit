/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MicroBit.h"
#include "global.h"
#include "camera_basic.h"
#include "SDFileSystem.h"

void onData(MicroBitEvent e);

bool send = false;
bool boardPresent = false;
bool buttonA = false;
bool buttonB = false;
bool takePicture = false;
int timer = 10;
int group = 8;
int state = 0;

MicroBitImage smiley("0,255,0,255, 0\n0,255,0,255,0\n0,0,0,0,0\n255,0,0,0,255\n0,255,255,255,0\n");
MicroBitImage sadsmiley("0,255,0,255, 0\n0,255,0,255,0\n0,0,0,0,0\n0,255,255,255,0\n255,0,0,0,255\n");
MicroBitImage notentry("0,255,255,255,0\n255,255,0,0,255\n255,0,255,0,255\n255,0,0,255,255\n0,255,255,255,0\n");
MicroBit        uBit;

SDFileSystem sd(MICROBIT_PIN_P13, MICROBIT_PIN_P14, MICROBIT_PIN_P15, MICROBIT_PIN_P16, "sd"); // MOSI, MISO, SCLK, SSEL


int TakePicture(int i)
{
    uBit.serial.printf("\rStarting Camera\n\r");
    if(sacsik::camera::setup())
        {
            //uBit.serial.printf("Setting image size\n");
            //sacsik::camera::system_reset();
            //sacsik::camera::set_Image_Size(0x22);
            uBit.serial.printf("\n\rStart pic\n\r");
            sacsik::camera::fbuf_ctrl(0x02);
            uBit.serial.printf("\n\rtake pic\n\r");
            sacsik::camera::fbuf_ctrl(0x00);
            uBit.serial.printf("\n\rget length\n\r");
            unsigned int t = sacsik::camera::get_fbuf_len();
            uBit.serial.printf("\n\rSave\n\r");
            sacsik::camera::read_fbuf(t,i); 
            uBit.serial.printf("\n\rStop capture\n\r");
            sacsik::camera::fbuf_ctrl(0x03);
            return 1;
        }
     else
     {
        printf("Take picture failed");
        return 0;
     }
}

void onData(MicroBitEvent e)
{
    
    PacketBuffer buf = uBit.radio.datagram.recv();
    // Need to check that camera command is ack'ed or reset takePicture
    
    // case for D-ACK and Ping
    switch (buf[0])
    {
        case 100: //D
        case 68:
            state = 1;
            uBit.radio.datagram.send("D");
            takePicture = true;
            // Update screen to be camera in action
        break;
        case 112://P
        case 80:
            if(!takePicture)
                if(state!=2)
                    state = 0;
            uBit.radio.datagram.send("P");
            // reset timer
            timer = 10;
        break;
        
    }
    
        switch (state)
    {
        case 0:
            uBit.display.scrollAsync("~");
        break;
        case 1:
            uBit.display.scrollAsync(smiley);
        break;
        case 2:
            uBit.display.stopAnimation();
            uBit.display.print(smiley);
        break;
    }

}

void onButtonAHold(MicroBitEvent)
{
    buttonA = true;
}

void onButtonBHold(MicroBitEvent)
{
    buttonB = true;
       
}

void onButtonAUp(MicroBitEvent)
{
    buttonA = false;
}

void onButtonBUp(MicroBitEvent)
{
    buttonB = false;
       
}

int main()
{
    uBit.init();
    uBit.radio.setGroup(group);
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);
    uBit.radio.setTransmitPower(6);
    uBit.radio.enable();
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_HOLD, onButtonAHold);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_HOLD, onButtonBHold);    
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_UP, onButtonAUp);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_UP, onButtonBUp);
    
    uBit.display.scroll("On");
    
    // Configure environment
   
   int imageincrement = 0;
   int ret = 0;
 
    while(1)
    {
        if (buttonA && buttonB)
        {
            uBit.display.print(smiley);
            wait_ms(60000);
            takePicture = true;
        }
        if(takePicture)
        {
            state = 1;
            uBit.display.printAsync(notentry);
            uBit.radio.datagram.send("F");
            ret = TakePicture(imageincrement);
            if(ret==1)
            {
                imageincrement++;
                state = 2;
                uBit.radio.datagram.send("C");
                // Update screen to say picture taken
                uBit.display.scrollAsync(smiley);
            }
            else
            {
                state = 3;
                uBit.radio.datagram.send("E");
                // Update screen to say picture taken
                uBit.display.scrollAsync(sadsmiley);
            }
            takePicture = false;
        }
        timer--;
        
        if(state == 0){         
            if(timer<1)
            {
                //Update screen to be broken connection
                uBit.display.stopAnimation();
                uBit.display.print("X");
            }
        }
        
        uBit.sleep(10); //was 100
    }

}

