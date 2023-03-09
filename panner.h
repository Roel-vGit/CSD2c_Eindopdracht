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

        enum class SoundSource  
        {
            Speaker, Source
        };

        Panner(uint numSpeakers, uint numSources) : numSpeakers(numSpeakers), numSources(numSources)
        {
            for (uint i = 0; i < numSpeakers; i++)
                speakers.push_back(Object());

            for (uint i = 0; i < numSpeakers; i++)
                sources.push_back(Object());
        }

        ~Panner() {}

        /*Sets the position of the object in cartesian form
        Object: The object to set the position for
        xPos: X-coordinate of the object
        yPos: Y-coordinate of the object
        */
        void setCartesianPosition(Object& object, float xPos, float yPos)
        {
            object.xPos = xPos;
            object.yPos = yPos;
            object.radius = calcRadius(xPos, yPos);
            object.angle = calcAngle(xPos, yPos);
        }

        /*Sets the position of the object in polar form
        Object: The object to set the position for
        Radius: Radius of the position from the origin
        Angle: Angle in radians
        Degrees: set to true to enter angle in degrees*/
        void setPolarPosition(SoundSource source, int index, float radius, float angle, bool degrees = false)
        {
            Object object;

            object.radius = radius;
            if (!degrees)
                object.angle = angle;
            else
                object.angle = degreesToRadians(angle);
            
            object.xPos = calcX(radius, angle);
            object.yPos = calcY(radius, angle);

            if (source == SoundSource::Speaker) {
                if (index > speakers.size())
                    throw std::invalid_argument("Speaker index out of bounds");
                speakers[index] = object;
            }

            if (source == SoundSource::Source) {
                if (index > sources.size())
                    throw std::invalid_argument("Source index out of bounds");
                sources[index] = object;
            }
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
        const float pi = acos(-1);

        inline float calcRadius(float x, float y)
        {
            return sqrt(pow(x, 2.0) + pow(y, 2.0));
        }

        inline float calcAngle(float x, float y)
        {
            return atan2(y,x);
        }

        inline float calcX(float radius, float angle)
        {
            return radius * cos(angle);
        }

        inline float calcY(float radius, float angle)
        {
            return radius * sin(angle);
        }

        inline float radiansToDegrees(float radians)
        {
            return (radians / pi) * 180.0f;
        }

        inline float degreesToRadians(float degrees)
        {
            return degrees * (pi/180.0f);
        }
};