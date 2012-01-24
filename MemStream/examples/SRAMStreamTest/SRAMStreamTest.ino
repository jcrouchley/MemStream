#include "MemStream.h"
#include "SRamSPI.h"

#define MSBUFFSIZE 4096
SRamSPI SR = SRamSPI(9);
MemStream MS = MemStream(&SR,0,MSBUFFSIZE);    // 1 k circular buffer (fifo)

byte a=0x5A;
byte b;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
    a^=0xff;
  Serial.print("Test byte is 0x"); Serial.println(a,HEX);
  //SR.writestream(0);  // write to SRAM 1
  unsigned long stopwatch = millis(); //start stopwatch
  for(unsigned int i = 0; i < 32768; i++)
    SR.write(i,a); //write to every SRAM 1 address 
  Serial.print(millis() - stopwatch);
  Serial.println("   ms to write full SRAM 1");
  //SR.readstream(0);   // start address from 0 
  stopwatch = millis(); //start stopwatc
  for(unsigned int i = 0; i < 32768; i++)
  {
    b=SR.read(i);
    if(b != a)  //check every address in the SRAM1
    {
      Serial.print("error in SRAM 1 at location  ");
      Serial.print(i);
      Serial.print(" Expected 0x"); Serial.print(a,HEX);
      Serial.print(" Got 0x"); Serial.println(b,HEX);
      break;
    }//end of print error
    if(i == 32767)
    {
      Serial.print(millis() - stopwatch);
      Serial.println("   ms to read full SRAM 1");
      Serial.println("no errors for SRAM 1 in its 32768 bytes");
    }
  }//end of get byte
  Serial.println(" ");
  
  delay(1000);
  stopwatch = millis();
  for (unsigned int i=0; i<MSBUFFSIZE; i++)
    MS.write((byte)i);
  Serial.print(millis() - stopwatch);
  Serial.println("   ms to write full MS SRAM");
  stopwatch = millis();
  Serial.print("Memstream available:"); Serial.println(MS.available());
  for (unsigned int i=0; i<MSBUFFSIZE; i++)
  {
    b = MS.read();
    if ((byte)i != b)
    {
      Serial.print("error in MS SRAM at location  ");
      Serial.print(i);
      Serial.print(" Expected 0x"); Serial.print(i,HEX);
      Serial.print(" Got 0x"); Serial.println(b,HEX);
    }
  } 
  Serial.print(millis() - stopwatch);
  Serial.println("   ms to read full MS SRAM");
  Serial.println();

}
