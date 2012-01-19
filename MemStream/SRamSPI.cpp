/*
writestream: Setup the SRAM in sequential write mode starting from the passed address.
             Data can then be written one byte at a time using RWdata(byte data).
			 Each byte is stored in the next location and it wraps around 32767.

readstream:  Setup the SRAM in sequential read mode starting from the passed address.
             Data can then be read one byte at a time using  byte RWdata(0).The passed data is irrelavent.
			 Each byte is read from the next location and it wraps around 32767.

RWdata:      Write or read the data from the SRAM.
             If the writesteam is open the passed data will be written to the current address.
			 If the readstream is open the data from the current address will be returned.

closeRWstream: Use to close the  open read or write stream.
               Dont need when changing between read/write.
               Close before using SPI somewhere else.
New addition - inherited from the Memory class, used by the MemStream class
read:        Open a readstream, read a byte and close the stream
write:       Open a writestream, write a byte and close the stream

digital pin 13    SCK
digital pin 12    MISO
digital pin 11    MOSI
specified in constructor    SS
*/

#include "WProgram.h"
#include "SRamSPI.h"

SRamSPI::SRamSPI(byte CSPin)  //constructor
{
  csPin = CSPin;
  setupDDRB;
  digitalWrite(10,HIGH); //required to setup SPI
  setupSPI;
  pinMode(csPin , OUTPUT);
  selectSS;
  RWdata(0x05);//read status register
  int Sreg = RWdata(0xff);//get status value
  deselectSS;

  if(Sreg != 0x41) //are we in sequential mode
  {
    selectSS;
    RWdata(0x01); //write to status reg
    RWdata(0x41);  //set sequencial  mode
    deselectSS;
  }//end of set sequential mode
  //deselectSS;
}//end of constructor

void SRamSPI::writestream(unsigned int address)
{
  deselectSS;  //deselect if still selected
  selectSS; //select now
  RWdata(0x02);//write to address
  RWdata(address >> 8);//msb address
  RWdata(address);//lsb address 
  
}//end of write stream

void SRamSPI::readstream(unsigned int address)
{
  deselectSS;  //deselect if still selected
  selectSS; //select now
  RWdata(0x03);//read from address
  RWdata(address >> 8);//read from address
  RWdata(address); 
}//end of read stream

void SRamSPI::closeRWstream(void)
{
  deselectSS;  //deselect
}//end of close RW stream

byte SRamSPI::RWdata(byte data)
{
  SPDR = data;
  while (!(SPSR & _BV(SPIF)))
    ;
  return SPDR;
}//end of RWdata

uint8_t SRamSPI::read(unsigned int address)
{
  uint8_t c;
  readstream(address);
  c = RWdata(0xFF);
  closeRWstream();
  return c;
}

void SRamSPI::write(unsigned int address, uint8_t c)
{
  writestream(address);
  RWdata(c);
  closeRWstream();
}

