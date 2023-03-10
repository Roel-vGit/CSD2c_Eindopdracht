#include "effect.h"
#include "delay.h"
#include "sine.h"

#pragma once

class Chorus : public Effect {
    public:
        Chorus();
        Chorus(float feedback, float rate, float depth, float phase = 0.0f);
        ~Chorus();

        //initialze the delay buffer and LFO
        void prepareToPlay(int sampleRate) override;

        //calculate the chorus effect
        void calculate(const float& input, float& output) override;

        //sets the feedback of the delay line
        void setFeedback(float feedback);
        
        //sets the rate (in Hz) of the LFO modulating the delay
        void setRate(float rate);

        /*sets the depth (delay time) of the LFO
        depth: value between 0% and 100% */
        void setDepth(float depth);

        /*sets the phase of the LFO modulating the delay
        function can be used to offset the LFO */
        void setLFOPhase(float phase);

    private:
        float rate { 2.0f };
        float depth { 20.0f };
        const float delayTime { 25.0f };
        float sampleRate;
        
        Delay delay = { Delay(50, delayTime) };
        Sine lfo = { Sine(rate, 0.5f) };
};