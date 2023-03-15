#include "effect.h"
#include "../libs/circularBuffer.h"
#include "../libs/util.h"
#include "../include/filter.h"
#include <cmath>
#include <iostream>

#pragma once

class Decorrelator : public Effect {
    public:

        //prepares the allpass filters and generates random coefficients
        void prepareToPlay(int sampleRate) override;

        void calculate(const float& input, float& output) override;

        /*generate random coefficients for the allpass filters
        maxFeedback: floating point value between -0.999f and 0.999f.
        maxDelay: maximum delay in samples. Value between 0 and samplerate */
        void setCoefficients(float maxFeedback, float maxDelay);

        void changeCoefficients(float gainFactor, float delayFactor);
    
    private:
        Allpass filters[5];
};
