#include "effect.h"
#include "../libs/CircularBuffer.h"
#include <cmath>
#include <iostream>

#pragma once

class Filter : public Effect {
    public:

        Filter();
        ~Filter();

        //initialize delay buffers
        void prepareToPlay(int samplerate) override;

        /*Sets the cutoff frequency of the lowpass and highpass filters
        cutoff: cutoff frequency in Hz */
        void setCutoff(float cutoff);

    protected:

        CircBuffer<float, uint> circBuf = { CircBuffer<float, uint>((uint)samplerate) }; //delay buffer for allpass input X[n-d]
        CircBuffer<float, uint> filterBuf = { CircBuffer<float, uint>((uint)samplerate) }; //delay buffer for allpass output (Y[n-d])

        float cutoff;
        float output1 { 0.0f }; //1 sample delay
        float output2; // 2 sample delay
        int samplerate { 48000 };
        float allpassFeedback { 0.2f };
        double allpassDelay { 5 };
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
        void calculate(const float& input, float& output) override;

        /*Sets the allpass filter coefficients and delay
        gain: filter coefficient (value between 0.0f and 1.0f)
        delay: delay of the allpass filter*/
        void setAllpass(float gain, double delay);

        //returns the delay of the allpass filter
        float getAllpassDelay() const;

        //returns the feedback (coefficient) of the allpass
        float getAllpassFeedback() const;
};

