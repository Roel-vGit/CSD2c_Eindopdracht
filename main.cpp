#include "jack_module.h"
#include "CircularBuffer.cpp" //<-- need to include .cpp since it is a template class
#include "delay.h"

#include "sine.h"
#include "sawtooth.h"
#include "triangle.h"

class Callback : public AudioCallback {
    public:
        void prepare(int sampleRate) override
        {
            for (Delay& delay: delays)
                delay.prepareToPlay(sampleRate);
        }

        void process(AudioBuffer buffer) override
        {
            auto [inputChannels, outputChannels, numInputChannels, numOutputChannels, numFrames] = buffer;
            for (int channel = 0u; channel < numOutputChannels; ++channel) {  
                for (int sample = 0u; sample < numFrames; ++sample)
                {
                    // sines[channel].tick();
                    delays[channel].process(inputChannels[channel][sample], outputChannels[channel][sample]);
                    // outputChannels[channel][sample] = sines[channel].getSample();
                }
            }
        }

    private:
    std::array<Delay, 2> delays { Delay(), Delay() };
    std::array<Sine, 2> sines { Sine(400, 0.8f), Sine(400, 0.8f) };

};



int main() {

    auto callback = Callback {};
    auto jack = JackModule (callback);

    jack.init(2,2);

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