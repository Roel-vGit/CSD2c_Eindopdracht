#include "effect.h"
#include "delay.h"
#include "sine.h"
#include "sawtooth.h"
#include "triangle.h"

#pragma once

class Flanger : public Effect {
    public:

        Flanger();
        ~Flanger();

        //Sets the delay and LFO
        void prepareToPlay(int sampleRate) override;

        //Outputs the flanger effect
        void calculate(const float& input, float& output) override;

        /*set depth of the flanger. This is the size of the delay.
        Typical flanger values are between 2 - 5 milliseconds.*/
        void setDepth(float depth);

        //set feedback of the delay line
        void setFeedback(float feedback);

        //sets the frequency / rate of the flanger
        void setFrequency(float frequency);

    private:
        Delay delay = { Delay() };
        Triangle triangle = { Triangle(0.5f, 0.9f) };

        float depth { 2.0f };
        float freq { 1.0f };
        float feedback { 0.8f };

        //ticks the LFO and increments the delay time
        void tick();
};