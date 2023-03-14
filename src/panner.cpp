#include "../include/panner.h"
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

float Object::getSpeed()
{
    speedBuf = (xPos + yPos) - speedBuf;
    return speedBuf;
}


//class Speaker
//----------------------------------------------------------------------


Speaker::Speaker()
{

}

Speaker::~Speaker()
{

}

void Speaker::prepareToPlay(int sampleRate)
{
    this->sampleRate = sampleRate;
    delay.prepareToPlay(sampleRate);
}

void Speaker::calculate(const float& input, float& output)
{
    delay.calculate(input, output);
    output = output * amplitude;
}

void Speaker::calcAmplitude(const Object& source)
{
    float distance = (soundRadius - getDistance(source)) / soundRadius;
    if (distance < 0.0f) 
        distance = 0.0f;
    this->amplitude = distance;
}

void Speaker::calcDelay(const Object& source)
{
    float delayValue = getDistance(source) - Util::mapInRange(source.getRadius(), 0.0f, 0.707f, 1.0f, 0.707f);
    if (delayValue < 0.0001f)
        delayValue = 0.0f;
    this->delayTime = delayValue * maxDelay;
    delay.setDelayTime(this->delayTime);
}

float Speaker::getDistance(const Object& source) const
{   
    return Util::calcRadius(getX() - source.getX(), getY() - source.getY());
}

