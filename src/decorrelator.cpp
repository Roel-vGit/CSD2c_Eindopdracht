#include "../include/decorrelator.h"
#include "circularBuffer.cpp"

void Decorrelator::prepareToPlay(int sampleRate)
{
	this->type = "Decorrelator";
    this->sampleRate = sampleRate;
    for (Allpass& filter : filters)
        filter.prepareToPlay(sampleRate);
    setCoefficients(0.999f, sampleRate);
}

void Decorrelator::calculate(const float& input, float& output)
{
    output = input;
    for (Allpass& filter : filters)
        filter.process(output, output);
}

void Decorrelator::setCoefficients(float maxFeedback, float maxDelay)
{  
    int sign = 1;
    if (maxFeedback > 0.999f)
        maxFeedback = 0.999f;
    else if (maxFeedback < 0)
    {
        maxFeedback *= -1.0f;
        sign = -1;
    }

    if (maxDelay > sampleRate)
        maxDelay = sampleRate;

    for (Allpass& filter : filters)
    {
        float gain = Util::random(maxFeedback * 1000) / 1000.0f * sign;
        float delay = Util::random(maxDelay);
        filter.setAllpass(gain, delay);
    }
}

void Decorrelator::changeCoefficients(float gainFactor, float delayFactor)
{   

    if (gainFactor > 1.0f) gainFactor = 1.0f;
    else if (gainFactor < 0.0f) gainFactor = 0.0f;

    if (delayFactor > 1.0f) delayFactor = 1.0f;
    else if (delayFactor < 0.0f) delayFactor = 0.0f;

    for (Allpass& filter : filters)
    {
        filter.gainFactor = gainFactor;
        filter.delayFactor = delayFactor;
    }
}
