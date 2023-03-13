#include <iostream>

#pragma once

template<class dataType, class headType> 
class CircBuffer {
    using uint = unsigned int;
    
public:
    CircBuffer (uint size);
    ~CircBuffer();

    //changes the size of the buffer in samples
    void setSize(uint size);

    //return the size of the buffer in samples
    uint getSize() const;

    //writes to the buffer at the current writeHead position
    void writeSample(dataType value);

    //reads from the buffer at writeHead - delay
    dataType readSample(headType delay);

    //increments the writeHead with 1
    void incrementWrite();

    //returns the writeHead position
    uint getWritePosition() const;    


private:
    inline int wrapRead(headType head);
    inline void wrapWriteHeader (uint& head);
    inline void deleteBuffer();


    bool delayStarted { false }, waitingForResize { false };
    bool changeIncrement { false };

    dataType* buffer;
    uint currentSize { 0 };
    dataType* newBuffer;
    uint writeHead { 0 };
    headType readMax { 0 };

};