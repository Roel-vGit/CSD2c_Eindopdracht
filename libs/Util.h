
#include <cmath>
#include <cstdlib>
#include <ctime>


#pragma once

static const float pi = acos(-1);

namespace Util
{
    double mapInRange(double value, double inputMin, double inputMax, double outputMin, double outputMax);

    double linearMap(double factor, double min, double max);

    int random(int maxValue);

    float gainToDecibels(float gain);

    float calcRadius(float x, float y);

    float calcAngle(float x, float y);

    float calcX(float radius, float angle);

    float calcY(float radius, float angle);

    float radiansToDegrees(float radians);

    float degreesToRadians(float degrees);
        
    float msToSamples(float ms, int sampleRate);

    float samplesToMs(float samples, int sampleRate);

}// Util