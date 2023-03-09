#include <iostream>

#pragma once

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

    void incrementWrite();
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