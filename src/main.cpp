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

class Callback : public AudioCallback {
    public:
        void prepare(int sampleRate) override
        {
			std::vector<Effect*> effects_ = {new Chorus()};
			for (auto & effect : effects_){
				rack.addEffect(effect->clone());
			}

			for (auto & effects : rack.bank){
				for( auto & instances : effects){
					std::cout << instances->getType() << " ";
				}
			}
			Chorus* chorus1ptr = dynamic_cast<Chorus*>(rack.bank[0][0]);
			Chorus* chorus2ptr = dynamic_cast<Chorus*>(rack.bank[0][1]);
			rack.bank[0][0]->prepareToPlay(sampleRate);
			rack.bank[0][1]->prepareToPlay(sampleRate);
			chorus1ptr->setLFOPhase(0.0f);
			chorus2ptr->setLFOPhase(0.5f);

			std::cout << rack.bank[0][0] << std::endl;
			std::cout << rack.bank[0][1] << std::endl;

//			for
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

                }
            }
        }


		Rack rack {Rack(2)};
    std::array<Sine, 2> sines { Sine(400, 0.5f), Sine(400, 0.5f) };
    std::array<Sawtooth, 2> saws { Sawtooth(300, 0.5f), Sawtooth(300, 0.5f) };
//    std::array<Chorus, 2> chorus { Chorus(0.35f, 1.0f, 10), Chorus(0.4f, 1.2f, 15, 0.5f) } ;
//    std::array<Decorrelator, 2> decorrelators { Decorrelator(), Decorrelator() };
//    std::array<Flanger, 2> flangers { Flanger(), Flanger() };
//    std::array<Reverb, 2> reverbs { Reverb(), Reverb() };
//    std::array<Panner, 2> speaker { Panner(), Panner() };
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