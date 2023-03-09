#pragma once

#include <cmath>
#include <vector>
#include "../libs/Util.h"

/*Class that will do the panning calculations based on the speaker positions
and hardware input*/

//----------------------------------------------------------------------

using uint = unsigned int;

struct Object {
        float xPos { 0.0f };
        float yPos { 0.0f };
        float radius { 1.0f };
        float angle { 90.0f };

        /*Sets the position of the object in cartesian form
        xPos: X-coordinate of the object
        yPos: Y-coordinate of the object
        */
        void setCartesianPosition(float xPos, float yPos);

        /*Sets the position of the object in polar form
        Radius: Radius of the position from the origin
        Angle: Angle in radians
        Degrees: set to true to enter angle in degrees
        */
        void setPolarPosition(float radius, float angle, bool degrees = false); 
};

//----------------------------------------------------------------------

class Panner {
    public:

        /*Constructor that defaults the speaker amount to 4 and position to a circle.
        Also constructs one source which is front centered*/
        Panner(uint numSpeakers, uint numSources);


        //Destructor
        ~Panner();

        /*Calculates the distance between a source and a speaker and returns the gain
        of the speaker for that source.
        source: the audio source object
        speaker: the speaker object
        */
        float getSpeakerAmplitude(const Object& source, const Object& speaker) const;
        
        /*Returns the distance between a source and a speaker.
        source: the audio source object
        speaker: the speaker object
        */
        float getDistance(const Object& source, const Object& speaker) const;

        /*Returns the peed (movement) of a source.
        source: the source object
        */
        float getSpeed(const Object& source);

        std::vector<Object> speakers;
        std::vector<Object> sources;
        int numSpeakers;
        int numSources;
        float speedBuf { 0.0f };
        float soundRadius { 1.8f };
};