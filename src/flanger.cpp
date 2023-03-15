#include "../include/flanger.h"
#include "../libs/util.h"

Flanger::Flanger()
{
	this->type = "Flanger";
}

Flanger::~Flanger()
{

}


void Flanger::prepareToPlay(int sampleRate)
{
    this->sampleRate = sampleRate;
    delay.prepareToPlay(sampleRate);
    delay.setMaxDelay(sampleRate);
    delay.setDelayTime(depth);
    delay.setFeedback(feedback);
    triangle.setSamplerate(sampleRate);
}

void Flanger::calculate(const float& input, float& output)
{
    tick();
    delay.process(input, output);
}

void Flanger::setDepth(float depth)
{   
    if (depth > 1000.0f)
        depth = 1000.0f;
    this->depth = depth;
    triangle.setAmplitude(depth);
}

void Flanger::setFeedback(float feedback)
{
    this->feedback = feedback;
    delay.setFeedback(feedback);
}

void Flanger::setFrequency(float frequency)
{
    this->freq = frequency;
    triangle.setFrequency(frequency);
}

void Flanger::tick()
{
    triangle.tick();
    delay.setDelayTime((triangle.getSample() + 1.0f) / 2.0f);
}