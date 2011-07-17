#include "MemStream.h"

MemStream::MemStream(Memory* memory, unsigned int address, unsigned int bufferSize)
{
  _memory = memory;
  _address = address;
  _bufferSize = bufferSize;
  _head = _tail = 0;
}

int MemStream::available(void)
{
  return (unsigned int)(_bufferSize + _head - _tail) % _bufferSize;
}

int MemStream::peek(void)
{
  if (_head == _tail) {
    return -1;
  } else {
    return _memory->read(_address + _tail);
  }
}

int MemStream::read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (_head == _tail) {
    return -1;
  } else {
    unsigned char c = _memory->read(_address + _tail);
    _tail = (unsigned int)(_tail + 1) % _bufferSize;
    return c;
  }
}

void MemStream::flush()
{
  _head = _tail;
}

void MemStream::write(uint8_t c)
{
  unsigned int i = (unsigned int)(_head + 1) % _bufferSize;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != _tail) {
    _memory->write(_address + _head,c);
    _head = i;
  }
}


