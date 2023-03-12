//
// Created by Jens on 12/03/2023.
//

#include "effectBank.h"

EffectBank::EffectBank(int outputs) : outputs(outputs) {
	//  Create a 2D vector with 3 rows and 0 columns.
	effects_ = std::vector<std::vector<Effect*>>(3, std::vector<Effect*>(0));
}

EffectBank::~EffectBank() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < effects_[i].size(); j++) {
			delete effects_[i][j];
		}
	}
}

void EffectBank::addEffect(Effect* effect, int amount) {
	std::vector<Effect*> instances;
	for (int i = 0; i < amount; i++) {
		instances.push_back(effect);
	}
	effects_.push_back(instances);
}




