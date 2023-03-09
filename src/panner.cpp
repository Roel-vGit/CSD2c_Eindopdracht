#include "panner.h"

//----------------------------------------------------------------------

/*Sets the position of the object in cartesian form
xPos: X-coordinate of the object
yPos: Y-coordinate of the object
*/
void Object::setCartesianPosition(float xPos, float yPos)
{
    this->xPos = xPos;
    this->yPos = yPos;
    this->radius = Util::calcRadius(xPos, yPos);
    this->angle = Util::calcAngle(xPos, yPos);
}

/*Sets the position of the object in polar form
Radius: Radius of the position from the origin
Angle: Angle in radians
Degrees: set to true to enter angle in degrees
*/
void Object::setPolarPosition(float radius, float angle, bool degrees /*= false*/)
{
    this->radius = radius;

    if (!degrees)
        this->angle = angle;
    else
        this->angle = Util::degreesToRadians(angle);
    
    this->xPos = Util::calcX(radius, angle);
    this->yPos = Util::calcY(radius, angle);
}

//----------------------------------------------------------------------

/*Constructor that defaults the speaker amount to 4 and position to a circle.
Also constructs one source which is front centered*/
Panner::Panner(uint numSpeakers, uint numSources) : numSpeakers(numSpeakers), numSources(numSources)
{
    for (uint i = 0; i < numSpeakers; i++)
    {
        speakers.push_back(Object());
        speakers[i].setPolarPosition(1.0f, 135 - (90 * i), true);
    }
        
    for (uint i = 0; i < numSources; i++)
        sources.push_back(Object());

}

//Destructor
Panner::~Panner()
{

}

/*Calculates the distance between a source and a speaker and returns the gain
of the speaker for that source.
source: the audio source object
speaker: the speaker object
*/
float Panner::getSpeakerAmplitude(const Object& source, const Object& speaker) const
{
    float distance = (soundRadius - getDistance(source, speaker)) / soundRadius;
    if (distance < 0.0f) 
        distance = 0.0f;
    return distance;
}

/*Calculates the distance between a source and a speaker.
source: the audio source object
speaker: the speaker object
*/
float Panner::getDistance(const Object& source, const Object& speaker) const
{
    return Util::calcRadius(speaker.xPos - source.xPos, speaker.yPos - source.yPos);
}

/*Returns the peed (movement) of a source.
source: the source object
*/
float Panner::getSpeed(const Object& source)
{
    speedBuf = (source.xPos + source.yPos) - speedBuf;
    return speedBuf;
}