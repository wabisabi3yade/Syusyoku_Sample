#include "pch.h"
#include "CP_Button.h"

void CP_Button::OnEvent()
{
	// �ݒ肵���֐����N����
	eventFunction();
}

void CP_Button::SetEventFunction(const std::function<void()>& _setFunction)
{
	eventFunction = _setFunction;
}
