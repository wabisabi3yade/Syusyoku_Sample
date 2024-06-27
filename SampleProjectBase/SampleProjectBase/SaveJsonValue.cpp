#include "pch.h"
#include "SaveJsonValue.h"

void SaveJsonValue::WriteJson(nlohmann::json& _jsonData)
{
	// 配列内の全てをjsonに代入する
	for (auto itr = saveValues.begin(); itr != saveValues.end(); itr++)
	{
		itr->JasonSave(_jsonData);
	}
}
