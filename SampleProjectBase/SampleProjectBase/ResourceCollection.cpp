#include "pch.h"
#include "ResourceCollection.h"

bool ResourceCollection::GetImpotred(std::string _resourceName)
{
	// –¼‘O‚©‚ç’T‚·
	auto itr = resources.find(_resourceName);
	if (itr != resources.end())	// –¼‘O‚ª”z—ñ“à‚É‚ ‚ê‚Î
	{
		return true;	// “ü‚Á‚Ä‚¢‚é‚±‚Æ‚ð•Ô‚·
	}

	return false;	// “ü‚Á‚Ä‚¢‚È‚¢‚±‚Æ‚ð•Ô‚·
}
