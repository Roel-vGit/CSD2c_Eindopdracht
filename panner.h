#pragma once

#include <cmath>
#include <vector>

/*Class that will do the panning calculations based on the speaker positions
and hardware input*/

using uint = unsigned int;

struct Object {
        float xPos { 0.0f };
        float yPos { 0.0f };
        float radius { 0.0f };
        float angle { 0.0f };
};

class Panner {
    public:
        Panner(uint numSpeakers, uint numSources) : numSpeakers(numSpeakers), numSources(numSources)
        {
            for (uint i = 0; i < numSpeakers; i++)
                speakers.push_back(Object());

            for (uint i = 0; i < numSpeakers; i++)
                sources.push_back(Object());
        }

        ~Panner() {}

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

        float getDistance(Object& source, Object& speaker)
        {
            return calcRadius(speaker.xPos - source.xPos, speaker.yPos - source.yPos);
        }

        float getSpeed(Object& source)
        {
            speedBuf = (source.xPos + source.yPos) - speedBuf;
            return speedBuf;
        }

        std::vector<Object> speakers;
        std::vector<Object> sources;
        int numSpeakers;
        int numSources;
        float speedBuf { 0.0f };

        
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

};