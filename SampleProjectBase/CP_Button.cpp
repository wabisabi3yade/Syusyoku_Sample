#include "pch.h"
#include "CP_Button.h"

namespace HashiTaku
{
	void CP_Button::OnEvent()
	{
		// 設定した関数を起こす
		eventFunction();
	}

	void CP_Button::SetEventFunction(const std::function<void()>& _setFunction)
	{
		eventFunction = _setFunction;
	}
}