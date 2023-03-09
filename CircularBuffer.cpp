#include "CircularBuffer.h"
#include "Util.h"
#include <type_traits>

#include <iostream>
#include <unistd.h>
#include <thread>
#include <cmath>


//Constructor
template<class dataType, class headType>
CircBuffer<dataType, headType>::CircBuffer(uint size) : buffer (new dataType[size]), currentSize (size), newBuffer(nullptr)
{ 
    //initialize writeHeaderto buffer size
    readMax = size;

    //initialize all samples to 0
    for (uint i = 0; i < size; i++)
        buffer[i] = 0.0;
}

//Destructor
template<class dataType, class headType>
CircBuffer<dataType, headType>::~CircBuffer()
{
    deleteBuffer();
}


//changes the size of the buffer
template<class dataType, class headType>
void CircBuffer<dataType, headType>::setSize(uint size)
{
    newBuffer = new dataType[size];

    if (size == currentSize)
        return; //exit function if size is not changed
    else
    {
        if (size < currentSize)
            std::copy(buffer, buffer+size, newBuffer); //copy contents of old buf to new buf
        else
            std::copy(buffer, buffer+currentSize, newBuffer);

        delete[] buffer; //delete old buf
        buffer = newBuffer; //set new buffer to the buffer we will be using
        currentSize = size;
    }
}

template<class dataType, class headType>
uint CircBuffer<dataType, headType>::getSize() const
{
    return currentSize;
}



//Writes to the buffer
template<class dataType, class headType>
void CircBuffer<dataType, headType>::writeSample(dataType value) 
{
    // std::cout << "Write: " << value << std::endl;
    buffer[writeHead] = value;
}


/*Returns the sample value at Writeposition - delay
delay: delay in samples*/
template<class dataType, class headType>
dataType CircBuffer<dataType, headType>::readSample(headType delay)
{
    //limit delay to bufferSize
    if (delay > currentSize)
        delay = currentSize;
    
    //set delayStarted to true when the writePos has written the samples
    if (writeHead > delay)
        delayStarted = true;

    //return 0 if the buffer at index writeHead - delay is not filled yet
    if (!delayStarted)
        return 0.0f;
    else 
    {
        if (delay > writeHead)
            delay = readMax - delay + writeHead; //wrap readPos
        else 
            delay = writeHead - delay;
        
        //check if the readIndex is an int or uint
        if (std::is_same<headType, int>::value || std::is_same<headType, uint>::value)
        {
            return buffer[(int)delay];
        }
        else 
        {
            dataType low = buffer[(int)delay];
            dataType high = buffer[wrapRead((int)delay + 1)];
            headType fraction = delay - (int)delay;
            dataType sample = Util::linearMap(fraction, low, high);
            return sample;
        }
    } 
}

template<class dataType, class headType>
uint CircBuffer<dataType, headType>::getWritePosition() const
{
    return writeHead;
}

//wraps the readPos
template<class dataType, class headType>
inline int CircBuffer<dataType, headType>::wrapRead(headType head)
{
    if (head >= readMax)
    head -= readMax;
    return head;
}


template<class dataType, class headType>
inline void CircBuffer<dataType, headType>::wrapWriteHeader(uint& head)
{
    if (head >= currentSize)
        head -= currentSize;
}

template<class dataType, class headType>
void CircBuffer<dataType, headType>::incrementWrite() 
{
    writeHead++;
    wrapWriteHeader(writeHead);

    /*if statement that protects the user from reading positions that have not been written yet
    when the buffer has been resized (to a larger buffer). This wraps the readPos based on the 
    old buffer size until the write head has passed the old size*/
    if (writeHead > readMax)
        readMax = currentSize;
}

template<class dataType, class headType>
inline void CircBuffer<dataType, headType>::deleteBuffer() 
{
        delete[] buffer;
}

