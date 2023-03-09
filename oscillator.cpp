#include "oscillator.h"


#include <iostream>

Oscillator::Oscillator() : Oscillator(500, 1.0)
{
    // std::cout << "Concstructed default oscillator" << std::endl;
}

Oscillator::Oscillator(float frequency, float amplitude) : frequency(frequency), amplitude(amplitude)
{
    phase = 0.0f;
    sample = 0.0f;
    samplerate = 48000; //default samplerate
    phaseDelta = frequency / samplerate;
}

Oscillator::~Oscillator()
{
    // std::cout << "Oscillator destructor" << std::endl;
}


float Oscillator::getSample()
{
    return sample;
}

void Oscillator::setFrequency(float frequency)
{
    this->frequency = frequency;
}

float Oscillator::getFrequency()
{
    return frequency;
}

float Oscillator::getAmplitude()
{
    return amplitude;
}

float Oscillator::getSampleRate()
{
    return samplerate;
}

void Oscillator::setSamplerate(float samplerate) 
{
    this->samplerate = samplerate;
}

void Oscillator::setAmplitude(float amplitude)
{
    this->amplitude = amplitude;
}

//increment phase of oscillator
void Oscillator::tick()
{
    phase += frequency / samplerate;
    if(phase > 1.0f){
        phase -= 1.0f;
    }
    calculate(); //call calculate() from subclass
}

//Anti-aliasing method that smoothes steep edges in the waveform
//https://www.martin-finke.de/articles/audio-plugins-018-polyblep-oscillator/
float Oscillator::polyBLEP(float t)
{
    float dt = phaseDelta;
    
    if (t < dt) //if phase is at begin
    {
        t /= dt; //scale t
        return t+t - t*t - 1.0; //2x - x^2 - 1.0 <-- return polynomial
    }
    else if (t > 1.0 - dt) //if phase is at end
    {
        t = (t - 1.0) / dt; //scale t
        return t*t + t+t + 1.0; //x^2 + 2x + 1.0 <-- return polynomial
    }
    else return 0.0;

}

void Oscillator::setPhase(float phase)
{
    this->phase = phase;
}