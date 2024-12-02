#include "pch.h"
#include "CP_TitleButtonGroup.h"
#include "InSceneSystemManager.h"

CP_TitleButtonGroup::CP_TitleButtonGroup()
{
	// �T�C�Y�ύX
	u_int buttonCnt = static_cast<u_int>(ButtonType::MaxNum);
	buttonGroup.resize(buttonCnt);
	buttonObjNames.resize(buttonCnt);
}

void CP_TitleButtonGroup::Start()
{
	CP_ButtonGroup::Start();

	ButtonSetup();
}

void CP_TitleButtonGroup::ButtonSetup()
{
	u_int buttonCnt = static_cast<u_int>(ButtonType::MaxNum);
	
	SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();
	for (u_int b_i = 0; b_i < buttonCnt; b_i++)
	{
		// �ݒ肵���{�^���̖��O����{�^���R���|�[�l���g���擾
		GameObject* pGO = sceneObjs.GetSceneObject(buttonObjNames[b_i]);
		CP_Button* pGetButton = pGO->GetComponent<CP_Button>();
		buttonGroup[b_i] = pGetButton;

		if (!pGetButton)	// �{�^���擾�o���Ȃ�������
			HASHI_DEBUG_LOG(buttonObjNames[b_i] + "����{�^�����擾�ł��܂���ł���");
	}

	// �֐���ݒ肵�Ă���
	// �X�^�[�g
	CP_Button* pButton = buttonGroup[static_cast<u_int>(ButtonType::Start)];
	pButton->SetEventFunction([]()
		{
			HASHI_DEBUG_LOG("Start");
		});

	// �G���h
	pButton = buttonGroup[static_cast<u_int>(ButtonType::End)];
	pButton->SetEventFunction([]()
		{
			HASHI_DEBUG_LOG("End");
		});
}

void CP_TitleButtonGroup::ImGuiDebug()
{
	CP_ButtonGroup::ImGuiDebug();

	// �I�u�W�F�N�g���Z�b�g
	static char inputText[IM_INPUT_BUF];
	ImGui::InputText("objName", inputText, IM_INPUT_BUF);

	u_int buttonCnt = static_cast<u_int>(ButtonType::MaxNum);
	for (u_int b_i = 0; b_i < buttonCnt; b_i++)
	{
		ButtonType type = static_cast<ButtonType>(b_i);
		std::string typeName = std::string(magic_enum::enum_name<ButtonType>(type));

		// �{�^���Ŗ��O���Z�b�g
		if (ImGui::Button(typeName.c_str()))
		{
			buttonObjNames[b_i] = inputText;
			break;
		}
	}
}
