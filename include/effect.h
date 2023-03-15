//
// Created by Jens on 14/02/2023.
//

#pragma once
#include <iostream>
#include <string>

class Effect {
public:
	Effect();
	~Effect();

//	Process an effect (bypass and calculate).
	void process(const float& input, float& output);

//	Prepare function to load the sampleRate.
	virtual void prepareToPlay (int sampleRate);

	virtual Effect* clone() = 0;

//	calculate the effect (needs to be overridden).
	virtual void calculate(const float& input, float& output) = 0;

	void setDryWet(float dryWet);
	void setBypass(bool bypass);
	void setType(std::string type) { this->type = type; }

	std::string getType(){return this->type;}
	std::string getSampleRate() { return std::to_string(sampleRate); }

	float getDryWet() { return dryWet;}


protected:
	float input;
	int sampleRate { 0 };
	float dryWet = 1.0f;
	bool bypass = false;
	std::string type;
	std::string name;
};
