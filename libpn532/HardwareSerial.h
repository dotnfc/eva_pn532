/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __HARDWARESERIAL_H
#define __HARDWARESERIAL_H

#include "mbed.h"
#include <inttypes.h>
#include "BufferedSerial.h"


class HardwareSerial : public BufferedSerial
{
  public:
      HardwareSerial (PinName tx, PinName rx) : BufferedSerial (tx, rx) {
          
      }
    //virtual void begin(unsigned long) = 0;
    //virtual void end() = 0;
    //virtual int available(void) = 0;
    //virtual int peek(void) = 0;
    //virtual int read(void) = 0;
    //virtual void flush(void) = 0;
//    inline size_t write(uint8_t val) {
//        putc (val);
//        return 1;
//    }
    
//    inline size_t write(const uint8_t *buffer, size_t size) {
//        size_t i ;
//        for (i = 0; i < size; i ++)
//            putc (buffer[i]);
//        
//        return size;
//    }
    
    //using Print::write; // pull in write(str) and write(buf, size) from Print
    //virtual operator bool() = 0;
};

extern void serialEventRun(void) __attribute__((weak));

#endif // __HARDWARESERIAL_H

