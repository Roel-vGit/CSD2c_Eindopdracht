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

        preDelay.setDelayTime(20.0f);
        lowpass[0].setCutoff(8000.0f);
        lowpass[1].setCutoff(5000.0f);
        lowpass[2].setCutoff(3000.0f);

        allpassFilters[0].setAllpass(0.75f, Util::samplesToMs(210, sampleRate));
        allpassFilters[1].setAllpass(0.75f, Util::samplesToMs(158, sampleRate));
        allpassFilters[2].setAllpass(0.625f, Util::samplesToMs(561, sampleRate));
        allpassFilters[3].setAllpass(0.625f, Util::samplesToMs(410, sampleRate));
        allpassFilters[6].setAllpass(0.5f, Util::samplesToMs(3931, sampleRate));
        allpassFilters[7].setAllpass(0.5f, Util::samplesToMs(2664, sampleRate));

        delays[0].setDelayTime(141.0f);
        delays[1].setDelayTime(149.0f);
        delays[2].setDelayTime(105.0f);
        delays[3].setDelayTime(125.0f);

        allpassFilters[4].setAllpass(0.4f, (int)Util::samplesToMs(1403, sampleRate));
        allpassFilters[5].setAllpass(0.3f, (int)Util::samplesToMs(1987, sampleRate));


    }

    void calculate(const float& input, float& output) override
    {
        preDelay.process(input, output);
        lowpass[0].process(output, output);
        for (int i = 0; i < 4; i++)
        {
            allpassFilters[i].process(output, output);
        }
        delays[0].process(output + feedback1, output); // a
        lowpass[1].process(output, output);
        allpassFilters[6].process(output, output); // b
        delays[1].process(output, output);  // c
        delays[2].process(output * 0.7f, output); // d
        lowpass[2].process(output, output);
        allpassFilters[7].process(output, output); // e
        delays[3].process(output, output); // f
        feedback1 = output * 0.7f;
        


    }

    private:
        Allpass allpassFilters[8];
        Delay preDelay { Delay() };
        Delay delays[4];
        Lowpass lowpass[3];
        float preDelayTime { 50.0f };
        float damping;
        float decay;
        float feedback1 { 0.0f };
        float outputLeft { 0.0f };


};