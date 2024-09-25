#include "pch.h"
#include "ImGuiGameInput.h"

#ifdef EDIT
std::vector<std::string> gButtonTypeStrings =
{
	"Player_Attack",
	"Player_Jump"
};
#endif // EDIT

bool ImGuiGameInput::InputComboBox(const std::string& _caption, GameInput::ButtonType& _currentType)
{

	bool isChange = false;
#ifdef EDIT
	GameInput::ButtonType curType = _currentType;
	u_int curId = static_cast<u_int>(curType);
	isChange = ImGuiMethod::ComboBox(_caption, curId, gButtonTypeStrings);
	_currentType = static_cast<GameInput::ButtonType>(curType);
#endif // EDIT
	return isChange;
}
