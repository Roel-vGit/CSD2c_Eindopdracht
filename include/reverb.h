#include "effect.h"
#include "delay.h"
#include "decorrelator.h"
#include "../libs/util.h"

#pragma once

class Reverb : public Effect {
    public:
        Reverb();
        ~Reverb();
    
    void prepareToPlay(int sampleRate) override;

    /*Calculates the reverb
    Input: input sample to read from
    output: output sample to write to*/
    void calculate(const float& input, float& output) override;

	Effect* clone() override {return new Reverb(*this);}

    /*sets the decay of the reverb
    decay: value between 0.0f and 0.999f.*/
    void setDecay(float decay);

    /*sets the filters in the reverb. High values result in short high frequency decays
    damping: value between 0.0f and 1.0f*/
    void setDamping(float damping);

    /*Sets the predelay of the reverb.
    preDelay: Value between 0 and 1000 milliseconds.*/
    void setPreDelay(float preDelay);

    /*Sets the high pass filter cutoff.
    cutoff: Value between 0 and 20.000 Hz.*/
    void setHighpass(float cutoff);

    private:
        Allpass allpassFilters[8];
        Delay preDelay;
        Delay delays[4];
        Lowpass lowpass[3];
        Highpass highpass;
        float preDelayTime { 25.0f };
        float damping { 0.0 };
        float decay { 0.75f };
        float feedback1 { 0.0f };
        float outputY[6] { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        int modDelay { 1343 }, modDelay2 { 995 };



};