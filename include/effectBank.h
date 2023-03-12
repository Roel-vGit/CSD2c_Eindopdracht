//
// Created by Jens on 12/03/2023.
//

#pragma once
#include "../libs/Util.h"
#include "effect.h"
#include <vector>



class EffectBank{
public:
	EffectBank(int outputs);
	~EffectBank();

	//Add an effect to the effect bank
	void addEffect(Effect* effect, int amount);

private:
	int outputs;
	std::vector<std::vector<Effect*>> effects_;

};

/*
 Number of outputs
*/



