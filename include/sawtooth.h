#pragma once

#include "oscillator.h"

class Sawtooth : public Oscillator {
    public:
        Sawtooth() {};
        Sawtooth(float frequency, float amplitude) : Oscillator(frequency, amplitude) {};
        ~Sawtooth() {};
    
    void calculate() 
    {     
        sample = amplitude*2 * phase - amplitude;
        sample -= polyBLEP(phase);
    }
};