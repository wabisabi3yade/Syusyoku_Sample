#include "pch.h"
#include "CP_BattleButtonGroup.h"
#include "SceneManager.h"

namespace HashiTaku
{
	constexpr auto BATTLE_SCENE_NAME("BossBattle");	// �{�X�o�g���V�[����
	constexpr auto TITLE_SCENE_NAME("Title");	// �^�C�g���V�[����

	CP_BattleButtonGroup::CP_BattleButtonGroup() : 
		pFade(nullptr),
		pFrontFade(nullptr),
		closeFadeTime(1.0f),
		poseFadeAlpha(0.5f)
	{
		// �T�C�Y�ύX
		SetMaxButtonCnt(static_cast<u_int>(ButtonType::MaxNum));
	}

	void CP_BattleButtonGroup::OpenDisplay()
	{
		CP_ButtonGroup::OpenDisplay();

		// �t�F�[�h�ňÂ�����
		if (pFade)
			pFade->CloseFade(0.0f, poseFadeAlpha);
	}

	void CP_BattleButtonGroup::CloseDisplay()
	{
		CP_ButtonGroup::CloseDisplay();

		// �t�F�[�h�𓧖��ɂ���
		if (pFade)
			pFade->OpenFade(0.0f, 0.0f);
	}

	json CP_BattleButtonGroup::Save()
	{
		auto data = CP_ButtonGroup::Save();

		data["fadeObjName"] = fadeObjName;
		data["frontFadeName"] = frontFadeName;
		data["closeFadeTime"] = closeFadeTime;
		data["poseFadeAlpha"] = poseFadeAlpha;

		return data;
	}

	void CP_BattleButtonGroup::Load(const json& _data)
	{
		CP_ButtonGroup::Load(_data);

		LoadJsonString("fadeObjName", fadeObjName, _data);
		LoadJsonString("frontFadeName", frontFadeName, _data);
		LoadJsonFloat("closeFadeTime", closeFadeTime, _data);
		LoadJsonFloat("poseFadeAlpha", poseFadeAlpha, _data);
	}

	void CP_BattleButtonGroup::Start()
	{
		CP_ButtonGroup::Start();

		ButtonSetup();

		FindObject();

		// �|�[�Y�Ȃǂ��J���ꂽ�ꍇ�̂ݑ���\�ɂ���
		canInput = false;
	}

	void CP_BattleButtonGroup::FindObject()
	{
		SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();

		// �t�F�[�h
		GameObject* pObj = sceneObjs.GetSceneObject(fadeObjName);
		if (pObj)
		{
			pFade = pObj->GetComponent<CP_Fade>();
		}

		// ��O�t�F�[�h
		pObj = sceneObjs.GetSceneObject(frontFadeName);
		if (pObj)
		{
			pFrontFade = pObj->GetComponent<CP_Fade>();
		}
	}

	void CP_BattleButtonGroup::ButtonSetup()
	{
		// �֐���ݒ肵�Ă���
		// ���g���C
		CP_Button* pButton = 
			buttonParameterGroup[static_cast<u_int>(ButtonType::Retry)].pButton;
		if (pButton)
			pButton->SetEventFunction([&]()
				{
					OnPushRetry();
				});

		// �^�C�g��
		pButton = 
			buttonParameterGroup[static_cast<u_int>(ButtonType::Title)].pButton;
		if (pButton)
			pButton->SetEventFunction([&]()
				{
					OnPushTitle();
				});

		// �߂�
		pButton =
			buttonParameterGroup[static_cast<u_int>(ButtonType::Back)].pButton;
		if (pButton)
			pButton->SetEventFunction([]()
				{
					CP_BattleManager* pBattle = CP_BattleManager::GetInstance();
					if (pBattle)
						pBattle->EndPose();
				});
	}

	void CP_BattleButtonGroup::OnPushRetry()
	{
		// ��O�t�F�[�h���Ȃ��Ȃ�
		if (!pFrontFade)
		{
			SceneManager::GetInstance()->ChangeSceneRequest(BATTLE_SCENE_NAME);
			return;
		}
		// ���͕s��
		canInput = false;

		// ��O�̃t�F�[�h���Â������Ă���
		pFrontFade->GetGameObject().SetActive(true);

		// �t�F�[�h��h��Ԃ�
		pFrontFade->OpenFade(0.0f, 0.0f);
		pFrontFade->CloseFade(closeFadeTime, 1.0f);
		// ������x�o�g���V�[�������[�h
		pFrontFade->SetOnEndFunction([]()
			{
				SceneManager::GetInstance()->ChangeSceneRequest(BATTLE_SCENE_NAME);
			});
	}

	void CP_BattleButtonGroup::OnPushTitle()
	{
		// �t�F�[�h���Ȃ��Ȃ�
		if (!pFrontFade)
		{
			SceneManager::GetInstance()->ChangeSceneRequest(TITLE_SCENE_NAME);
			return;
		}
		// ���͕s��
		canInput = false;

		// ��O�̃t�F�[�h���Â������Ă���
		pFrontFade->GetGameObject().SetActive(true);

		// �t�F�[�h��h��Ԃ�
		pFrontFade->OpenFade(0.0f, 0.0f);
		pFrontFade->CloseFade(closeFadeTime, 1.0f);

		// �^�C�g���V�[�������[�h
		pFrontFade->SetOnEndFunction([]()
			{
				SceneManager::GetInstance()->ChangeSceneRequest(TITLE_SCENE_NAME);
			});
	}

	void CP_BattleButtonGroup::ImGuiDebug()
	{
#ifdef EDIT
		CP_ButtonGroup::ImGuiDebug();

		ImGui::DragFloat("CloseFadeTime", &closeFadeTime, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("PoseFadeAlpha", &poseFadeAlpha, 0.01f, 0.0f, 1.0f);

		static char input[IM_INPUT_BUF] = "\0";
		ImGui::PushID("Battle");
		ImGui::InputText("objName", input, IM_INPUT_BUF);
		ImGui::PopID();

		// �t�F�[�h
		if (ImGui::Button("Set Fade"))
			fadeObjName = input;
		ImGui::SameLine();
		ImGui::Text(fadeObjName.c_str());

		// ��O�̃t�F�[�h
		if (ImGui::Button("Set FrontFade"))
			frontFadeName = input;
		ImGui::SameLine();
		ImGui::Text(frontFadeName.c_str());
#endif // EDIT
	}
}
