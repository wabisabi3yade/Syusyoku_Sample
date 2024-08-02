#include "Asset_Base.h"

void Asset_Base::SetAssetName(const std::string& _name)
{
	name = _name;
}

std::string Asset_Base::GetAssetName() const
{
	return name;
}