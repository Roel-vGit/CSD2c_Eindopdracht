#include "../libs/jack_module.h"
#include "circularBuffer.cpp" //<-- need to include .cpp since it is a template class

#include "../include/delay.h"
//#include "../include/sine.h"
//#include "../include/sawtooth.h"
//#include "../include/triangle.h"
#include "../include/panner.h"
#include "../include/chorus.h"
#include "../include/decorrelator.h"
#include "../include/waveshaper.h"
#include "../include/flanger.h"
#include "../include/reverb.h"
#include <array>

class Callback : public AudioCallback {
    public:
        void prepare(int sampleRate) override
        {
            for (int i = 0; i < 2; i++)
            {   
                flangers[i].prepareToPlay(sampleRate);
                flangers[i].setDryWet(0.5f);
                chorus[i].prepareToPlay(sampleRate);
                chorus[i].setDryWet(0.5f);
                chorus[i].setType("Chorus");
                decorrelators[i].prepareToPlay(sampleRate);
                decorrelators[i].setDryWet(1.0f);
                decorrelators[i].setType("Decorrelator");
                panner[i].prepareToPlay(sampleRate);
                panner[i+2].prepareToPlay(sampleRate);
                reverbs[i].prepareToPlay(sampleRate);
                reverbs[i].setDryWet(1.0f);
                delays[i].prepareToPlay(sampleRate);
                // delays[i].setMaxDelay(sampleRate);
                delays[i].setDelayTime(200.0f);
                delays[i].setFeedback(0.5f);
                delays[i].setDryWet(1.0f);
            }
                //set the panner positions
                panner[0].setPolarPosition(1.0f, 135, true);
                panner[1].setPolarPosition(1.0f, 45, true);
                panner[2].setPolarPosition(1.0f, 315, true);
                panner[3].setPolarPosition(1.0f, 225, true);
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
                    source.setPolarPosition(1.0f, 1.57f);
                    angle += 0.0001f;
                    if (angle > 6.28f)
                        angle -= 6.28f;

                    // //calculate amplitude and delay per panner based on source position
                    panner[channel].calcAmplitude(source);
                    panner[channel].calcDelay(source);

                    //calculate the effects
                    // flangers[channel].process(saws[channel].getSample(), outputChannels[channel][sample]);
                    // chorus[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
                    // decorrelators[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
                    reverbs[channel].process(inputChannels[channel][sample], outputChannels[channel][sample]);

                    //apply panning
                    panner[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
                }
            }
        }


    std::array<Sine, 2> sines { Sine(400, 0.5f), Sine(400, 0.5f) };
    std::array<Sawtooth, 2> saws { Sawtooth(300, 0.5f), Sawtooth(300, 0.5f) };
    std::array<Chorus, 2> chorus { Chorus(0.35f, 1.0f, 10), Chorus(0.4f, 1.2f, 15, 0.5f) } ;
    std::array<Decorrelator, 2> decorrelators { Decorrelator(), Decorrelator() };
    std::array<Delay, 2> delays { Delay(), Delay() };
    std::array<Flanger, 2> flangers { Flanger(), Flanger() };
    std::array<Reverb, 2> reverbs { Reverb(), Reverb() };
    std::array<Speaker, 4> panner { Speaker(), Speaker(), Speaker(), Speaker() };
    Object source { Object() };
    float angle = { 0.0f };
    std::array<WaveShaper, 2> waveShapers { WaveShaper(4.0f), WaveShaper(4.0f) };
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
            case 'd':
                float dryWet;
                std::cout << "Enter dry wet: ";
                std::cin >> dryWet;
                callback.chorus[0].setDryWet(dryWet);
                callback.chorus[1].setDryWet(dryWet);
                std::cout << "chorus L:" << callback.chorus[0].getDryWet() << std::endl;
                std::cout << "chorus R:" << callback.chorus[0].getDryWet() << std::endl;
                continue;
            case 'b':
                bool bypass;
                std::cout << "Enter dry wet: ";
                std::cin >> bypass;
                callback.chorus[0].setBypass(bypass);
                callback.chorus[1].setBypass(bypass); //<---- this does bypass the effect live?!?!?
                continue;
            case 's':
                float amp;
                std::cout << "Enter saw amp: ";
                std::cin >> amp;
                callback.saws[0].setAmplitude(amp);
                callback.saws[1].setAmplitude(amp);
        }   

    }

    return 0;
}