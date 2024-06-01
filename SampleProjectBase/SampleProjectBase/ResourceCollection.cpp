#include "pch.h"
#include "ResourceCollection.h"

bool ResourceCollection::GetImpotred(std::string _resourceName)
{
	// 名前から探す
	auto itr = resources.find(_resourceName);
	if (itr != resources.end())	// 名前が配列内にあれば
	{
		return true;	// 入っていることを返す
	}

	return false;	// 入っていないことを返す
}
