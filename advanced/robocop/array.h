#ifndef __ARRAY_H__
#define __ARRAY_H__

// change the size of the array index type
// based on the number of elements you expect to add
// uint8_t for < 256; uint16_t for >= 256

typedef uint8_t ArrayIndex_t;

template <typename T>
class Array
{
  private:
    T *array;
    ArrayIndex_t  capacity; // number of slots in the array
    ArrayIndex_t  head; // current location to pull off front of list
    ArrayIndex_t  tail; // current location to insert on end of list
    
  public:
    Array(ArrayIndex_t capacity = 10);
    ~Array();

    ArrayIndex_t size() const { return (capacity); }
    ArrayIndex_t count() const { return (((tail+capacity)-head) % capacity); }
    
    bool isEmpty() const { return (head==tail); }
    bool isFull() const { return ((tail+1)%capacity == head); }
    
    void flush(void) { head = tail = 0; };	// force array to be empty
    
    void push(const T v);	// push onto the tail
    const T pop();				// pop off the tail
    const T peekPop();			// peek the tail
    
    void unshift(const T v);	// push onto the head
    const T shift();					// pop off the head
    const T peekShift();				// peek at the head
};

template<typename T>
Array<T>::Array(ArrayIndex_t _capacity)
{
  capacity = _capacity;
  array = new T[capacity];
  head = tail = 0;
}

template<typename T>
Array<T>::~Array()
{
  delete array;
}

template<typename T>
void Array<T>::push(const T v)
{
  if (isFull())
    return;

  array[tail] = v;
  tail = (tail+1)%capacity;
}

template<typename T>
const T Array<T>::pop()
{
  if (isEmpty())
    return *(T *)0;

  tail = (tail == 0) ? capacity-1 : tail-1;
  return array[tail];
}

template<typename T>
const T Array<T>::peekPop()
{
  if (isEmpty())
    return *(T *)0;

  return array[(tail == 0) ? capacity-1 : tail-1];
}

template<typename T>
void Array<T>::unshift(const T v)
{
  if (isFull())
    return;

  head = (head == 0) ? capacity-1 : head-1;
  array[head] = v;
}

template<typename T>
const T Array<T>::shift()
{
  if (isEmpty())
    return *(T *)0;

  T v = array[head];
  head = (head+1)%capacity;

  return v;
}

template<typename T>
const T Array<T>::peekShift()
{
  if (isEmpty())
    return *(T *)0;

  return array[head];
}

#endif