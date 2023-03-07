#pragma once

#include <cmath>
#include <vector>


class Oscillator {
    public:
        Oscillator();
        Oscillator(float frequency, float amplitude);
        ~Oscillator();

        //getters and setters
        virtual void setFrequency(float frequency); //virtual because we override function in AntiAliasedOsc
        float getFrequency();
        virtual void setAmplitude(float amplitude);
        float getAmplitude();
        float getSample();
        float getSampleRate();
        void setSamplerate(float samplerate);
        void setPhase(float phase);

        virtual void tick();
        float polyBLEP(float t);

        float frequency; //public becauase we need to acces this in superSynth
        float amplitude; //public becauase we need to acces this in superSynth

    protected:
        float samplerate;
        float phaseDelta;
        float phase;
        float sample;
        double pi = acos(-1); //create a constant for pi
        virtual void calculate() = 0;

};