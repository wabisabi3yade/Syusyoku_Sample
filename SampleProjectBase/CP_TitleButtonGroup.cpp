#include "pch.h"
#include "CP_TitleButtonGroup.h"
#include "InSceneSystemManager.h"

CP_TitleButtonGroup::CP_TitleButtonGroup()
{
	// �T�C�Y�ύX
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

	// �֐���ݒ肵�Ă���
	// �X�^�[�g
	CP_Button* pButton = buttonGroup[static_cast<u_int>(ButtonType::Start)];
	if(pButton)
	pButton->SetEventFunction([]()
		{
			HASHI_DEBUG_LOG("Start");
		});

	// �G���h
	pButton = buttonGroup[static_cast<u_int>(ButtonType::End)];
	if (pButton)
	pButton->SetEventFunction([]()
		{
			HASHI_DEBUG_LOG("End");
		});
}
