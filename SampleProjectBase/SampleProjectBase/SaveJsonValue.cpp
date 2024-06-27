#include "pch.h"
#include "SaveJsonValue.h"

void SaveJsonValue::WriteJson(nlohmann::json& _jsonData)
{
	// ”z—ñ“à‚Ì‘S‚Ä‚ðjson‚É‘ã“ü‚·‚é
	for (auto itr = saveValues.begin(); itr != saveValues.end(); itr++)
	{
		itr->JasonSave(_jsonData);
	}
}
