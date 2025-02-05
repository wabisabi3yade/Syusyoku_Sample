#include "pch.h"
#include "CP_TitleButtonGroup.h"
#include "InSceneSystemManager.h"
#include "SceneManager.h"
#include "CP_Fade.h"

namespace HashiTaku
{
	constexpr auto BATTLE_SCENE_NAME("BossBattle");

	CP_TitleButtonGroup::CP_TitleButtonGroup() :
		pFade(nullptr),
		pCheckImage(nullptr)
	{
		// サイズ変更
		SetMaxButtonCnt(static_cast<u_int>(ButtonType::MaxNum));
	}

	json CP_TitleButtonGroup::Save()
	{
		auto data = CP_ButtonGroup::Save();
		data["fadeObj"] = fadeObjName;
		data["checkImageObj"] = checkImageObjName;

		return data;
	}

	void CP_TitleButtonGroup::Load(const json& _data)
	{
		CP_ButtonGroup::Load(_data);
		LoadJsonString("fadeObj", fadeObjName, _data);
		LoadJsonString("checkImageObj",checkImageObjName, _data);
	}

	void CP_TitleButtonGroup::Start()
	{
		CP_ButtonGroup::Start();

		ButtonSetup();

		FindObject();
	}

	void CP_TitleButtonGroup::FindObject()
	{
		SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();

		// フェード
		GameObject* pObj = sceneObjs.GetSceneObject(fadeObjName);
		if (pObj)
		{
			pFade = pObj->GetComponent<CP_Fade>();
		}
		
		// チェックマーク
		pObj = sceneObjs.GetSceneObject(checkImageObjName);
		if (pObj)
		{
			pCheckImage = pObj->GetComponent<CP_UIRenderer>();
			if (pCheckImage)
			{
				// 無敵フラグにチェックマークを合わせる
				bool isInvincible = CP_Player::GetIsDebugInvincible();
				pCheckImage->SetEnable(isInvincible);
			}
		}
	}

	void CP_TitleButtonGroup::OnPushStart()
	{
		// フェード後にシーンを移動
		if (pFade)
		{
			pFade->CloseFade();

			pFade->SetOnEndFunction([&]()
				{
					SceneManager::GetInstance()->ChangeSceneRequest(BATTLE_SCENE_NAME);
				});
		}
		else
		{
			SceneManager::GetInstance()->ChangeSceneRequest(BATTLE_SCENE_NAME);
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

	void CP_TitleButtonGroup::OnPushCheck()
	{
		// 無敵フラグを切り替える
		bool isDebugInvincible = CP_Player::GetIsDebugInvincible();
		isDebugInvincible = !isDebugInvincible;

		CP_Player::SetIsDebugInvincible(isDebugInvincible);

		if (pCheckImage)
			pCheckImage->SetEnable(isDebugInvincible);
	}

	void CP_TitleButtonGroup::ButtonSetup()
	{
		// 関数を設定していく
		// スタート
		CP_Button* pButton = 
			buttonParameterGroup[static_cast<u_int>(ButtonType::Start)].pButton;
		if (pButton)
			pButton->SetEventFunction([&]()
				{
					OnPushStart();
				});

		// エンド
		pButton = 
			buttonParameterGroup[static_cast<u_int>(ButtonType::End)].pButton;
		if (pButton)
			pButton->SetEventFunction([&]()
				{
					OnPushEnd();
				});

		// チェックマーク
		pButton = 
			buttonParameterGroup[static_cast<u_int>(ButtonType::Check)].pButton;
		if (pButton)
			pButton->SetEventFunction([&]()
				{
					OnPushCheck();
				});
	}

	void CP_TitleButtonGroup::ImGuiDebug()
	{
#ifdef EDIT
		CP_ButtonGroup::ImGuiDebug();

		static char input[IM_INPUT_BUF] = "\0";
		ImGui::PushID("Title");
		ImGui::InputText("ObjName", input, IM_INPUT_BUF);
		ImGui::PopID();

		if (ImGui::Button("Set Fade"))
			fadeObjName = input;
		ImGui::SameLine();
		ImGui::Text(fadeObjName.c_str());

		if (ImGui::Button("Set CheckImage"))
			checkImageObjName = input;
		ImGui::SameLine();
		ImGui::Text(checkImageObjName.c_str());

#endif // EDIT
	}
}