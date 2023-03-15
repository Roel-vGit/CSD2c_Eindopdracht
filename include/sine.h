#pragma once

#include <iostream>
#include <cmath>
#include "oscillator.h"


class Sine : public Oscillator
{
  public:
    //Constructor and destructor
    Sine() {};
    Sine(float frequency, float amplitude) : Oscillator(frequency, amplitude) {};
    ~Sine() {};

//	float getPhase() { return phase; }

    void calculate() { sample = sin(pi * 2 * phase) * amplitude; };
};
