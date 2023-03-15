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
		Effect* clone() override {return new Lowpass(*this);}

};

class Highpass : public Filter {
    public:
        void calculate(const float& input, float& output) override;
		Effect* clone() override {return new Highpass(*this);}

};

class Allpass : public Filter {
    public:

        void prepareToPlay(int samplerate) override;

        void calculate(const float& input, float& output) override;

		Effect* clone() override {return new Allpass(*this);}

        /*Sets the allpass filter coefficients and delay
        gain: filter coefficient (value between 0.0f and 1.0f)
        delay: delay of the allpass filter*/
        void setAllpass(float gain, float delay);

        //returns the delay of the allpass filter
        float getAllpassDelay() const;

        //returns the feedback (coefficient) of the allpass
        float getAllpassFeedback() const;
        
        CircBuffer<float, float> circBuf = { CircBuffer<float, float>(48000) }; //delay buffer for allpass input X[n-d]
        CircBuffer<float, float> filterBuf = { CircBuffer<float, float>(48000) }; //delay buffer for allpass output (Y[n-d])
        float allpassFeedback { 0.2f };
        float allpassDelay { 5 };
        float gainFactor { 1.0f };
        float delayFactor { 1.0 };
};