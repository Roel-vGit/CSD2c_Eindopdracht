#include "panner.h"

//----------------------------------------------------------------------

void Object::setCartesianPosition(float xPos, float yPos)
{
    this->xPos = xPos;
    this->yPos = yPos;
    this->radius = Util::calcRadius(xPos, yPos);
    this->angle = Util::calcAngle(xPos, yPos);
}

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

Panner::~Panner()
{

}

float Panner::getSpeakerAmplitude(const Object& source, const Object& speaker) const
{
    float distance = (soundRadius - getDistance(source, speaker)) / soundRadius;
    if (distance < 0.0f) 
        distance = 0.0f;
    return distance;
}

float Panner::getDistance(const Object& source, const Object& speaker) const
{
    return Util::calcRadius(speaker.xPos - source.xPos, speaker.yPos - source.yPos);
}

float Panner::getSpeed(const Object& source)
{
    speedBuf = (source.xPos + source.yPos) - speedBuf;
    return speedBuf;
}