#ifndef ROVERINGBUFFER_H_
#define ROVERINGBUFFER_H_

#include <stdint.h>
#include <stddef.h>

/////////////////////////////////////////////
template<class TYPE> class RoveRingBuffer
{
  private:

    TYPE*    _buffer_pointer; 
    size_t   _buffer_size;
  
    uint32_t _push_index;
    uint32_t _pop_index;
    size_t   _count;

  public:  
     
    RoveRingBuffer(TYPE buffer[]);  
    
    void push(TYPE push);  
    TYPE pop();
  
    bool     isEmpty();
    bool     isFull();
    size_t   count(); 
};

//////////////////////////////////////////////////
template<class TYPE> RoveRingBuffer<TYPE>::RoveRingBuffer(TYPE buffer[])
{
   _buffer_pointer = buffer;
   _buffer_size    = sizeof(buffer);
}

///////////////////////////////////////////////////
template<class TYPE> void RoveRingBuffer<TYPE>::push(TYPE push)
{
  if(isFull())
  {
    return;

  } else {

    _buffer_pointer[_push_index] = push;
    
    _push_index = (_push_index + 1) % _buffer_size;
    _count++;
    
    return;
  }
}

//////////////////////////////////////////////////
template<class TYPE> TYPE RoveRingBuffer<TYPE>::pop()
{
  if(isEmpty())
  {
    return 0;

  } else {

    TYPE pop_byte = _buffer_pointer[_pop_index];
    
    _pop_index = (_pop_index + 1) % _buffer_size;
    _count--;
    
    return pop_byte;
  }
}

//////////////////////////////
template<class TYPE> bool RoveRingBuffer<TYPE>::isEmpty()
{
  return (_count == 0);
}

////////////////////////////////////////////////////////
template<class TYPE> bool RoveRingBuffer<TYPE>::isFull()
{
  return ((_push_index == _pop_index) && (_count != 0));
}

///////////////////////////////
template<class TYPE> size_t RoveRingBuffer<TYPE>::count()
{
  return _count;
}

#endif // ROVERINGBUFFER_H_
