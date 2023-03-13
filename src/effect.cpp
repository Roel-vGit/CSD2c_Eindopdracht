//
// Created by Jens on 14/02/2023.
//

#include "../include/effect.h"

Effect::Effect() {
}
Effect::~Effect() {
}

// Loading the sample rate.
void Effect::prepareToPlay(int sampleRate) { this->sampleRate = sampleRate; }

// Using constant float because you never want to change the input.
void Effect::process(const float& input, float& output) {
	try {
		if (sampleRate == 0)
			throw 404;
	} catch (int error) {
		std::cout << "Error " << error;
		std::cout << ". No sample rate known in " << typeid(*this).name() << ". You probably forgot to call prepareToPlay() dumbass..." << std::endl;
		exit(1); //exit the program
	}

	if (bypass){
		output = input;
		return;
	}
	calculate(input, output);
	output = (output * dryWet) + input * (1.0f - dryWet);
}

//sets the dryWet of the effect
void Effect::setDryWet(float dryWet) {
//	Set range to 0-1.
	if (dryWet > 1) dryWet = 1;
	else if (dryWet < 0) dryWet = 0;
	this->dryWet = dryWet;
}

//sets the effect in bypass mode
void Effect::setBypass(bool bypass) { this->bypass = bypass; }