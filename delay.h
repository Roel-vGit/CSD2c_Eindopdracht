#include "effect.h"
#include "CircularBuffer.h"

#pragma once

class Delay : public Effect {
public:
    Delay() {}
    ~Delay() {}

    void prepareToPlay (int sampleRate) override {
        this->sampleRate = sampleRate;
        circBuf.setSize(sampleRate*2);
        setDelayTime(200);
    }

    void calculate(const float& input, float& output) override
    {
        circBuf.writeSample(input + feedback * circBuf.readSample(delayTimeSamples));
        circBuf.incrementWrite();
        output = (1.0f - dryWet)*input + (dryWet * circBuf.readSample(delayTimeSamples));
    }

    void setDelayTime(float ms)
    {
        this->delayTimeSamples = msToSamples(ms);
    }

    void setMaxDelay(int maxDelay)
    {
        circBuf.setSize(msToSamples(maxDelay));
    }

    double getDelayTime()
    {
        return samplesToMs(delayTimeSamples);
    }

    void setFeedback(float feedback)
    {
        if (feedback > 0.9f)
            this->feedback = 0.9f;
        else if (feedback < -0.9f)
            this->feedback = -0.9f;
        else
            this->feedback = feedback;
    }


    CircBuffer<float, float> circBuf = CircBuffer<float, float>(sampleRate);

private:

    float msToSamples(float ms)
    {
        return (ms/1000.0f) * sampleRate;
    }

    float samplesToMs(float samples)
    {
        return (samples / sampleRate) * 1000.0f;
    }

    int sampleRate;
    float feedback { 0.0f };
    float delayTimeSamples;
};
