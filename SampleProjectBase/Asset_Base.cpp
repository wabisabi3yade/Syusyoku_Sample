#include "Asset_Base.h"

void Asset_Base::SetName(const std::string& _name)
{
	name = _name;
}

std::string Asset_Base::GetName() const
{
	return name;
}