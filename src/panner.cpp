#include "panner.h"
#include <iostream>

// Struct Object
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

    if (degrees)
        angle = Util::degreesToRadians(angle);
    
    this->xPos = Util::calcX(radius, angle);
    this->yPos = Util::calcY(radius, angle);
}

float Object::getX() const
{
    return xPos;
}

float Object::getY() const
{
    return yPos;
}

float Object::getAngle() const
{
    return angle;
}

float Object::getRadius() const
{
    return radius;
}


//class Panner
//----------------------------------------------------------------------


Panner::Panner(uint numSpeakers, uint numSources) : numSpeakers(numSpeakers), numSources(numSources)
{
    for (uint i = 0; i < numSpeakers; i++)
    {
        speakers.push_back(Object());
    }
        
    for (uint i = 0; i < numSources; i++)
        sources.push_back(Object());

    speakers[0].setPolarPosition(1.0f, 135, true);
    speakers[1].setPolarPosition(1.0f, 45, true);
    speakers[2].setPolarPosition(1.0f, 315, true);
    speakers[3].setPolarPosition(1.0f, 225, true);

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

float Panner::getSpeakerDelay(const Object& source, const Object& speaker)
{
    float delay = getDistance(source, speaker) - Util::mapInRange(source.getRadius(), 0.0f, 0.707f, 1.0f, 0.707f);
    if (delay < 0.0001f)
        delay = 0.0f;
    return delay * maxDelay;
}


float Panner::getDistance(const Object& source, const Object& speaker) const
{   
    return Util::calcRadius(speaker.getX() - source.getX(), speaker.getY() - source.getY());
}

float Panner::getSpeed(const Object& source)
{
    speedBuf = (source.getX() + source.getY()) - speedBuf;
    return speedBuf;
}