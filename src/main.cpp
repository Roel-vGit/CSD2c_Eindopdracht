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

//int outputs_ = 2;
// Effect Chain = [WaveShaper, Decorrelator, Chorus, Flanger, Reverb, Panner, Amp];
//int waveshaper = 0;
//int decorrelator = 1;
//int chorus = 2;
//int flanger = 3;
//int reverb = 4;
//int panner = 5;

class Callback : public AudioCallback {
    public:
        void prepare(int sampleRate) override
        {
			for(int i = 0; i < 2; i++){
				chorus_[i].prepareToPlay(sampleRate);
				chorus_[i].setDryWet(1.0f);
			}
//			Initializing the amount of panners based on the number of outputs.
			for(int i = 0; i < 2; i++){
				float increment = 360.0f / 2;
				float angle = 135 - increment * i;
				if (angle < 0) angle += 360; else if (angle > 360) angle -= 360;
				panner.push_back(new Panner());
				std::cout << "Angle: " << angle << std::endl;
				panner[i]->setPolarPosition(1.0f, angle, true);
			}

//			Array containing all effect pointers.
			std::vector<Effect*> effects_ = {new WaveShaper(), new Decorrelator(), new Chorus(), new Flanger(), new Reverb()};

//			Ranged based for loop to add all the effects to the rack.
			for (auto & effect : effects_){
				rack.addEffect(effect);
			}

//			Range based for loop to iterate over the rack.bank and prepare all the effects.
			for (auto & effects : rack.bank){
				int counter = 0;
				for(auto & instances : effects){
//					Setting sampleRate for all effects.
					instances->prepareToPlay(sampleRate);
					std::cout << counter << " ";
					std::cout << instances->getType() << ": " << instances->getSampleRate()
					<< "\n";

//					Type checker to set specific parameters for each effect.
					if (instances->getType() == "WaveShaper"){
//						Cast the Effect pointer in a subclass Pointer to call the subclass specific member functions.
						auto* pWaveShaper = dynamic_cast<WaveShaper*>(instances);
						pWaveShaper->setDrive(10.0f);
						pWaveShaper->setDryWet(0.0f);

					}
					if (instances->getType() == "Decorrelator"){
						auto* pDecorrelator = dynamic_cast<Decorrelator*>(instances);
						pDecorrelator->setDryWet(1.0f);
					}
					if (instances->getType() == "Chorus"){
						auto* pChorus = dynamic_cast<Chorus*>(instances);
						pChorus->setDryWet(1.0f);
						pChorus->setFeedback(0.5f);
						pChorus->setRate(0.5f);
						pChorus->setDepth(15.0f);
						pChorus->setLFOPhase(0 + (0.5f * counter));
					}
					if (instances->getType() == "Flanger"){
						auto* pFlanger = dynamic_cast<Flanger*>(instances);
						pFlanger->setDryWet(1.0f);
					}
					if (instances->getType() == "Reverb"){
						auto* pReverb = dynamic_cast<Reverb*>(instances);
						pReverb->setDryWet(1.0f);
					}
					counter++;


				}
			}

			int counter_ = 0;
			for (auto instance : rack.bank[2]){
				auto* pChorus = dynamic_cast<Chorus*>(instance);
				std::cout  << pChorus->getType() << " " << counter_ << std::endl;
				std::cout << "	SampleRate:	" << pChorus->getSampleRate() << std::endl;
				std::cout << "	Rate: 		" << pChorus->getRate() << std::endl;
				std::cout << "	Depth: 		" << pChorus->getDepth() << std::endl;
				std::cout << "	DelayTime: 	"<< pChorus->getDelayTime() << std::endl;
				std::cout << "	Phase: 		"<< pChorus->getPhase() << std::endl;
				counter_++;
			}
        }
           

        void process(AudioBuffer buffer) override
        {
            auto [inputChannels, outputChannels, numInputChannels, numOutputChannels, numFrames] = buffer;
            for (int channel = 0; channel < numOutputChannels; channel++) {
                for (int sample = 0u; sample < numFrames; ++sample)
				{

                    //Test tone.
                    saws[channel].tick();
					outputChannels[channel][sample] = saws[channel].getSample();
					if (channel == 0){
						std::cout << "Channel: " << channel << " Sample: " << sample << std::endl;
					}
					chorus_[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
//					To use and effect type:
//						" rack.bank["effect"	][channel]->process(outputChannels[channel][sample], outputChannels[channel][sample]); "
//						rack.bank[flanger][channel]->process(outputChannels[channel][sample], outputChannels[channel][sample]);
//						rack.bank[2][channel]->process(saws[channel].getSample(), outputChannels[channel][sample]);


//					rack.bank[waveshaper][channel]->process(outputChannels[channel][sample], outputChannels[channel][sample]);
//
//					for (int i = 0; i < rack.bank.size(); i++){
//						rack.bank[i][channel]->process(outputChannels[channel][sample], outputChannels[channel][sample]);
//					}

//					Maybe faster implementation.
//					for (auto & effect : rack.bank){
//						effect[channel]->process(outputChannels[channel][sample], outputChannels[channel][sample]);
//					}

//					std::cout << outputChannels[channel][sample];
					/*
                    //make the audio source circle
                    source.setPolarPosition(1.0f, angle);
                    angle += 0.0001f;
                    if (angle > 6.28f)
                        angle -= 6.28f;

                    // //calculate amplitude and delay per panner based on source position
                    panner[channel].calcAmplitude(source);
                    panner[channel].calcDelay(source);

                    //calculate the effects


                    flangers[channel].process(saws[channel].getSample(), outputChannels[channel][sample]);
                    chorus[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
                    decorrelators[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
                    
                    //apply panning
                    panner[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
                    
                    //apply reverb (do this after panning so the reverb does not get panned)
                    reverbs[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);

                    */


                }
				channel++;
            }
        }

//		Initiate the effect rack.
		Rack rack {Rack(2)};
    	std::array<Sine, 2> sines { Sine(400, 0.5f), Sine(400, 0.5f) };
    	std::array<Sawtooth, 2> saws { Sawtooth(300, 0.5f), Sawtooth(300, 0.5f) };
		std::array<Chorus, 2> chorus_ { Chorus(0.5f, 0.5f, 1.0f, 0.0f), Chorus(0.5f, 0.5f, 1.0f, 0.5f) };
//		initiate the Panner vector.
		std::vector<Panner*> panner;
    	Object source { Object() };
};


int main() {
//	std::cout << "How many outputs?\n";
//	std::cin >> outputs_;



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
                std::cout << "Enter dry wet: "; std::cin >> dryWet;

                continue;
            case 'b':
                bool bypass;
                std::cout << "Enter dry wet: "; std::cin >> bypass;

                continue;
            case 's':
                float amp;
                std::cout << "Enter saw amp: "; std::cin >> amp;
                callback.saws[0].setAmplitude(amp);
                callback.saws[1].setAmplitude(amp);

				continue;
		}
	}



    return 0;
}