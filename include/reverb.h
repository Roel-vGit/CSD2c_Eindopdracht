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

        preDelay.setDelayTime(10.0f);
        lowpass[0].setCutoff(2000.0f);
        lowpass[1].setCutoff(2000.0f);
        lowpass[2].setCutoff(3000.0f);
        highpass.setCutoff(1000.0f);

        allpassFilters[0].setAllpass(0.75f, Util::samplesToMs(210, sampleRate));
        allpassFilters[1].setAllpass(0.75f, Util::samplesToMs(158, sampleRate));
        allpassFilters[2].setAllpass(0.625f, Util::samplesToMs(561, sampleRate));
        allpassFilters[3].setAllpass(0.625f, Util::samplesToMs(410, sampleRate));
        allpassFilters[6].setAllpass(0.5f, Util::samplesToMs(3931, sampleRate));
        allpassFilters[7].setAllpass(0.5f, Util::samplesToMs(2664, sampleRate));

        delays[0].setDelayTime(21.0f);
        delays[1].setDelayTime(43.0f);
        delays[2].setDelayTime(67.0f);
        delays[3].setDelayTime(125.0f);

        allpassFilters[4].setAllpass(0.7f, (int)Util::samplesToMs(modDelay, sampleRate));
        allpassFilters[5].setAllpass(0.7f, (int)Util::samplesToMs(modDelay2, sampleRate));


    }

    void calculate(const float& input, float& output) override
    {   

        allpassFilters[4].setAllpass(0.7f, modDelay++);
        allpassFilters[4].setAllpass(0.7f, modDelay2++);

        if (modDelay > 1355)
            modDelay -= 24;
        
        if (modDelay2 > 1007)
            modDelay2 -= 24;


        preDelay.process(input, outputY[0]);
        lowpass[0].process(outputY[0], outputY[0]);
        for (int i = 0; i < 4; i++)
        {
            allpassFilters[i].process(outputY[0], outputY[0]);
        }

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

        output = outputY[0] + outputY[1] + outputY[2] + outputY[3] + outputY[4] + outputY[5];
        
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