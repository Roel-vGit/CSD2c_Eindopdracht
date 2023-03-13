#include "effect.h"
#include "delay.h"
#include "sine.h"
#include "sawtooth.h"
#include "triangle.h"

class Flanger : public Effect {
    public:

        Flanger();
        ~Flanger();

        //Sets the delay and LFO
        void prepareToPlay(int samplerate) override;

        //Outputs the flanger effect
        void calculate(const float& input, float& output) override;

        /*set depth of the flanger. This is the size of the delay.
        Do not set to zero. This will cause a segmentation fault in the jack module. */
        void setDepth(float depth);

        //set feedback of the delay line
        void setFeedback(float feedback);

        //sets the frequency / rate of the flanger
        void setFrequency(float frequency);

    private:
        Delay delay = { Delay() };
        Triangle triangle = { Triangle(0.5f, 0.9f) };

        float depth { 0.0f };
        float freq { 1.0f };
        float feedback { 0.8f };

        //ticks the LFO and increments the delay time
        void tick();
};