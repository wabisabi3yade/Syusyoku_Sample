#include "pch.h"
#include "CP_TitleButtonGroup.h"
#include "InSceneSystemManager.h"

CP_TitleButtonGroup::CP_TitleButtonGroup()
{
	// サイズ変更
	maxButtonCnt = static_cast<u_int>(ButtonType::MaxNum);
	buttonGroup.resize(maxButtonCnt);
	buttonObjNames.resize(maxButtonCnt);
}

void CP_TitleButtonGroup::Start()
{
	CP_ButtonGroup::Start();

	ButtonSetup();
}

void CP_TitleButtonGroup::ButtonSetup()
{
	u_int buttonCnt = static_cast<u_int>(ButtonType::MaxNum);

	// 関数を設定していく
	// スタート
	CP_Button* pButton = buttonGroup[static_cast<u_int>(ButtonType::Start)];
	if(pButton)
	pButton->SetEventFunction([]()
		{
			HASHI_DEBUG_LOG("Start");
		});

	// エンド
	pButton = buttonGroup[static_cast<u_int>(ButtonType::End)];
	if (pButton)
	pButton->SetEventFunction([]()
		{
			HASHI_DEBUG_LOG("End");
		});
}
