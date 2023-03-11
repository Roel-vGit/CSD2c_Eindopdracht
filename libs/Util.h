
#include <cmath>
#include <cstdlib>
#include <ctime>


#pragma once

static const float pi = 3.14159265359;

namespace Util
{
    /*Scales an input value between an input range to an output range
    value: input value to be scaled
    inputMin: lower bound of input range
    inputMax: upper bound of input range
    outputMin: lower bound of output range
    outputMax: upper bound of output range*/
    double mapInRange(double value, double inputMin, double inputMax, double outputMin, double outputMax);

    /*Linearly interpolates a value between a lower and upper bound value
    factor: the factor to which the output is scaled
    min: lower bound
    max: higher bound
    */
    double linearMap(double factor, double min, double max);

    /*Generate a random integer
    The random seed is changed everytime the function is called. So it will generate a
    new random number when the function is called.
    maxValue: The maximum integer to be generated.
    */
    int random(int maxValue);

    //convert gain to decibels
    float gainToDecibels(float gain);

    //calculate the magnitude of a vector
    float calcRadius(float x, float y);

    //calculate the angle of a vector
    float calcAngle(float x, float y);

    //calculate the x-coordinate of a vector
    float calcX(float radius, float angle);

    //calculate the y-coordinate of a vector
    float calcY(float radius, float angle);

    //convert radians to degrees
    float radiansToDegrees(float radians);

    //convert degrees to radians
    float degreesToRadians(float degrees);

    //convert milliseconds to samples
    float msToSamples(float ms, int sampleRate);

    //convert samples to milliseconds
    float samplesToMs(float samples, int sampleRate);

}// Util