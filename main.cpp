// Basic example show how to read a tag uid.
// you can use i2c, spi, hsu interface.
//
// Assembled by dotnfc as Arducleo Sample
// 2016/09/10



#include "mbed.h"
#include "PN532.h"
#include "PN532_HSU.h"
#include "PN532_SPI.h"
#include "PN532_I2C.h"

DigitalOut ledBrd (LED1);   // arducleo onboard led

DigitalOut ledNFC (D9);     // status led 

DigitalOut rstNFC (D4);     // pn532 chip reset control

PwmOut buz(PB_1);

Serial pc(SERIAL_TX, SERIAL_RX);

// ----------------------------------------- HSU
//HardwareSerial pn532_hsu (PC_10, PC_11);
//PN532_HSU pn532_if (pn532_hsu);

// ----------------------------------------- SPI
SPI pn532_spi (SPI_MOSI, SPI_MISO, SPI_SCK);
PN532_SPI pn532_if (pn532_spi, SPI_CS);

// ----------------------------------------- I2C
//I2C pn532_i2c (I2C_SDA, I2C_SCL);
//PN532_I2C pn532_if (pn532_i2c);



PN532 nfc(pn532_if);

/*==============================================================================
 * \brief reset the pn532 chip
 */
void reset_chip (void)
{
    rstNFC = 0;
    wait_ms (100);
    rstNFC = 1;
}


/*==============================================================================
 * \brief init the peripheral
 */
void setup(void) 
{
    ledBrd = 0;
    ledNFC = 0;
    reset_chip ();
    
    uint32_t versiondata = 0;
    pc.baud(115200);
    pc.printf ("Hello!\n");

    while (1) {
        nfc.begin();
        //nfc.SAMConfig();
        versiondata = nfc.getFirmwareVersion();
        if (! versiondata) {
            pc.printf("Didn't find PN53x board\n\n");
            wait_ms(500);
        }
        else {
            break;
        }
    }
    
    // Got ok data, print it out!
    pc.printf ("Found chip PN5%02X , Firmware ver. %d.%d\n", 
            (versiondata>>24) & 0xFF, 
            (versiondata>>16) & 0xFF, 
            (versiondata>>8) & 0xFF);

    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    nfc.setPassiveActivationRetries(0xFF);

    // configure board to read RFID tags
    nfc.SAMConfig();

    pc.printf ("\nWaiting for an ISO14443A card\n");
}


/*==============================================================================
 * \brief find a tag
 */
void loop(void) 
{
    bool success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;  // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // configure board to read RFID tags
    nfc.SAMConfig();

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

    printf ("\n");  
  
    if (success) {
        tone (buz, 800);        // turn on the buzzer
        ledNFC = 1;             // led on
        
        pc.printf("Found a card!\n");
        
        pc.printf("UID Length: %d bytes\n", uidLength);
        pc.printf("UID Value: ");
        
        for (uint8_t i=0; i < uidLength; i++) 
            pc.printf(" 0x%02X", uid[i]);

        pc.printf("\n");
        
        wait_ms (100);
        tone (buz, 0);          // turn off the buzzer
        
        // wait until the card is taken away
        while (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 500)) {}
        
        ledNFC = 0;         // led off
    }
    else {
        // PN532 probably timed out waiting for a card
        pc.printf("\nTimed out waiting for a card\n");
        ledNFC = 0;
        wait_ms (200);  
    }    
}


/*==============================================================================
 * \brief main entry
 */
int main()
{
    setup();
    
    while (1) 
        loop ();
}

