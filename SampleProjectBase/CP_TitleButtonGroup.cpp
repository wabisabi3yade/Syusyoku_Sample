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
		// サイズ変更
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

		// フェード開けに入力できるように
		canInput = false;
		pFade->SetOnEndFunction([&]()
			{
				canInput = true;
			});
	}

	void CP_TitleButtonGroup::OnPushStart()
	{
		// フェード後にシーンを移動
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
		// フェード後にシーンを移動
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

		// 関数を設定していく
		// スタート
		CP_Button* pButton = buttonGroup[static_cast<u_int>(ButtonType::Start)];
		if (pButton)
			pButton->SetEventFunction([&]()
				{
					OnPushStart();
				});

		// エンド
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