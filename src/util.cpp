#include "../libs/util.h"
#include <iostream>

namespace Util
{
    double mapInRange(double value, double inputMin, double inputMax, double outputMin, double outputMax)
    {
        double inputRange = inputMax - inputMin;
        double outputRange = outputMax - outputMin;
        double valueScaled = (value-inputMin)/inputRange;
        double output = valueScaled * outputRange + outputMin;
        return output;
    }

    double linearMap(double factor, double min, double max)
    {
        double output = mapInRange(factor, 0.0, 1.0, min, max);
        return output;
    }

    int random(int maxValue)
    {
        if (maxValue <= 0)
            return 0;
        std::srand(clock());
        int randomInt = rand() % maxValue;
        return randomInt;
    }

    float gainToDecibels(float gain)
    {
        return 20 * log10(gain);
    }

    float calcRadius(float x, float y)
    {
        return sqrt(x*x + y*y);
    }

    float calcAngle(float x, float y)
    {
        float angle = atan2(y,x);
        if (angle < 0.0f)
            return 2*pi + angle;
        else
            return angle;
    }

    float calcX(float radius, float angle)
    {
        return radius * cos(angle);
    }

    float calcY(float radius, float angle)
    {
        return radius * sin(angle);
    }

    float radiansToDegrees(float radians)
    {
        return (radians / pi) * 180.0f;
    }

    float degreesToRadians(float degrees)
    {
        return degrees * (pi/180.0f);
    }
        
    int msToSamples(float ms, int sampleRate)
    {
        return (ms/1000.0f) * sampleRate;
    }

    float samplesToMs(float samples, int sampleRate)
    {
        return (samples / sampleRate) * 1000.0f;
    }

	double sgn(double x)
	{
		return (x >= 0) ? 1.0 : -1.0;
	}

}