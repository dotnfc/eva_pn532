////////////////////////////////////////////////////////////////////////////////
// pn532 i2c interface for mbed platform
// 
// by dotnfc@163.com
// 2016/09/10 18:16:00

#include "PN532_I2C.h"
#include "PN532_debug.h"
#include "Arduino.h"


PN532_I2C::PN532_I2C(mbed::I2C &wire)
{
    _wire = &wire;
    command = 0;
}

void PN532_I2C::begin()
{

}

void PN532_I2C::wakeup()
{

}

int8_t PN532_I2C::writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    command = header[0];
    MyBuffer <uint8_t> tBuf;
        
    tBuf.put (PN532_PREAMBLE);
    tBuf.put (PN532_STARTCODE1);
    tBuf.put (PN532_STARTCODE2);
    
    uint8_t length = hlen + blen + 1;   // length of data field: TFI + DATA
    tBuf.put (length);
    tBuf.put (~length + 1);                 // checksum of length
    
    tBuf.put (PN532_HOSTTOPN532);
    uint8_t sum = PN532_HOSTTOPN532;    // sum of TFI + DATA
    
    DMSG("write: ");
       
    for (uint8_t i = 0; i < hlen; i++) {
        tBuf.put (header[i]);
        sum += header[i];
        DMSG_HEX(header[i]);
    }

    for (uint8_t i = 0; i < blen; i++) {
        tBuf.put (body[i]);
        sum += body[i];
        DMSG_HEX(body[i]);
    }
  
    uint8_t checksum = ~sum + 1;            // checksum of TFI + DATA
    tBuf.put (checksum);
    tBuf.put (PN532_POSTAMBLE);
    
    _wire->write (PN532_I2C_ADDRESS, (const char *)tBuf.head(), tBuf.getLength());
    
    DMSG("\n");

    return readAckFrame();
}


int16_t PN532_I2C::readResponse(uint8_t buf[], uint8_t len, uint16_t timeout)
{
    uint8_t stat;
    uint16_t tlen = 256 + 5;
    MyBuffer <uint8_t> tBuf(256 + 5);
    Timer timer;
    if (timeout !=0) {
        timer.start();
    }
    
    do {
        if (_wire->read (PN532_I2C_ADDRESS, (char *)&stat, 1) == 0) {

            if (stat & 1) { // check the STATUS byte
                wait_ms (2);
                
                if (tlen > len)
                    tlen = len;
                    
                if (_wire->read(PN532_I2C_ADDRESS, (char *)tBuf.head(), tlen) == 0) {
                    break;
                }
            }
        }
        
        if (0 != timeout) {
            if (timer.read_ms() > timeout) {
                timer.stop ();
                return -1;
            }
        }

    } while (1); 
    
    if (0x01 != tBuf.get()      ||       // STATUS
        0x00 != tBuf.get()      ||       // PREAMBLE
        0x00 != tBuf.get()      ||       // STARTCODE1
        0xFF != tBuf.get()               // STARTCODE2
        ) {
        
        return PN532_INVALID_FRAME;
    }
    
    uint8_t length = tBuf.get();
    if (0 != (uint8_t)(length + tBuf.get())) {   // checksum of length
        return PN532_INVALID_FRAME;
    }
    
    uint8_t cmd = command + 1;               // response command
    if (PN532_PN532TOHOST != tBuf.get() || (cmd) != tBuf.get()) {
        return PN532_INVALID_FRAME;
    }
    
    length -= 2;
    if (length > len) {
        return PN532_NO_SPACE;  // not enough space
    }
    
    DMSG("read:  ");
    DMSG_HEX(cmd);
    
    uint8_t sum = PN532_PN532TOHOST + cmd;
    for (uint8_t i = 0; i < length; i++) {
        buf[i] = tBuf.get();
        sum += buf[i];
        
        DMSG_HEX(buf[i]);
    }
    DMSG("\n");
    
    uint8_t checksum = tBuf.get();
    if (0 != (uint8_t)(sum + checksum)) {
        DMSG("checksum is not ok\n");
        return PN532_INVALID_FRAME;
    }
    //tBuf.get();         // POSTAMBLE
    
    return length;
}

int8_t PN532_I2C::readAckFrame()
{
    const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};
    MyBuffer <uint8_t> ackBuf(sizeof(PN532_ACK) + 1);
    
    Timer timer;
    timer.start();
    
    do {
        // if (_wire->requestFrom(PN532_I2C_ADDRESS,  sizeof(PN532_ACK) + 1)) {
        if (_wire->read(PN532_I2C_ADDRESS, (char *)ackBuf.head(), sizeof(PN532_ACK) + 1) == 0) {
            if (ackBuf.get() & 1) {  // check first byte --- status
                break;         // PN532 is ready
            }
        }

        if (timer.read_ms() > PN532_ACK_WAIT_TIME) {
            DMSG("Time out when waiting for ACK\n");
            timer.stop ();
            return PN532_TIMEOUT;
        }
    } while (1);
    
    for (uint8_t i = 0; i < sizeof(PN532_ACK); i++) {
        if (ackBuf.get() != PN532_ACK[i])
            return PN532_INVALID_ACK;
    }
    
    return 0;
}
