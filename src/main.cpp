#include "../libs/jack_module.h"
#include "CircularBuffer.cpp" //<-- need to include .cpp since it is a template class

#include "../include/delay.h"
#include "../include/sine.h"
#include "../include/sawtooth.h"
#include "../include/triangle.h"
#include "../include/panner.h"

class Callback : public AudioCallback {
    public:
        void prepare(int sampleRate) override
        {
            
        }

        void process(AudioBuffer buffer) override
        {
            auto [inputChannels, outputChannels, numInputChannels, numOutputChannels, numFrames] = buffer;
            for (int channel = 0u; channel < numOutputChannels; ++channel) {  
                for (int sample = 0u; sample < numFrames; ++sample)
                {
                    sines[channel].tick();
                    outputChannels[channel][sample] = sines[channel].getSample();
                }
            }
        }

    std::array<Sine, 2> sines { Sine(400, 0.8f), Sine(400, 0.8f) };

};


int main() {

    auto callback = Callback {};
    auto jack = JackModule (callback);

    Panner panner(4, 1);
    std::cout << "Distance from source: " << panner.getDistance(panner.sources[0], panner.speakers[0]) << std::endl;

    jack.init(2,2);

    bool running = true;

    while (running) {
        switch (std::cin.get()) {
            case 'q':
                running = false;
                break;
            case 'd':
                float distance, angle;
                std::cout << "Enter distance: " << std::endl;
                std::cin >> distance;
                std::cout << "Enter angle: " << std::endl;
                std::cin >> angle;
                panner.sources[0].setPolarPosition(distance, angle);
                callback.sines[0].setAmplitude(panner.getSpeakerAmplitude(panner.sources[0], panner.speakers[0]));
                callback.sines[1].setAmplitude(panner.getSpeakerAmplitude(panner.sources[0], panner.speakers[1]));
                std::cout << "Amplitude of speaker 1: " << panner.getSpeakerAmplitude(panner.sources[0], panner.speakers[0]) << std::endl;
                std::cout << "Amplitude of speaker 2: " << panner.getSpeakerAmplitude(panner.sources[0], panner.speakers[1]) << std::endl;
                continue;
        }
    }

    return 0;
}