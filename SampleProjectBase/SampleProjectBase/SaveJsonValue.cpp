#include "pch.h"
#include "SaveJsonValue.h"

void SaveJsonValue::WriteJson(nlohmann::json& _jsonData)
{
	// �z����̑S�Ă�json�ɑ������
	for (auto itr = saveValues.begin(); itr != saveValues.end(); itr++)
	{
		itr->JasonSave(_jsonData);
	}
}
