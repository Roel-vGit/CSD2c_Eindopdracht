#include <iostream>
#include <thread>

#pragma once

//TODO: When changing delay time to bufferSize changeDistance is never set to false again.
template<class dataType, class headType> 
class CircBuffer {
    using uint = unsigned int;
public:
    CircBuffer (uint size);
    ~CircBuffer();

    void setSize(uint size);
    uint getSize() const;

    void writeSample(dataType value);
    dataType readSample(headType delay);

    void incrementHeads();
    uint getWritePosition() const;    


private:
    inline int readBuffer(headType head);
    inline void wrapWriteHeader (uint& head);
    inline void incrementWrite();
    void deleteBuffer();


    bool delayStarted { false }, waitingForResize { false };
    bool changeIncrement { false };

    headType readIncrement { 1 };
    headType distance, newDistance;
    dataType* buffer;
    uint currentSize { 0 }, newSize { 0 }; //newSize is a variable that stores the new bufferSize when the setSize() function is called
    dataType* newBuffer;
    uint writeHead { 0 };
    uint writeMax { 0 };
    headType readMax { 0 };

};