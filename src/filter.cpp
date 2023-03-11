#include "../include/filter.h"
#include "../src/CircularBuffer.cpp"

Filter::Filter() {};

Filter::~Filter() {};

//initialize delay buffer size to 1 second
void Filter::prepareToPlay(int samplerate)
{
    this->sampleRate = samplerate;
    circBuf.setSize(samplerate);
    filterBuf.setSize(samplerate);
}

void Filter::setCutoff(float cutoff)
{
    if (cutoff > sampleRate/2)
        cutoff = sampleRate/2;
    else if (cutoff < 0)
        cutoff = 0.0;

    this->cutoff = sin(cutoff*(M_PI/sampleRate)); //calculate cutoff -> https://docs.cycling74.com/max7/refpages/onepole~
}

void Lowpass::calculate(const float& input, float& output)
{
    output1 += cutoff * (input - output1); //y(n) = y(n-1) + a_{0}[ x(n) - y(n-1) ] https://docs.cycling74.com/max7/refpages/onepole~
    output2 += cutoff * (output1 - output2); //2nd order lowpass
    output = output2;
}

void Highpass::calculate(const float& input, float& output) 
{
    output1 += cutoff * (input - output1);
    output2 += cutoff * (output1 - output2); 
    output = input - output2; //subtract original signal from lowpass signal to get highpass signal
}

void Allpass::calculate(const float& input, float& output)
{   
    circBuf.writeSample(input);
    output = (allpassFeedback * -1 * input) + circBuf.readSample(allpassDelay) + (allpassFeedback * filterBuf.readSample(allpassDelay));// y[n] = (-g * x[n]) + x[n - d] + (g * y[n - d])
    filterBuf.writeSample(output1);
    circBuf.incrementWrite();
    filterBuf.incrementWrite();
}

void Allpass::setAllpass(float gain, double delay)
{
    this->allpassFeedback = gain;
    this->allpassDelay = delay;
}

float Allpass::getAllpassDelay() const
{
    return allpassDelay;
}

float Allpass::getAllpassFeedback() const
{
    return allpassFeedback;
}

void Decorrelator::prepareToPlay(int sampleRate)
{
    this->sampleRate = sampleRate;
    for (Allpass& filter : filters)
        filter.prepareToPlay(sampleRate);
    setCoefficients();
}

void Decorrelator::calculate(const float& input, float& output)
{
    output = input;
    for (Allpass& filter : filters)
        filter.calculate(output, output);


}
