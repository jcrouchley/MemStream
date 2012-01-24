#include <MemStream.h>
#include <SRamSPI.h>

#define MSBUFFSIZE 4096
#define TESTSIZE 1007
SRamSPI SR = SRamSPI(9);
MemStream MS = MemStream(&SR,0,MSBUFFSIZE);    // 1 k circular buffer (fifo)

void setup()
{
  Serial.begin(9600);
  Serial.println("Start test");
  for (int j=0; j<4096; j++)
  {
	  Serial.write('.');
	  for (unsigned int i=0; i<TESTSIZE; i++)
		MS.write((byte)i);
	  if (MS.available() != TESTSIZE)
	  {
		  Serial.println("Error in available");
		  Serial.print("Pass:"); Serial.print(j); Serial.print(" Avail:"); Serial.println(MS.available());
	  }
	  for (unsigned int i=0; i<TESTSIZE; i++)
	  {
		  if ( (byte)i != MS.read())
		  {
			  Serial.println("Error in read");
			Serial.print("Pass:"); Serial.print(j); Serial.print(" i:"); Serial.println(i);
		  }
	  }
  }
  Serial.println("Done");
}

void loop()
{
}

