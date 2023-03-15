#include "../include/reverb.h"

Reverb::Reverb() 
{

}
Reverb::~Reverb() 
{

}

void Reverb::prepareToPlay(int sampleRate)
{
    this->sampleRate = sampleRate;
    preDelay.prepareToPlay(sampleRate);

    for (Allpass& allpass : allpassFilters)
        allpass.prepareToPlay(sampleRate);
    for (Delay& delay : delays)
        delay.prepareToPlay(sampleRate);
    for (Lowpass& lowpass : lowpass)
        lowpass.prepareToPlay(sampleRate);

    highpass.prepareToPlay(sampleRate);
    
    //initialize pre-delayTime
    preDelay.setDelayTime(preDelayTime);

    //input lowpass filter cutoff
    lowpass[0].setCutoff(5000.0f);

    //damping filters
    lowpass[1].setCutoff(5000.0f);
    lowpass[2].setCutoff(1500.0f);

    //initialize highpass cutoff at output
    highpass.setCutoff(500.0f);

    //generate random values so every reverb instance sounds a bit different
    int randInt1 = Util::random(15);
    int randInt2 = Util::random(20);
    int randInt3 = Util::random(23);
    int randInt4 = Util::random(27);
    int randInt5 = Util::random(35);
    int randInt6 = Util::random(45);

    float randFloat1 = Util::random(10) / 100.0f;
    float randFloat2 = Util::random(15) / 100.0f;
    float randFloat3 = Util::random(12) / 100.0f;
    float randFloat4 = Util::random(14) / 100.0f;
    float randFloat5 = Util::random(19) / 100.0f;
    float randFloat6 = Util::random(25) / 100.0f;

    //initialize allpass filters, this influences the sound a lot
    allpassFilters[0].setAllpass(0.7f + randFloat1, Util::samplesToMs(35 + randInt1, sampleRate));
    allpassFilters[1].setAllpass(0.7f + randFloat2, Util::samplesToMs(1550 + randInt2, sampleRate));
    allpassFilters[2].setAllpass(0.6f + randFloat3, Util::samplesToMs(510 + randInt3, sampleRate));
    allpassFilters[3].setAllpass(0.6f + randFloat4, Util::samplesToMs(3600 + randInt4, sampleRate));
    allpassFilters[4].setAllpass(0.7f + randFloat4, (int)Util::samplesToMs(modDelay, sampleRate));
    allpassFilters[5].setAllpass(0.6f +randFloat6, (int)Util::samplesToMs(modDelay2, sampleRate));
    allpassFilters[6].setAllpass(0.5f + randFloat1, Util::samplesToMs(3450 + randInt6, sampleRate));
    allpassFilters[7].setAllpass(0.5f +randFloat1, Util::samplesToMs(2364 + randInt5, sampleRate));

    //set the delay time (density) of the delay lines in the reverb
    delays[0].setDelayTime(12.0f);
    delays[1].setDelayTime(55.0f);
    delays[2].setDelayTime(65.0f);
    delays[3].setDelayTime(145.0f);

}

void Reverb::calculate(const float& input, float& output)
{   
    //sets the coefficients for the modulated allpass filters
    allpassFilters[4].setAllpass(0.3f, (int)Util::samplesToMs(modDelay++, sampleRate));
    allpassFilters[4].setAllpass(0.3f, (int)Util::samplesToMs(modDelay2++, sampleRate));

    //wraps the coefficients
    if (modDelay > 1355)
        modDelay -= 24;
    
    if (modDelay2 > 1007)
        modDelay2 -= 24;

    //input first enters pre delay
    preDelay.process(input, outputY[0]);

    //gets lowpass filtered
    lowpass[0].process(outputY[0], outputY[0]);

    //gets allpass filtered
    for (int i = 0; i < 4; i++)
    {
        allpassFilters[i].process(outputY[0], outputY[0]);
    }

    //Here the signal gets delayed and filtered to create the reverb
    allpassFilters[4].process(outputY[0] + feedback1, outputY[0]);
    delays[0].process(outputY[0] , outputY[0]); // a
    lowpass[1].process(outputY[0], outputY[1]);
    allpassFilters[6].process(outputY[1], outputY[1]); // b
    delays[1].process(outputY[1], outputY[2]);  // c

    allpassFilters[5].process(outputY[2], outputY[2]);
    delays[2].process(outputY[2] * 0.7f, outputY[3]); // d
    lowpass[2].process(outputY[3], outputY[4]);
    allpassFilters[7].process(outputY[4], outputY[4]); // e
    delays[3].process(outputY[4], outputY[5]); // f

    feedback1 = outputY[5] * decay;

    //output = output at point a, b, c, d, e, f
    output = outputY[0] + outputY[1] + outputY[2] + outputY[3] + outputY[4] + outputY[5];
    
    //finally remove low frequencies from the delay to avoid low rumble
    highpass.process(output, output);
}


/*sets the decay of the reverb
decay: value between 0.0f and 0.999f.*/
void Reverb::setDecay(float decay)
{
    if (decay > 0.999f) decay = 0.99f;
    else if (decay < 0.0f) decay = 0.0f;
    this->decay = decay;
}

/*sets the filters in the reverb. High values result in short high frequency decays
damping: value between 0.0f and 1.0f*/
void Reverb::setDamping(float damping)
{
    if (damping > 1.0f) damping = 1.0f;
    else if (damping < 0.0f) damping = 0.0f;
    lowpass[1].setCutoff(4545 * (1.1f - damping));
    lowpass[2].setCutoff(750 * (2.0f - damping));
    this->damping = damping;
}

/*Sets the predelay of the reverb.
preDelay: Value between 0 and 1000 milliseconds.*/
void Reverb::setPreDelay(float preDelay)
{
    if (preDelay < 0.0f) preDelay = 0.0f;
    else if (preDelay > 1000.0f) preDelay = 1000.0f;
    
    this->preDelay.setDelayTime(preDelay);
    this->preDelayTime = preDelay;
}

void Reverb::setHighpass(float cutoff)
{
    if (cutoff < 0.0f) cutoff = 0.0f;
    else if (cutoff > 20000.0f) cutoff = 20000.0f;
    highpass.setCutoff(cutoff);
}