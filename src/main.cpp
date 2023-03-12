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
#include <array>

class Callback : public AudioCallback {
    public:
        void prepare(int sampleRate) override
        {
            for (int i = 0; i < 2; i++)
            {
                chorus[i].prepareToPlay(sampleRate);
                chorus[i].setDryWet(0.5f);  
                allpass[i].prepareToPlay(sampleRate);
                allpass[i].setDryWet(1.0f);
                speaker[i].prepareToPlay(sampleRate);
            }
                //set the speaker positions
                speaker[0].setPolarPosition(1.0f, 135, true);
                speaker[1].setPolarPosition(1.0f, 45, true);
                speaker[2].setPolarPosition(1.0f, 315, true);
                speaker[3].setPolarPosition(1.0f, 225, true);
        }
           

        void process(AudioBuffer buffer) override
        {
            auto [inputChannels, outputChannels, numInputChannels, numOutputChannels, numFrames] = buffer;
            for (int channel = 0u; channel < numOutputChannels; ++channel) {  
                for (int sample = 0u; sample < numFrames; ++sample)
                {
                    //test tone
                    saws[channel].tick();

                    //make the audio source circle
                    source.setPolarPosition(1.0f, angle, true);
                    angle += 0.005;
                    // outputChannels[channel][sample] = sines[channel].getSample();
//                    allpass[channel].process(saws[channel].getSample(), outputChannels[channel][sample]);
					// waveShapers[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);

                    //calculate amplitude and delay per speaker based on source position
                    speaker[channel].calcAmplitude(source);
                    speaker[channel].calcDelay(source);

                    //calculate the effects
                    allpass[channel].process(saws[channel].getSample(), outputChannels[channel][sample]);
                    // allpass[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
                    
                    //apply panning
                    speaker[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
                }
            }
        }

    std::array<Sine, 2> sines { Sine(400, 0.5f), Sine(400, 0.5f) };
    std::array<Sawtooth, 2> saws { Sawtooth(100, 0.5f), Sawtooth(100, 0.5f) };
    std::array<Chorus, 2> chorus { Chorus(0.35f, 1.0f, 10), Chorus(0.4f, 1.2f, 15, 0.5f) } ;
    std::array<Decorrelator, 2> allpass { Decorrelator(), Decorrelator() };
    std::array<Delay, 2> delays { Delay(), Delay() };
    std::array<Speaker, 4> speaker { Speaker(), Speaker(), Speaker(), Speaker() };
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
            case 'w':
                float dryWet;
                std::cout << "Enter dry wet: ";
                std::cin >> dryWet;
                for (Chorus& chorus : callback.chorus)
                {
                    chorus.setDryWet(dryWet);
                }
                continue;
            case 'a':
                float gain;
                float dly;
                std::cout << "Enter gain: ";
                std::cin >> gain;
                std::cout << std::endl << "Enter delay: ";
                std::cin >> dly;
                for (Decorrelator& decorrelator : callback.allpass)
                {
                    decorrelator.setDryWet(dly);
                    decorrelator.setCoefficients(gain, dly);
                }
                continue;
        }       
    }

    return 0;
}