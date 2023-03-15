//
// Created by Jens on 15/03/2023.
//

#pragma once
#include <vector>
#include "effect.h"
using namespace std;

class Rack{
public:
	Rack(int outputs) {outputs_ = outputs;}
	~Rack(){};

//	Function to add an effect to the rack.
	void addEffect(Effect* effect){
		vector<Effect*> temp;
		for (int i = 0;i < outputs_; i++){
			Effect* newEffect = effect->clone();
			temp.push_back(newEffect);
		}
		bank.push_back(temp);
	}

	int outputs_;
	vector<vector<Effect*>> bank;
	vector<int> intBank;
};