#pragma once
#include "Object.h"

class Light : public Object
{
public:
	Light(std::string name, DirectXManager* dm, CVector3 colour, float strength);
	Light();
	CVector3 getColour() const;
	float getStrength() const;
	void updateColour(CVector3 colour);
	void updateStrength(float strength);

private:
	CVector3 colour;
	float    strength;
};

