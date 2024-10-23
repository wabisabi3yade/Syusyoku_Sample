#include "pch.h"
#include "Tag.h"

#ifdef EDIT
std::vector<std::string> g_tagTypeNames
{
	"Default",	// デフォルト
	"MainCamera",	// カメラ
	"Player",	// プレイヤー
	"Enemy",	// 敵
	"Ground",	// 地面
};

std::vector<std::string> g_layerTypeNames
{
	"Default",	// デフォルト
	"Object",	// オブジェクト
	"UI",	// UI
};
#endif // EDIT


bool Tag::ImGuiComboBox(Type& _curType)
{
#ifdef EDIT
	u_int curId = static_cast<u_int>(_curType);

	if (ImGuiMethod::ComboBox("Tag", curId, g_tagTypeNames))
	{
		_curType = static_cast<Type>(curId);
		return true;
	}
#endif // EDIT
	return false;
}

bool Layer::ImGuiComboBox(Type& _curType)
{
#ifdef EDIT
	u_int curId = static_cast<u_int>(_curType);

	if (ImGuiMethod::ComboBox("Layer", curId, g_layerTypeNames))
	{
		_curType = static_cast<Type>(curId);
		return true;
	}
#endif // EDIT
	return false;
}
