#include "effect.h"
#include "../libs/circularBuffer.h"
#include "../libs/util.h"
#include <cmath>
#include <iostream>

#pragma once

class Filter : public Effect {
    public:

        Filter();
        ~Filter();

        //initialize delay buffers
        virtual void prepareToPlay(int samplerate) override;

        /*Sets the cutoff frequency of the lowpass and highpass filters
        cutoff: cutoff frequency in Hz */
        void setCutoff(float cutoff);

    protected:
        float cutoff;
        float output1 { 0.0f }; //1 sample delay
        float output2; // 2 sample delay
};

class Lowpass : public Filter {
    public:
        void calculate(const float& input, float& output) override;

};

class Highpass : public Filter {
    public:
        void calculate(const float& input, float& output) override;

};

class Allpass : public Filter {
    public:

        void prepareToPlay(int samplerate) override;

        void calculate(const float& input, float& output) override;

        /*Sets the allpass filter coefficients and delay
        gain: filter coefficient (value between 0.0f and 1.0f)
        delay: delay of the allpass filter*/
        void setAllpass(float gain, float delay);

        //returns the delay of the allpass filter
        float getAllpassDelay() const;

        //returns the feedback (coefficient) of the allpass
        float getAllpassFeedback() const;
        
        CircBuffer<float, uint> circBuf = { CircBuffer<float, uint>(48000) }; //delay buffer for allpass input X[n-d]
        CircBuffer<float, uint> filterBuf = { CircBuffer<float, uint>(48000) }; //delay buffer for allpass output (Y[n-d])
        float allpassFeedback { 0.2f };
        float allpassDelay { 5 };
        float gainFactor { 1.0f };
        float delayFactor { 1.0 };
};

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