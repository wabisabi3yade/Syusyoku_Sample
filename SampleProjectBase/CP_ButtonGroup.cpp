#include "pch.h"
#include "CP_ButtonGroup.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	CP_ButtonGroup::CP_ButtonGroup() :
		pSelectBackImage(nullptr),
		curSelectButtonId(0),
		defaultSelectButtonId(0),
		maxButtonCnt(0),
		vertMoveSpeed(0),
		horiMoveSpeed(0),
		canInput(true)
	{
	}

	void CP_ButtonGroup::SetCanMove(bool _canMove)
	{
		canInput = _canMove;
	}

	json CP_ButtonGroup::Save()
	{
		auto data = Component::Save();

		data["defaultId"] = defaultSelectButtonId;
		data["vertSpeed"] = vertMoveSpeed;
		data["horiSpeed"] = horiMoveSpeed;
		data["buttonObjNames"] = buttonObjNames;
		data["backObjName"] = backImageObjName;

		auto& targetData = data["selectTargetPos"];
		for (int b_i = 0; b_i < maxButtonCnt; b_i++)
		{
			json posData;
			SaveJsonVector3("target", selectTargetPos[b_i], posData);
			targetData.push_back(posData);
		}

		return data;
	}

	void CP_ButtonGroup::Load(const json& _data)
	{
		Component::Load(_data);

		LoadJsonUnsigned("defaultId", defaultSelectButtonId, _data);
		LoadJsonInteger("vertSpeed", vertMoveSpeed, _data);
		LoadJsonInteger("horiSpeed", horiMoveSpeed, _data);
		LoadJsonString("backObjName", backImageObjName, _data);

		if (IsJsonContains(_data, "buttonObjNames"))
		{
			const json& namesData = _data["buttonObjNames"];
			int idx = 0;
			for (auto& nameData : namesData)
			{
				if (idx >= maxButtonCnt) break;
				buttonObjNames[idx] = nameData.get<std::string>();
				idx++;
			}
		}

		// 座標
		json targetData;
		if (LoadJsonDataArray("selectTargetPos", targetData, _data))
		{
			int i = 0;
			for (auto& posData : targetData)
			{
				DXSimp::Vector3 pos;
				LoadJsonVector3("target", pos, posData);
				selectTargetPos[i] = pos;
				i++;
			}
		}
	}

	void CP_ButtonGroup::Start()
	{
		if (static_cast<u_int>(buttonGroup.size()) < defaultSelectButtonId)
		{
			HASHI_DEBUG_LOG("デフォルトのIDがボタンの数を超えています");
			defaultSelectButtonId = 0;
		}
		if (static_cast<u_int>(buttonGroup.size()) == 0)
		{
			HASHI_DEBUG_LOG("ボタンが1つもありません");
		}

		// デフォルトから始める
		curSelectButtonId = defaultSelectButtonId;


		// 各コンポーネントを取得
		SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();
		GameObject* pGO = sceneObjs.GetSceneObject(backImageObjName);
		if (pGO)
		{
			pSelectBackImage = pGO->GetComponent<CP_UIRenderer>();
		}

		for (int b_i = 0; b_i < maxButtonCnt; b_i++)
		{
			// 設定したボタンの名前からボタンコンポーネントを取得
			pGO = sceneObjs.GetSceneObject(buttonObjNames[b_i]);
			if (!pGO) continue;
			CP_Button* pGetButton = pGO->GetComponent<CP_Button>();
			buttonGroup[b_i] = pGetButton;

			if (!pGetButton)	// ボタン取得出来なかったら
				HASHI_DEBUG_LOG(buttonObjNames[b_i] + "からボタンが取得できませんでした");
		}
	}

	void CP_ButtonGroup::Update()
	{
		if (maxButtonCnt == 0) return;	// ボタンが1つも設定されていないなら
		if (!canInput) return;	// 操作できないなら抜ける

		MoveButton();
		DecideButton();
	}

	void CP_ButtonGroup::MoveButton()
	{
		const InputClass& gameInput = MainApplication::GetInput();
		u_int prevSelectId = curSelectButtonId;	// 変更前のボタンのId

		int inputX = 0, inputY = 0;

		// 入力によってボタンの移動を行う
		if (gameInput.GetInputTriggerDir(InputClass::InputDirection::Up))
		{
			inputY += 1;
		}
		if (gameInput.GetInputTriggerDir(InputClass::InputDirection::Down))
		{
			inputY -= 1;
		}
		if (gameInput.GetInputTriggerDir(InputClass::InputDirection::Right))
		{
			inputX += 1;
		}
		if (gameInput.GetInputTriggerDir(InputClass::InputDirection::Left))
		{
			inputX -= 1;
		}

		// 選択中のボタンを移動
		curSelectButtonId += inputX * horiMoveSpeed + inputY * vertMoveSpeed;
		// 前と変わっていないなら処理しない
		if (curSelectButtonId == prevSelectId) return;

		// 最大数を超えたら
		if (curSelectButtonId >= maxButtonCnt)
		{
			curSelectButtonId = 0;
		}
		else if (curSelectButtonId < 0) // 0下回ったら
		{
			curSelectButtonId = maxButtonCnt - 1;
		}

		MoveSelectBackImage();	// 選択の背景イメージを移動させる
	}

	void CP_ButtonGroup::DecideButton()
	{
		const InputClass& gameInput = MainApplication::GetInput();
		const GamePad& gamePad = gameInput.GetGamePad();

		bool isDecide = false;
		if (gamePad.GetConnecting()) // コントローラ
		{
			isDecide = gamePad.InputTrigger(GamePad::PadFlag::Batsu);
		}
		else // キーボード
		{

			isDecide = gameInput.GetKeyboard().GetKeyDown(DIK_RETURN);
		}

		// 決定したらそのボタンのイベントを発生させる
		if (isDecide && buttonGroup[curSelectButtonId])
		{
			buttonGroup[curSelectButtonId]->OnEvent();
		}
	}

	void CP_ButtonGroup::MoveSelectBackImage()
	{
		if (!pSelectBackImage)
		{
			HASHI_DEBUG_LOG("選択イメージが設定されていません");
			return;
		}

		// 選択されているボタンの座標を取得
		Transform& backImageTrans = pSelectBackImage->GetTransform();
		DXSimp::Vector3 buttonPos = selectTargetPos[curSelectButtonId];

		// Z座標以外を反映させる
		buttonPos.z = backImageTrans.GetPosition().z;
		backImageTrans.SetPosition(buttonPos);
	}

	void CP_ButtonGroup::SetMaxButtonCnt(int _max)
	{
		maxButtonCnt = _max;
		buttonGroup.resize(maxButtonCnt);
		buttonObjNames.resize(maxButtonCnt);
		selectTargetPos.resize(maxButtonCnt);
	}

	void CP_ButtonGroup::ImGuiDebug()
	{
#ifdef EDIT
		ImGuiMethod::Text("CanInput", canInput);
		ImGui::DragInt("VertSpeed", &vertMoveSpeed);
		ImGui::DragInt("HoriSpeed", &horiMoveSpeed);

		// オブジェクト名セット
		static char inputText[IM_INPUT_BUF];
		ImGui::InputText("objName", inputText, IM_INPUT_BUF);
		ImGui::Text("Back:%s", backImageObjName.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Set"))
			backImageObjName = inputText;

		for (int b_i = 0; b_i < maxButtonCnt; b_i++)
		{
			ImGui::PushID(b_i);

			ImGui::Text("%d:%s", b_i, buttonObjNames[b_i].c_str());
			ImGui::SameLine();
			if (ImGui::Button("Set"))
				buttonObjNames[b_i] = inputText;

			ImGui::DragFloat3("Pos", &selectTargetPos[b_i].x);

			ImGui::PopID();
		}
#endif // EDIT
	}
}