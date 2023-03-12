#include "../libs/jack_module.h"
#include "CircularBuffer.cpp" //<-- need to include .cpp since it is a template class

#include "../include/delay.h"
#include "../include/sine.h"
#include "../include/sawtooth.h"
#include "../include/triangle.h"
#include "../include/panner.h"
#include "../include/chorus.h"
#include "../include/filter.h"
#include "../include/waveshaper.h"
#include "../include/effectBank.h"
#include <array>
#include <vector>

class Callback : public AudioCallback {
    public:
        void prepare(int sampleRate) override
        {
			effects_.addEffect(new Chorus(0.35f, 1.0f, 10), 2);
        }
           

        void process(AudioBuffer buffer) override
        {
            auto [inputChannels, outputChannels, numInputChannels, numOutputChannels, numFrames] = buffer;
            for (int channel = 0u; channel < numOutputChannels; ++channel) {  
                for (int sample = 0u; sample < numFrames; ++sample)
                {
                    //test tone
                    saws[channel].tick();
					outputChannels[channel][sample] = saws[channel].getSample();

                }
            }
        }

		EffectBank effects_ {EffectBank(2)};

    std::array<Sine, 2> sines { Sine(400, 0.5f), Sine(400, 0.5f) };
    std::array<Sawtooth, 2> saws { Sawtooth(100, 0.5f), Sawtooth(100, 0.5f) };
    std::array<Chorus, 2> chorus { Chorus(0.35f, 1.0f, 10), Chorus(0.4f, 1.2f, 15, 0.5f) } ;
    std::array<Delay, 2> delays { Delay(), Delay() };
    Object source { Object() };
    float angle = { 0.0f };
    std::array<WaveShaper, 2> waveShapers { WaveShaper(4.0f), WaveShaper(4.0f) };


};


int main() {

    auto callback = Callback {};
    auto jack = JackModule (callback);

    jack.init(1,2);

    bool running = true;

    while (running) {
        switch (std::cin.get()) {
            case 'q':
                running = false;
                break;
        }       
    }

    return 0;
}