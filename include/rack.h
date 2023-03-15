//
// Created by Jens on 15/03/2023.
//

#pragma once
#include "effect.h"
#include <vector>

class Rack{
public:
	Rack(int outputs) {outputs_ = outputs;}
	~Rack(){};

//	Add effect.
//	void addInt(int i){intBank.push_back(i);}
	void addEffect(Effect* effect) {
		std::vector<Effect*> temp;
		for (int i = 0; i < outputs_; i++) { temp.push_back(effect); }
		bank.push_back(temp);
	}


	int getBankSize(){return bank.size();}
	void getEffect(){
		for (auto & effects : bank){
			for( auto & instances : effects){
				std::cout << instances->getType() << " ";
			}
		}

		std::cout << std::endl;
	};


	std::vector< std::vector<Effect*> > bank;
protected:
	int outputs_;

	std::vector<int> intBank;
};
