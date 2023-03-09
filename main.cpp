#include "jack_module.h"
#include "CircularBuffer.cpp" //<-- need to include .cpp since it is a template class
#include "delay.h"

#include "sine.h"
#include "sawtooth.h"
#include "triangle.h"
#include "panner.h"
#include <array>

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
                    sines[channel].tick();
//                    delays[channel].process(inputChannels[channel][sample], outputChannels[channel][sample]);
                    outputChannels[channel][sample] = sines[channel].getSample();
                }
            }
        }

    std::array<Delay, 2> delays { Delay(), Delay() };
    std::array<Sine, 2> sines { Sine(400, 0.8f), Sine(400, 0.8f) };

};





int main() {

    auto callback = Callback {};
    auto jack = JackModule (callback);

    Panner panner(4, 1);
    panner.setPolarPosition(Panner::SoundSource::Speaker, 1.0f, 135, true);
    panner.setPolarPosition(Panner::SoundSource::Speaker, 1, 1.0f, 45, true);
    panner.setPolarPosition(Panner::SoundSource::Source, 0, 1.0f, 45, true);
//    std::cout << "Distance from surce: " << panner.getDistance(panner.sources[0], panner.speakers[0]) << std::endl;

    jack.init(1,2);

    bool running = true;

    while (running) {
        switch (std::cin.get()) {
            case 'q':
                running = false;
                break;
            // case 'd':
            //     float distance, angle;
            //     std::cout << "Enter distance: " << std::endl;
            //     std::cin >> distance;
            //     std::cout << "Enter angle: " << std::endl;
            //     std::cin >> angle;
            //     panner.setPolarPosition(panner.sources[0], distance, angle);
            //     callback.sines[0].setAmplitude(panner.getDistance(panner.sources[0], panner.speakers[0]));
            //     callback.sines[1].setAmplitude(panner.getDistance(panner.sources[0], panner.speakers[1]));
            //     continue;
        }
    }

    return 0;
}