#include "pch.h"
#include "CP_BattleButtonGroup.h"
#include "SceneManager.h"

namespace HashiTaku
{
	constexpr auto BATTLE_SCENE_NAME("BossBattle");	// ボスバトルシーン名
	constexpr auto TITLE_SCENE_NAME("Title");	// タイトルシーン名

	CP_BattleButtonGroup::CP_BattleButtonGroup() : 
		pFade(nullptr),
		pFrontFade(nullptr),
		closeFadeTime(1.0f),
		poseFadeAlpha(0.5f)
	{
		// サイズ変更
		SetMaxButtonCnt(static_cast<u_int>(ButtonType::MaxNum));
	}

	void CP_BattleButtonGroup::OpenDisplay()
	{
		CP_ButtonGroup::OpenDisplay();

		// フェードで暗くする
		if (pFade)
			pFade->CloseFade(0.0f, poseFadeAlpha);
	}

	void CP_BattleButtonGroup::CloseDisplay()
	{
		CP_ButtonGroup::CloseDisplay();

		// フェードを透明にする
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

		// ポーズなどが開かれた場合のみ操作可能にする
		canInput = false;
	}

	void CP_BattleButtonGroup::FindObject()
	{
		SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();

		// フェード
		GameObject* pObj = sceneObjs.GetSceneObject(fadeObjName);
		if (pObj)
		{
			pFade = pObj->GetComponent<CP_Fade>();
		}

		// 手前フェード
		pObj = sceneObjs.GetSceneObject(frontFadeName);
		if (pObj)
		{
			pFrontFade = pObj->GetComponent<CP_Fade>();
		}
	}

	void CP_BattleButtonGroup::ButtonSetup()
	{
		// 関数を設定していく
		// リトライ
		CP_Button* pButton = 
			buttonParameterGroup[static_cast<u_int>(ButtonType::Retry)].pButton;
		if (pButton)
			pButton->SetEventFunction([&]()
				{
					OnPushRetry();
				});

		// タイトル
		pButton = 
			buttonParameterGroup[static_cast<u_int>(ButtonType::Title)].pButton;
		if (pButton)
			pButton->SetEventFunction([&]()
				{
					OnPushTitle();
				});

		// 戻る
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
		// 手前フェードがないなら
		if (!pFrontFade)
		{
			SceneManager::GetInstance()->ChangeSceneRequest(BATTLE_SCENE_NAME);
			return;
		}
		// 入力不可に
		canInput = false;

		// 手前のフェードを暗くさせていく
		pFrontFade->GetGameObject().SetActive(true);

		// フェードを塗りつぶし
		pFrontFade->OpenFade(0.0f, 0.0f);
		pFrontFade->CloseFade(closeFadeTime, 1.0f);
		// もう一度バトルシーンをロード
		pFrontFade->SetOnEndFunction([]()
			{
				SceneManager::GetInstance()->ChangeSceneRequest(BATTLE_SCENE_NAME);
			});
	}

	void CP_BattleButtonGroup::OnPushTitle()
	{
		// フェードがないなら
		if (!pFrontFade)
		{
			SceneManager::GetInstance()->ChangeSceneRequest(TITLE_SCENE_NAME);
			return;
		}
		// 入力不可に
		canInput = false;

		// 手前のフェードを暗くさせていく
		pFrontFade->GetGameObject().SetActive(true);

		// フェードを塗りつぶし
		pFrontFade->OpenFade(0.0f, 0.0f);
		pFrontFade->CloseFade(closeFadeTime, 1.0f);

		// タイトルシーンをロード
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

		// フェード
		if (ImGui::Button("Set Fade"))
			fadeObjName = input;
		ImGui::SameLine();
		ImGui::Text(fadeObjName.c_str());

		// 手前のフェード
		if (ImGui::Button("Set FrontFade"))
			frontFadeName = input;
		ImGui::SameLine();
		ImGui::Text(frontFadeName.c_str());
#endif // EDIT
	}
}
