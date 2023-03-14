#include "../include/chorus.h"

Chorus::Chorus() {};

Chorus::Chorus(float feedback, float rate, float depth, float phase)
{
    setFeedback(feedback);
    setRate(rate);
    setDepth(depth);
    setLFOPhase(phase);
}   

Chorus::~Chorus() {};

void Chorus::prepareToPlay(int sampleRate)
{
    this->sampleRate = sampleRate;
    delay.prepareToPlay(sampleRate);
    delay.setMaxDelay(50); //<-- delay buffer size
    delay.setDelayTime(delayTime);
    lfo.setSamplerate(sampleRate);
}


void Chorus::calculate(const float& input, float& output)
{
    lfo.tick();
    float modulation = delayTime + depth * lfo.getSample();
    delay.setDelayTime(modulation);
    delay.process(input, output);
}

void Chorus::setFeedback(float feedback)
{
    delay.setFeedback(feedback);
}

void Chorus::setRate(float rate)
{
    lfo.setFrequency(rate);
}

void Chorus::setDepth(float depth)
{
    if (depth < 0.0f) depth = 0.0f;
    else if (depth > 100.0f) depth = 100.0f;

    this->depth = (depth/100.0f) * delayTime;
}

void Chorus::setLFOPhase(float phase)
{
    lfo.setPhase(phase);
}