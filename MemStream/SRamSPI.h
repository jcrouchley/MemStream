/*
Based upon the 
SRAM lib written by by Carl Morey. Adaptations: Marcus Hund aka Marko Hiero
writestream: Setup the SRAM in sequential write mode starting from the passed address.
             Bytes can then be written one byte at a time using RWdata(byte data).
			 Each byte is stored in the next location and it wraps around 32767.

readstream:  Setup the SRAM in sequential read mode starting from the passed address.
             Bytes can then be read one byte at a time using  byte RWdata(0).The passed data is irrelavent.
			 Each byte is read from the next location and it wraps around 32767.

RWdata:      Write or read a byte at any time from the SRAM.
             If the writesteam is open the passed byte will be written to the current address.
			 If the readstream is open the byte from the current address will be returned.

closeRWstream: Use to close the  open read or write stream.
               Dont need when changing between read/write.
			   Close before using SPI somewhere else.

New addition - inherited from the Memory class, used by the MemStream class
read:        Open a readstream, read a byte and close the stream
write:       Open a writestream, write a byte and close the stream

digital pin 13    SCK
digital pin 12    MISO
digital pin 11    MOSI
digital pin 9     SS
*/
#ifndef SRAMSPI_h
#define SRAMSPI_h

#define setupSPI SPCR = 0x50      //Master mode, MSB first, SCK phase low, SCK idle low, clock/4
#define setupDDRB DDRB |= 0x2c    //set  SCK(13) MOSI(11) and SS as output 
#define selectSS digitalWrite(csPin,LOW);  //set the SS to 0 to select 
#define deselectSS digitalWrite(csPin,HIGH);  //set the SS to 1 to deselect 
#include "WProgram.h"
#include "MemStream.h"

class SRamSPI: public Memory
{
  private:
    byte csPin;
  public:

    SRamSPI(byte CSPin);  //the constructor
    void writestream(unsigned int address);
    void readstream(unsigned int address);
    void closeRWstream(void);
    uint8_t RWdata(uint8_t data);
    virtual uint8_t read(unsigned int address);
    virtual void write(unsigned int address, uint8_t c);
};//end of class SRamSPI

#endif

