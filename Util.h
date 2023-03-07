
#include <cmath>

#pragma once


namespace Util
{
    inline double mapInRange(double value, double inputMin, double inputMax, double outputMin, double outputMax)
    {
        double inputRange = inputMax - inputMin;
        double outputRange = outputMax - outputMin;
        double valueScaled = (value-inputMin)/inputRange;
        double output = valueScaled * outputRange + outputMin;
        return output;
    }

    inline double linearMap(double factor, double min, double max)
    {
        double output = mapInRange(factor, 0.0, 1.0, min, max);
        return output;
    }

    inline int random(int maxValue)
    {
        std::srand(clock());
        int randomInt = rand() % maxValue;
        return randomInt;
    }

}// Util