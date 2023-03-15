#pragma once

#include <cmath>
#include <vector>
#include "../libs/util.h"
#include "effect.h"
#include "delay.h"

/*Class that will do the panning calculations based on the speaker positions
and hardware input*/

//----------------------------------------------------------------------

using uint = unsigned int;

struct Object {

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

        float getX() const;
        float getY() const;
        float getAngle() const;
        float getRadius() const;

        //Returns the speed (movement) of an object. Call the calcSpeed() function first.
        float getSpeed();
        
        //calculates the speed of an object. Only call this function once in a callback.
        void calcSpeed();

        private:
        
        float xPos { 0.0f };
        float yPos { 0.0f };
        float radius { 1.0f };
        float angle { 90.0f };
        float speed { 0.0f };
        float sum { 0.0 };
};

//----------------------------------------------------------------------

class Speaker : public Effect, public Object {
    public:

        /*Constructor that constructs the objects and positions them in a circle.
        Also constructs one source which is front centered*/
        Speaker();

        //Destructor
        ~Speaker();

        void prepareToPlay(int sampleRate) override;

        /*Processes the speaker amplitude and delay time based on the source position
        Calculate the amplitude and delay time first using the calcAmplitude() and calcDelay() functions*/
        void calculate(const float& input, float& output) override;

        /*Calculates the distance between a source and a speaker and returns the gain
        of the speaker for that source position.
        source: the audio source object */
        void calcAmplitude(const Object& source);
        
        /*Calculates the delay time of a speaker based on the position of a sound source
        source: the audio source object */
        void calcDelay(const Object& source);

        /*Returns the distance between a source and a speaker.
        source: the audio source object */
        float getDistance(const Object& source) const;

        float amplitude { 1.0f };
        float delayTime { 0.0f };
        float soundRadius { 1.8f };
        float maxDelay { 1.0 };

        Delay delay { Delay(10.0f, 0.0f) };
};