#include "effect.h"
#include "delay.h"
#include "decorrelator.h"

#pragma once

class Reverb : public Effect {
    public:
        Reverb() {};
        ~Reverb() {};
    
    void prepareToPlay(int sampleRate) override
    {
        this->sampleRate = sampleRate;
        preDelay.prepareToPlay(sampleRate);

        for (Allpass& allpass : allpassFilters)
            allpass.prepareToPlay(sampleRate);
        for (Delay& delay : delays)
            delay.prepareToPlay(sampleRate);
        for (Lowpass& lowpass : lowpass)
            lowpass.prepareToPlay(sampleRate);

        highpass.prepareToPlay(sampleRate);
        
        //initialize pre-delayTime
        preDelay.setDelayTime(preDelayTime);

        //initialize cutoff frequencies for filters in the reverb
        lowpass[0].setCutoff(2000.0f);
        lowpass[1].setCutoff(2000.0f);
        lowpass[2].setCutoff(3000.0f);

        //initialize highpass cutoff at output
        highpass.setCutoff(1000.0f);


        //initialize allpass filters
        allpassFilters[0].setAllpass(0.75f, Util::samplesToMs(210, sampleRate));
        allpassFilters[1].setAllpass(0.75f, Util::samplesToMs(158, sampleRate));
        allpassFilters[2].setAllpass(0.625f, Util::samplesToMs(561, sampleRate));
        allpassFilters[3].setAllpass(0.625f, Util::samplesToMs(410, sampleRate));
        allpassFilters[4].setAllpass(0.7f, (int)Util::samplesToMs(modDelay, sampleRate));
        allpassFilters[5].setAllpass(0.7f, (int)Util::samplesToMs(modDelay2, sampleRate));
        allpassFilters[6].setAllpass(0.5f, Util::samplesToMs(3931, sampleRate));
        allpassFilters[7].setAllpass(0.5f, Util::samplesToMs(2664, sampleRate));

        //set the delay time (density) of the delay lines in the reverb
        delays[0].setDelayTime(12.0f);
        delays[1].setDelayTime(55.0f);
        delays[2].setDelayTime(76.0f);
        delays[3].setDelayTime(120.0f);
    }

    void calculate(const float& input, float& output) override
    {   
        //sets the coefficients for the modulated allpass filters
        allpassFilters[4].setAllpass(0.7f, (int)Util::samplesToMs(modDelay++, sampleRate));
        allpassFilters[4].setAllpass(0.7f, (int)Util::samplesToMs(modDelay2++, sampleRate));

        //wraps the coefficients
        if (modDelay > 1355)
            modDelay -= 24;
        
        if (modDelay2 > 1007)
            modDelay2 -= 24;

        //input first enters pre delay
        preDelay.process(input, outputY[0]);

        //gets lowpass filtered
        lowpass[0].process(outputY[0], outputY[0]);

        //gets allpass filtered
        for (int i = 0; i < 4; i++)
        {
            allpassFilters[i].process(outputY[0], outputY[0]);
        }

        //Here the signal gets delayed and filtered to create the reverb
        allpassFilters[4].process(outputY[0] + feedback1, outputY[0]);
        delays[0].process(outputY[0] , outputY[0]); // a
        lowpass[1].process(outputY[0], outputY[1]);
        allpassFilters[6].process(outputY[1], outputY[1]); // b
        delays[1].process(outputY[1], outputY[2]);  // c

        allpassFilters[5].process(outputY[2], outputY[2]);
        delays[2].process(outputY[2] * 0.7f, outputY[3]); // d
        lowpass[2].process(outputY[3], outputY[4]);
        allpassFilters[7].process(outputY[4], outputY[4]); // e
        delays[3].process(outputY[4], outputY[5]); // f

        feedback1 = outputY[5] * 0.5f;

        //output = output at point a, b, c, d, e, f
        output = outputY[0] + outputY[1] + outputY[2] + outputY[3] + outputY[4] + outputY[5];
        
        //finally remove low frequencies from the delay to avoid low rumble
        highpass.process(output, output);
    }

    private:
        Allpass allpassFilters[8];
        Delay preDelay { Delay() };
        Delay delays[4];
        Lowpass lowpass[3];
        Highpass highpass;
        float preDelayTime { 50.0f };
        float damping;
        float decay;
        float feedback1 { 0.0f };
        float outputY[6];
        int modDelay { 1343 }, modDelay2 { 995 };



};