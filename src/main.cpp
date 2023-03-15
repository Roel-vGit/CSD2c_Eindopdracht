#include "../libs/jack_module.h"
#include "circularBuffer.cpp" //<-- need to include .cpp since it is a template class

#include "../include/delay.h"
#include "../include/sine.h"
#include "../include/sawtooth.h"
#include "../include/triangle.h"
#include "../include/panner.h"
#include "../include/chorus.h"
#include "../include/decorrelator.h"
#include "../include/waveshaper.h"
#include "../include/flanger.h"
#include "../include/reverb.h"
#include "../include/rack.h"
#include <array>

int outputs_ = 4;

class Callback : public AudioCallback {
    public:
        void prepare(int sampleRate) override
        {
			std::vector<Effect*> effects_ = {new WaveShaper(), new Decorrelator(), new Chorus(), new Flanger(), new Reverb()};
			for (auto & effect : effects_){
				rack.addEffect(effect->clone());
			}

			for (auto & effects : rack.bank){
				int counter_ = 0;
				for( auto & instances : effects){
					std::cout << counter_ << " " << instances->getType() << " ";
					instances->prepareToPlay(sampleRate);

					if (instances->getType() == "WaveShaper"){
						WaveShaper* ws = dynamic_cast<WaveShaper*>(instances);
						ws->setDryWet(1.0f);
						ws->setDrive(8.0f);
					}
					if (instances->getType() == "Chorus"){
						float phaseStep = counter_ *  (1.0f / outputs_);
						Chorus* chrs = dynamic_cast<Chorus*>(instances);
						chrs->setLFOPhase(phaseStep);
					}
					counter_++;
				}
			}
////                set the speaker positions
//                speaker[0].setPolarPosition(1.0f, 135, true);
//                speaker[1].setPolarPosition(1.0f, 45, true);
//                speaker[2].setPolarPosition(1.0f, 315, true);
//                speaker[3].setPolarPosition(1.0f, 225, true);

				angleStep = 0.0001f;
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
					rack.bank[0][channel]->process(outputChannels[channel][sample], outputChannels[channel][sample]);
					rack.bank[4][channel]->process(outputChannels[channel][sample], outputChannels[channel][sample]);

                }
            }
        }


		Rack rack {Rack(outputs_)};
    std::array<Sine, 2> sines { Sine(400, 0.5f), Sine(400, 0.5f) };
    std::array<Sawtooth, 2> saws { Sawtooth(300, 0.5f), Sawtooth(300, 0.5f) };
    std::array<Panner, 2> panner { Panner(), Panner() };
    Object joystick1 { Object() };
	Object joystick2 { Object() };
	Object touchpad1 { Object() };
	Object touchpad2 { Object() };
    float angle = { 0.0f };
	float angleStep { 0.0001f };
//    std::array<WaveShaper, 2> waveShapers { WaveShaper(4.0f), WaveShaper(4.0f) };
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
            case 'd':
                float dryWet;
                std::cout << "Enter dry wet: ";
                std::cin >> dryWet;
//                callback.reverbs[0].setDryWet(dryWet);
//                callback.reverbs[1].setDryWet(dryWet);
//                std::cout << "chorus L:" << callback.chorus[0].getDryWet() << std::endl;
//                std::cout << "chorus R:" << callback.chorus[0].getDryWet() << std::endl;
                continue;
            case 'b':
                bool bypass;
                std::cout << "Enter bypass: ";
                std::cin >> bypass;
//                callback.chorus[0].setBypass(bypass);
//                callback.chorus[1].setBypass(bypass);
//                callback.flangers[0].setBypass(bypass);
//                callback.flangers[1].setBypass(bypass);
//                callback.decorrelators[0].setBypass(bypass);
//                callback.decorrelators[1].setBypass(bypass);
                continue;
            case 's':
                float amp;
                std::cout << "Enter saw amp: ";
                std::cin >> amp;
                callback.saws[0].setAmplitude(amp);
                callback.saws[1].setAmplitude(amp);
			/*case 'y':
				for (Panner& speaker : callback.speaker)
				{
					std::cout << "delay: " << speaker.getDelay() << std::endl;
					std::cout << "distance: " << speaker.getDistance(callback.joystick1) << std::endl;
				}
				continue;
			 */
			case 'p':
				float speed;
				std::cout << "Set speed: ";
				std::cin >> speed;
				callback.angleStep = speed;
        }   

    }

    return 0;
}