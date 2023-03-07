#pragma once

#include <cmath>

/*Class that will do the panning calculations based on the speaker positions
and hardware input*/

using uint = unsigned int;

struct Object {
        float xPos;
        float yPos;
        float radius;
        float angle;
};

class Panner {
    public:
        Panner(uint numSpeakers, uint numSources) : numSpeakers(numSpeakers), numSources(numSources)
        {
            objects = new Object[numSpeakers + numSources];
        }

        ~Panner() { delete[] objects; }

        void setCartesianPosition(Object& object, float xPos, float yPos)
        {
            object.xPos = xPos;
            object.yPos = yPos;
            object.radius = calcRadius(xPos, yPos);
            object.angle = calcAngle(xPos, yPos);

        }

        void setPolarPosition(Object& object, float radius, float angle)
        {
            object.radius = radius;
            object.angle = angle;
            object.xPos = calcX(radius, angle);
            object.yPos = calcY(radius, angle);
        }

        float getDistance(Object source, Object& speaker)
        {
            return calcRadius(speaker.xPos - source.xPos, speaker.yPos - source.yPos);
        }

        Object* objects;

        
    private:

        float calcRadius(float x, float y)
        {
            return sqrt(pow(x, 2.0) + pow(y, 2.0));
        }

        float calcAngle(float x, float y)
        {
            return atan2(y,x);
        }

        float calcX(float radius, float angle)
        {
            return radius * cos(angle);
        }

        float calcY(float radius, float angle)
        {
            return radius * sin(angle);
        }

        int numSpeakers;
        int numSources;


        
};

