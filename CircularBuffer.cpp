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
void CircBuffer<dataType, headType>::input (dataType value) 
{
    // std::cout << "Write: " << value << std::endl;
    buffer[writeHead] = value;
}

//Reads from the buffer and interpolates decimal values
template<class dataType, class headType>
dataType CircBuffer<dataType, headType>::output()
{
    if (!delayStarted)
    {
        // std::cout << "Read: " << "0" << std::endl;
        return 0.0;
    }
    else 
    {
        if (std::is_same<headType, int>::value || std::is_same<headType, uint>::value)
        {
            return buffer[(int)readHead];
        }
        else 
        {
            dataType low = buffer[(int)readHead];
            dataType high = buffer[readBuffer((int)readHead + 1)];
            headType fraction = readHead - (int)readHead;
            dataType sample = Util::linearMap(fraction, low, high);
            return sample;
        }
    }
}

//Sets the delay instantly
template<class dataType, class headType>
void CircBuffer<dataType, headType>::setDistance (headType distance, bool move) 
{   
    if (move)
    {
        if (distance < currentSize)
            newDistance = distance;
        else
            newDistance = currentSize-1;

        changeDistance = true;
    }
    else 
    {
        if (distance < currentSize) //if the new distance is greater than the buffer size
            this->distance = distance;            
        else
            this->distance = currentSize;


        if (!delayStarted)
            readHead = 0.0; //wait with incrementing the readhead until delay time is reached (when we first start the delay)
        else 
        {
            if (writeHead > distance) //if true then the writehead is in front of the readhead in the buffer
                readHead = writeHead - this->distance;
            else
                readHead = currentSize - this->distance + writeHead; //wrap readHead to end if it's in front of writeHead
        }
    }

}

//Returns the current delay time in samples
template<class dataType, class headType>
headType CircBuffer<dataType, headType>::getDistance() const
{
    if (writeHead < readHead)
        return currentSize - readHead + writeHead;
    else
        return (writeHead - readHead);
}


//Increments the heads 1 position further
template<class dataType, class headType>
void CircBuffer<dataType, headType>::incrementHeads() 
{
    incrementWrite();
    incrementRead();

    if (changeDistance) //if we are gradually moving to a new delay time
    {
        distance = getDistance();
        if (distance < newDistance) //if new distance is larger than old distance
            readIncrement = 0.5;
        else if (distance > newDistance)
            readIncrement = 2.0;
        else if (distance == newDistance)
        {
            readIncrement = 1.0;
            changeDistance = false;
        }
    }
}

template<class dataType, class headType>
headType CircBuffer<dataType, headType>::getReadPosition() const
{
    return readHead;
}

template<class dataType, class headType>
uint CircBuffer<dataType, headType>::getWritePosition() const
{
    return writeHead;
}

template<class dataType, class headType>
inline void CircBuffer<dataType, headType>::wrapReadHeader (headType& head) 
{
    if (head <= writeHead) 
        readMax = currentSize; //if buffer is resized (larger than old buf) when readHead is > writeHead then it wraps the readHead based on the old buffer size

    if (head >= readMax)
        head -= readMax;
}

/*wraps the readHead without changing the actual readhead
function is used in output() function for interpolation
*/
template<class dataType, class headType>
inline int CircBuffer<dataType, headType>::readBuffer(headType head)
{
    if (head >= readMax)
    head -= readMax;
    return head;
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
inline void CircBuffer<dataType, headType>::incrementRead() 
{
    if (delayStarted)
    {
        readHead += readIncrement;
        wrapReadHeader(readHead);
    }

    if (waitingForResize) //wait for resizing until writeHead is in front of the readhead
    {
        if (readHead <= writeHead)
        {
            delete[] buffer;
            buffer = newBuffer;
            waitingForResize = false;
            readMax = newSize;
            writeMax = newSize;
            currentSize = newSize;
        }
    }
}

template<class dataType, class headType>
void CircBuffer<dataType, headType>::deleteBuffer() 
{
        delete[] buffer;
}


