#include "../include/delay.h"
#include "circularBuffer.cpp" //<-- need to include .cpp since it is a template

    //constructor
    Delay::Delay() 
    {

    }

    //constructor with initializer list
    Delay::Delay(float maxDelay, float delayTime, float feedback)
    {
        this->maxDelay = maxDelay;
        setDelayTime(delayTime);
        setFeedback(feedback);
    }

    //Destrcutor
    Delay::~Delay() 
    {

    }

    //Initialize delay buffer to 2 seconds and delay time to 0 ms (no delay)*/
    void Delay::prepareToPlay (int sampleRate)
    {
        this->sampleRate = sampleRate;
        circBuf.setSize(Util::msToSamples(maxDelay, sampleRate));
        setDelayTime(0.0f);
    }

    /*outputs the delayed signal
    input: input sample to read from
    output: output sample to write to
    */
    void Delay::calculate(const float& input, float& output)
    {
        circBuf.writeSample(input + feedback * circBuf.readSample(delayTime));
        circBuf.incrementWrite();
        output = circBuf.readSample(delayTime);
    }

    /*sets the delay time
    delay: delay value either in samples or milliseconds
    samples: if true sets the delay time in samples
    */
    void Delay::setDelayTime(float delay, bool samples /*= false*/)
    {
        if (!samples)
            this->delayTime = Util::msToSamples(delay, sampleRate);
        else
            this->delayTime = delay;
        
    }

    //sets the buffer size (max delay)
    void Delay::setMaxDelay(float maxDelay)
    {
        this->maxDelay = maxDelay;
        circBuf.setSize((int)Util::msToSamples(maxDelay, sampleRate));
    }

    //returns the current delay time
    double Delay::getDelayTime() const
    {
        return Util::samplesToMs(delayTime, sampleRate);
    }

    /*sets the feedback in the delay line
    feedback: value between -0.9f and 0.9f
    */
    void Delay::setFeedback(float feedback)
    {
        if (feedback > 0.9f)
            this->feedback = 0.9f;
        else if (feedback < -0.9f)
            this->feedback = -0.9f;
        else
            this->feedback = feedback;
    }