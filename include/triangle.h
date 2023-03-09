#include "oscillator.h"
#include <cmath>

#pragma once

class Triangle : public Oscillator {
    public:
        Triangle(float frequency, float amplitude) : Oscillator(frequency, amplitude) {}
        ~Triangle() {}
    
    void calculate()
    {
        sample = (2/pi)* asin(sin((2*pi*phase))) * amplitude;
    }

};

//f(x)=(2/\pi)*\arcsin(\sin(2\pi x))\cdot a