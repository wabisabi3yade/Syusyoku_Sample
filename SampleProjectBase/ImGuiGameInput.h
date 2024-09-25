#pragma once
#include "GameInput.h"

class ImGuiGameInput
{
public:
	static bool InputComboBox(const std::string& _caption, GameInput::ButtonType& _currentType);
};

