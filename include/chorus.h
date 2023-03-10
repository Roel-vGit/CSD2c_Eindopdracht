#include "effect.h"
#include "delay.h"
#include "sine.h"

#pragma once

class Chorus : public Effect {
    public:
        Chorus();
        ~Chorus();

        //initialze the delay buffer and LFO
        void prepareToPlay(int sampleRate) override;

        //calculate the chorus effect
        void calculate(const float& input, float& output) override;

        //sets the chorus feedback
        void setFeedback(float feedback);

    private:
        float rate { 2.0f };
        Delay delay = { Delay() };
        Sine lfo = { Sine(rate, 0.5f) };
        float depth { 20.0f };
        float delayTime { 25.0f };
        float sampleRate;

};