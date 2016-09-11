
#ifndef __PN532_I2C_H__
#define __PN532_I2C_H__

#include <I2C.h>
#include "PN532Interface.h"

#define PN532_I2C_ADDRESS       0x48

class PN532_I2C : public PN532Interface {
public:
    PN532_I2C(mbed::I2C &wire);
    
    virtual void begin();
    virtual void wakeup();
    virtual int8_t writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body = 0, uint8_t blen = 0);
    virtual int16_t readResponse(uint8_t buf[], uint8_t len, uint16_t timeout = 1000);

private:
    mbed::I2C* _wire;
    uint8_t command;
    
    int8_t readAckFrame();
};

#endif
