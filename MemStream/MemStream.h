#ifndef MEMSTREAM_h
#define MEMSTREAM_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class Memory
{
  public:
    virtual uint8_t read(unsigned int address) = 0;
    virtual void write(unsigned int address, uint8_t c);
};


class MemStream : public Stream
{
  private:
    unsigned int _address;
    unsigned int _bufferSize;
    unsigned int _head;
    unsigned int _tail;
    Memory* _memory;
  public:
    MemStream(Memory* memory, unsigned int address, unsigned int bufferSize);
    virtual int available(void);
    virtual int peek(void);
	int peek(unsigned int address);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    using Print::write; // pull in write(str) and write(buf, size) from Print
};

#endif
