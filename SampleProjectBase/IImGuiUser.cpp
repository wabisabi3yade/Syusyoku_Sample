#include "pch.h"

#ifdef EDIT
bool HashiTaku::IImGuiUser::isDisplay = true;
#else
bool HashiTaku::IImGuiUser::isDisplay = false;
#endif // EDIT

void HashiTaku::IImGuiUser::ImGuiCall()
{
	//if (isDisplay) ImGuiSetting();

#ifdef EDIT
	ImGuiSetting();
#endif // EDIT
}

bool HashiTaku::IImGuiUser::GetIsImGuiUse()
{
	return isDisplay;
}
