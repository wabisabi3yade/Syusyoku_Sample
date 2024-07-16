#pragma once

class IPrototype
{
public:
	virtual IPrototype* Clone()const = 0;
};