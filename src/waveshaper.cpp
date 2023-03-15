//
// Created by Jens on 14/02/2023.
//

#include "waveshaper.h"

// Default constructor.
WaveShaper::WaveShaper() {
	this->type = "WaveShaper";
	this->k = 1;
	bufferSize = 2048;
	buffer = new float[bufferSize];
	setDrive(k);

}


WaveShaper::WaveShaper(float k) {
	this->type = "WaveShaper";
	this->k = k;
	bufferSize = 2048;
	buffer = new float[bufferSize];
	setDrive(k);

}
// Constructor with buffer size.
WaveShaper::WaveShaper(uint size) : buffer(new float[size]){
	bufferSize = size;
}

WaveShaper::~WaveShaper() {
	delete[] buffer;
}

// Calculate the effect.
void WaveShaper::calculate(const float& input, float& output) {
//	Make an index, scale the bipolar input value to the unipolar bufferSize.
	float index = (input + 1) * (bufferSize / 2 - 1);

//	Save the int value of the index in the i variable.
	int i = (int) index;

//	Subtract the decimals from the index value.
	float indexDecimal = index - (float) i;

//	Output the value corresponding with the index.
 	output = Util::linearMap(indexDecimal, buffer[i], buffer[i + 1]);
}

void WaveShaper::setDrive(float k) {
	this->k = k;
//	Python plotter for the look-up table.
//	WriteToFile fileWriter("output.csv", true);

//	Function to write the look-up table to the buffer.
	for (int i = 0; i <= bufferSize -1; i++){
		double x = Util::mapInRange(i, 0.0f, bufferSize -1, -1.0f, 1.0f);
		buffer[i] = (atan(x * k) / atan(k));
//		fileWriter.write(std::to_string(buffer[i]) + "\n");
	}
}