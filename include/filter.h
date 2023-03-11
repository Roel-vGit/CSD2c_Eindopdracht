#include "effect.h"
#include "../libs/CircularBuffer.h"
#include "../libs/Util.h"
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

        CircBuffer<float, uint> circBuf = { CircBuffer<float, uint>(48000) }; //delay buffer for allpass input X[n-d]
        CircBuffer<float, uint> filterBuf = { CircBuffer<float, uint>(48000) }; //delay buffer for allpass output (Y[n-d])

        float cutoff;
        float output1 { 0.0f }; //1 sample delay
        float output2; // 2 sample delay
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

class Decorrelator : public Effect {
    public:

        //prepares the allpass filters and generates random coefficients
        void prepareToPlay(int sampleRate) override;

        void calculate(const float& input, float& output) override;

        //generate random coefficients for the allpass filters
        void setCoefficients()
        {  
            for (Allpass& filter : filters)
            {
                float gain = Util::random(999) / 1000.0f;
                float delay = Util::random(sampleRate);
                filter.setAllpass(gain, delay);
            }
        }
    
    private:
        Allpass filters[10];
};

