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

#include "../libs/osc.h"

localOSC osc;

class Callback : public AudioCallback {
    public:
        void prepare(int sampleRate) override
        {
            for (int i = 0; i < numOutputs; i++)
            {   
                flangers[i].prepareToPlay(sampleRate);
                flangers[i].setDryWet(0.5f);
                chorus[i].prepareToPlay(sampleRate);
                chorus[i].setDryWet(0.5f);
                chorus[i].setType("Chorus");
                decorrelators[i].prepareToPlay(sampleRate);
                decorrelators[i].setDryWet(1.0f);
                decorrelators[i].setType("Decorrelator");
                speaker[i].prepareToPlay(sampleRate);
                reverbs[i].prepareToPlay(sampleRate);
                reverbs[i].setDryWet(1.0f);
            }
                //set the speaker positions
                speaker[0].setPolarPosition(1.0f, 135, true);
                speaker[1].setPolarPosition(1.0f, 45, true);
                speaker[2].setPolarPosition(1.0f, 315, true);
                speaker[3].setPolarPosition(1.0f, 225, true);

				angleStep = 0.0001f;
                sample1 = 0.0f;
        }
           

        void process(AudioBuffer buffer) override
        {
            auto [inputChannels, outputChannels, numInputChannels, numOutputChannels, numFrames] = buffer;
            for (int channel = 0u; channel < numOutputChannels; ++channel) {  
                for (int sample = 0u; sample < numFrames; ++sample)
                {   
                    //test tone
                    saws[channel].tick();

                    //receive the controller values here (do this in auxilliary task in Bela)
                    //-----------------------------------------------------------------------

                    //make the audio source circle
                    joystick1.setCartesianPosition(osc.joystick1Xpos, osc.joystick1Ypos);
					joystick2.setCartesianPosition(osc.joystick2Xpos, osc.joystick2Ypos);
					touchpad1.setCartesianPosition(osc.touchPad1Xpos, osc.touchPad1Ypos);
					touchpad2.setCartesianPosition(osc.touchPad2Xpos, osc.touchPad2Ypos);
					angle += angleStep;

					joystick1.calcSpeed();
					joystick2.calcSpeed();

                    //calculate amplitude and delay per speaker based on joystick1 position
                    speaker[channel].calcAmplitude(touchpad2);
                    speaker[channel].calcDelay(touchpad2);


                    //adjust parameters here (TODO: more parameter changes to be added)
                    //-----------------------------------------------------------------------

					// flanging based on movement of the joysticks
					flangers[channel].setDryWet(touchpad2.getSpeed());

                    //chorus based on radius of touchPad and depth based on angle
					chorus[channel].setDryWet(touchpad1.getRadius());
                    chorus[channel].setDepth(touchpad1.getAngle(true) / 3.6f);
                    // std::cout << chorus[channel].getDryWet() << std::endl;

                    //decorrelator based on radius of touchpad2
					decorrelators[channel].setDryWet(touchpad1.getRadius());
                    

                    //reverb parameters
                    reverbs[channel].setDamping(1.0f - touchpad2.getRadius());
                    reverbs[channel].setDecay(touchpad2.getAngle(true) / 360.0f);
                    // reverbs[channel].setDryWet(touchpad1.getRadius());

                    //calculate the effects
                    //-----------------------------------------------------------------------   

                    flangers[channel].process(saws[channel].getSample(), outputChannels[channel][sample]);
                    chorus[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
                    // decorrelators[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
                    // sample1 = outputChannels[channel][sample];
                    
                    // outputChannels[channel][sample] = saws[channel].getSample();

                    //apply panning
                    speaker[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);
                    
                    //apply reverb (do this after panning so the reverb does not get panned)
                    reverbs[channel].process(outputChannels[channel][sample], outputChannels[channel][sample]);


                }
            }
        }
    static const int numOutputs = 3;

    std::array<Sine, numOutputs> sines { Sine(400, 0.5f), Sine(400, 0.5f) };
    std::array<Sawtooth, numOutputs> saws { Sawtooth(300, 0.9f), Sawtooth(300, 0.9f) };
    std::array<Chorus, numOutputs> chorus { Chorus(0.35f, 1.0f, 10), Chorus(0.4f, 1.2f, 15, 0.5f) } ;
    std::array<Decorrelator, numOutputs> decorrelators { Decorrelator(), Decorrelator() };
    std::array<Flanger, numOutputs> flangers { Flanger(), Flanger() };
    std::array<Reverb, numOutputs> reverbs { Reverb(), Reverb() };
    std::array<Panner, numOutputs> speaker { Panner(), Panner() };
    Object joystick1 { Object() };
	Object joystick2 { Object() };
	Object touchpad1 { Object() };
	Object touchpad2 { Object() };
    float angle = { 0.0f };
	float angleStep { 0.0001f };
    float sample1 = { 0.0f };
    float impulse = { 0.0f };
    std::array<WaveShaper, numOutputs> waveShapers { WaveShaper(4.0f), WaveShaper(4.0f), WaveShaper(4.0f) };
};


int main() {

    auto callback = Callback {};
    auto jack = JackModule (callback);

    jack.init(2,2);


    std::string serverport="7563";

    osc.init(serverport);
    osc.set_callback("/joystick1","ff");
    osc.set_callback("/joystick2","ff");
    osc.set_callback("/touchpad1", "ff");
    osc.set_callback("/touchpad2","ff");

  osc.start();
  std::cout << "Listening on port " << serverport << std::endl;

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
                callback.reverbs[0].setDryWet(dryWet);
                callback.reverbs[1].setDryWet(dryWet);
                std::cout << "chorus L:" << callback.chorus[0].getDryWet() << std::endl;
                std::cout << "chorus R:" << callback.chorus[0].getDryWet() << std::endl;
                continue;
            case 'b':
                bool bypass;
                std::cout << "Enter bypass: ";
                std::cin >> bypass;
                callback.chorus[0].setBypass(bypass);
                callback.chorus[1].setBypass(bypass);
                callback.flangers[0].setBypass(bypass);
                callback.flangers[1].setBypass(bypass);
                callback.decorrelators[0].setBypass(bypass);
                callback.decorrelators[1].setBypass(bypass);
                continue;
            case 's':
                float amp;
                std::cout << "Enter saw amp: ";
                std::cin >> amp;
                callback.saws[0].setAmplitude(amp);
                callback.saws[1].setAmplitude(amp);
			case 'y':
				for (Panner& speaker : callback.speaker)
				{
					std::cout << "delay: " << speaker.getDelay() << std::endl;
					std::cout << "distance: " << speaker.getDistance(callback.joystick1) << std::endl;
				}
				continue;
			case 'p':
				float speed;
				std::cout << "Set speed: ";
				std::cin >> speed;
				callback.angleStep = speed;
            case 'i':
                callback.impulse = 1.0f;
                callback.impulse = 0.0f;

        }   

    }


    return 0;
}