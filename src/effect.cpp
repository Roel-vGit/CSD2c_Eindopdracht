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
	}

	catch (int error) {
		std::cout << "Error " << error;
		std::cout << ". No sample rate entered. You probably forgot to call prepareToPlay() somewhere.. dumbass..." << std::endl;
	}

	if (bypass){
		output = input;
		return;
	}
	calculate(input, output);
	std::cout << "DryWet from " << type << ": " << this->dryWet << std::endl;
	output = (output * dryWet) + input * (1.0f - dryWet);
}

//sets the dryWet of the effect
void Effect::setDryWet(float dryWet) {
//	Set range to 0-1.
	if (dryWet > 1.0f) dryWet = 1.0f;
	else if (dryWet < 0.0f) dryWet = 0.0f;
	this->dryWet = dryWet;
	std::cout << "DryWet: " << this->dryWet << std::endl;
}

//sets the effect in bypass mode
void Effect::setBypass(bool bypass) { this->bypass = bypass; }