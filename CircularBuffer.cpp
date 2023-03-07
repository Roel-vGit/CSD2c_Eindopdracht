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
    //initialize write- and readHeader wrap values to buffer size
    writeMax = size; 
    readMax = size;

    for (uint i = 0; i < size; i++)
        buffer[i] = 0.0;

}

//Destructor
template<class dataType, class headType>
CircBuffer<dataType, headType>::~CircBuffer()
{
    delete[] buffer; 
}


//changes the size of the buffer
template<class dataType, class headType>
void CircBuffer<dataType, headType>::setSize(uint size)
{
    //----------------------------------------------------------------
    //when new size equals old size
    if (size == currentSize)
        return; //exit function
    
    //----------------------------------------------------------------
    //when new size is bigger than old size
    newBuffer = new dataType[size]; //create new buffer
    
    if (size > currentSize)
    {
        std::copy(buffer, buffer+currentSize, newBuffer); //copy contents of old buf to new buf
        delete[] buffer; //delete old buf
        buffer = newBuffer; //set new buffer to the buffer we will be using
        currentSize = size; //adjust size 
        writeMax = size; //set wrap value for write header to new buffer size
    }

    //----------------------------------------------------------------
    //when new size is smaller than new size
    if (size < currentSize)
    {
        if (distance > size)
        {
            setDistance(size, false); //decrease delay time when new bufferSize is smaller than the current delay time
        }

        if ((writeHead > size && readHead < size) || (writeHead > size && readHead > size))
        {
            std::copy(buffer, buffer+size, newBuffer);
            readMax = writeHead;
            writeHead = 0; //set writeHead to begin of buffer 
            writeMax = size;
            waitingForResize = true; //if true, we wait with resizing until the writeHead is in front of the readHead
            newSize = size;
        } 
        else if (readHead > size && writeHead < size)
        {
            std::copy(buffer, buffer+size, newBuffer);
            waitingForResize = true;
            writeMax = size;
            newSize = size;
        } 
        else if (readHead < size && writeHead < size)
        {
            std::copy(buffer, buffer+size, newBuffer);
            writeMax = size;
            readMax = size;
            delete[] buffer;
            buffer = newBuffer;
            currentSize = size;
            newSize = size;
        }
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

    //return 0 if the buffer at index writeHead - delay is not filled yet
    if (!delayStarted)
        return 0.0f;
    else 
    {
        if (delay > writeHead)
            delay = currentSize - delay + writeHead;
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
            dataType high = buffer[readBuffer((int)delay + 1)];
            headType fraction = delay - (int)delay;
            dataType sample = Util::linearMap(fraction, low, high);
            return sample;
        }
    } 
}

//Increments the heads 1 position further
template<class dataType, class headType>
void CircBuffer<dataType, headType>::incrementHeads() 
{
    incrementWrite();
}

template<class dataType, class headType>
uint CircBuffer<dataType, headType>::getWritePosition() const
{
    return writeHead;
}

template<class dataType, class headType>
inline void CircBuffer<dataType, headType>::wrapWriteHeader(uint& head)
{
    if (head >= writeMax)
        head -= writeMax;
}

template<class dataType, class headType>
inline void CircBuffer<dataType, headType>::incrementWrite() 
{
    writeHead++;
    wrapWriteHeader(writeHead);

    if (writeHead > distance)
        delayStarted = true; //start reading after distance (delay) had been reached
}

template<class dataType, class headType>
void CircBuffer<dataType, headType>::deleteBuffer() 
{
        delete[] buffer;
}

/*wraps the readPos
*/
template<class dataType, class headType>
inline int CircBuffer<dataType, headType>::readBuffer(headType head)
{
    if (head >= readMax)
    head -= readMax;
    return head;
}
