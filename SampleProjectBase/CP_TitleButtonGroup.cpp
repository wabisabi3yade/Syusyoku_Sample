#include "pch.h"
#include "CP_TitleButtonGroup.h"
#include "InSceneSystemManager.h"
#include "SceneManager.h"
#include "CP_Fade.h"

namespace HashiTaku
{
	CP_TitleButtonGroup::CP_TitleButtonGroup() :
		pFade(nullptr)
	{
		// �T�C�Y�ύX
		SetMaxButtonCnt(static_cast<u_int>(ButtonType::MaxNum));
	}

	nlohmann::json CP_TitleButtonGroup::Save()
	{
		auto data = CP_ButtonGroup::Save();
		data["fadeObj"] = fadeObjName;

		return data;
	}

	void CP_TitleButtonGroup::Load(const nlohmann::json& _data)
	{
		CP_ButtonGroup::Load(_data);
		LoadJsonString("fadeObj", fadeObjName, _data);
	}

	void CP_TitleButtonGroup::Start()
	{
		CP_ButtonGroup::Start();

		ButtonSetup();

		FindObject();
	}

	void CP_TitleButtonGroup::FindObject()
	{
		GameObject* pObj =
			InSceneSystemManager::GetInstance()->GetSceneObjects().GetSceneObject(fadeObjName);
		if (!pObj) return;

		pFade = pObj->GetComponent<CP_Fade>();
		if (!pFade) return;

		// �t�F�[�h�J���ɓ��͂ł���悤��
		canInput = false;
		pFade->SetOnEndFunction([&]()
			{
				canInput = true;
			});
	}

	void CP_TitleButtonGroup::OnPushStart()
	{
		// �t�F�[�h��ɃV�[�����ړ�
		if (pFade)
		{
			pFade->CloseFade();

			pFade->SetOnEndFunction([&]()
				{
					SceneManager::GetInstance()->ChangeSceneRequest("BossBattle");
				});
		}
		else
		{
			SceneManager::GetInstance()->ChangeSceneRequest("BossBattle");
		}

		canInput = false;
	}

	void CP_TitleButtonGroup::OnPushEnd()
	{
		// �t�F�[�h��ɃV�[�����ړ�
		if (pFade)
		{
			pFade->CloseFade();

			pFade->SetOnEndFunction([&]()
				{
					MainApplication::ExitRequest();
				});
		}
		else
		{
			MainApplication::ExitRequest();
		}

		canInput = false;
	}

	void CP_TitleButtonGroup::ButtonSetup()
	{
		u_int buttonCnt = static_cast<u_int>(ButtonType::MaxNum);

		// �֐���ݒ肵�Ă���
		// �X�^�[�g
		CP_Button* pButton = buttonGroup[static_cast<u_int>(ButtonType::Start)];
		if (pButton)
			pButton->SetEventFunction([&]()
				{
					OnPushStart();
				});

		// �G���h
		pButton = buttonGroup[static_cast<u_int>(ButtonType::End)];
		if (pButton)
			pButton->SetEventFunction([&]()
				{
					OnPushEnd();
				});
	}

	void CP_TitleButtonGroup::ChangeBattleScene()
	{

	}

	void CP_TitleButtonGroup::ImGuiDebug()
	{
#ifdef EDIT
		CP_ButtonGroup::ImGuiDebug();

		static char input[IM_INPUT_BUF] = "\0";
		std::strcpy(input, fadeObjName.c_str());
		ImGui::InputText("objName", input, IM_INPUT_BUF);
		fadeObjName = input;
#endif // EDIT
	}
}