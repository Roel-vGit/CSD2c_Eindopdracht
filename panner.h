#pragma once

#include <cmath>

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
            speakers = new Object[numSpeakers];
            sources = new Object[numSources];
        }

        ~Panner() 
        { 
            delete[] speakers;
            std::cout << "Deleted speakers" << std::endl;
            delete[] sources;
            std::cout << "Deleted sources" << std::endl;
        }

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

        Object* speakers;
        Object* sources;
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

