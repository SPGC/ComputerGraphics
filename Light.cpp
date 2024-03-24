#include "Light.h"

Light::Light(std::string name, DirectXManager* dm, CVector3 colour, float strength) : Object(name, dm)
{
	this->colour = colour;
	this->strength = strength;
}
Light::Light() : Object()
{
}

CVector3 Light::getColour() const 
{
	return colour;
}
float Light::getStrength() const 
{
	return strength;
}

void Light::updateColour(CVector3 colour)
{
	this->colour = colour;
}
void Light::updateStrength(float strength)
{
	this->strength = strength;
}