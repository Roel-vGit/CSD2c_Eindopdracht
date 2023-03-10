#include "../include/chorus.h"

//TODO: split header file in .h and this .cpp

Chorus::Chorus() {};
Chorus::~Chorus() {};

//initialze the delay buffer and LFO
void Chorus::prepareToPlay(int sampleRate)
{
    this->sampleRate = sampleRate;
    delay.prepareToPlay(sampleRate);
    delay.setMaxDelay(50); //<-- center delay time
    delay.setDelayTime(delayTime);
    lfo.setSamplerate(sampleRate);
}

//calculate the chorus effect
void Chorus::calculate(const float& input, float& output)
{
    lfo.tick();
    float modulation = delayTime + depth * lfo.getSample();
    delay.setDelayTime(modulation);
    delay.calculate(input, output);
}

//sets the chorus feedback
void Chorus::setFeedback(float feedback)
{
    delay.setFeedback(feedback);
}
