#include "pch.h"
#include "BattleDirection.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	// GameStart関連
	constexpr float START_FADE_TIME(0.6f);	// フェード時間
	constexpr float START_FADE_ALPHA(0.3f);	// フェードの一旦止めるα値
	constexpr float START_MOVE_TIME(2.4f);	// 文字の移動時間
	constexpr float START_START_POS_X(1600.0f);	// 文字の開始目標時間
	constexpr float START_TARGET_POS_X(-400.0f);	// 文字の移動先目標時間

	// 勝利時演出
	constexpr float WIN_FADE_INTERVAL(3.0f);	// フェードまでの時間
	constexpr float WIN_FADE_ALPHA(0.5f);	// フェードの一旦止めるα値
	constexpr float WIN_FADE_TIME(2.0f);	// 文字とフェードの時間
	constexpr float WIN_DISPLAY_TIME(4.0f);	// 文字とフェードの時間
	constexpr float WIN_CLOSE_FADE_TIME(1.3f);	// 閉じるフェード時間

	BattleDirection::BattleDirection() : 
		pFade(nullptr),
		pGameStartObj(nullptr),
		pGameOverObj(nullptr),
		pWinObj(nullptr),
		pCamMove(nullptr),
		curState(DirectionState::Wait),
		animationElapsedTime(0.0f),
		directionStep(0),
		isUIAnimation(true)
	{
	}

	void BattleDirection::Init(CP_CameraMove* _pCamMove)
	{
		pCamMove = _pCamMove;

		// オブジェクトを探す
		FindObject();
	}

	void BattleDirection::Update(float _deltaTime)
	{
		switch (curState)
		{
		case DirectionState::Start:
			StartUpdate(_deltaTime);
			break;
		case DirectionState::Win:
			WinUpdate(_deltaTime);
			break;
		case DirectionState::Lose:
			LoseUpdate(_deltaTime);
			break;
		default:
			break;
		}
	}

	void BattleDirection::OnBeginStartDirection()
	{
#ifdef EDIT
		// 演出なし
		if (!isUIAnimation)
		{
			curState = DirectionState::Start;
			// 開始演出が終了したことを伝える
			OnEndDirection();
			return;
		}
#endif // EDIT

		// フェードがないなら
		if (!pFade)
		{
			curState = DirectionState::Start;
			// 開始演出が終了したことを伝える
			OnEndDirection();
			return;
		}

		// フェード開けたらスタート演出に入る
		pFade->OpenFade(START_FADE_TIME, START_FADE_ALPHA);
		pFade->SetOnEndFunction([&]()
			{
				OnBeginStart();
			});
	}

	void BattleDirection::OnBeginWinDirection(const Transform* _targetTransform)
	{
#ifdef EDIT
		if (!isUIAnimation)
			return;
#endif // EDIT

		curState = DirectionState::Win;

		// ほぼ同タイミングで起きた場合に重複しないようにする
		if (!_targetTransform || curState == DirectionState::Lose)
		{
			OnEndDirection();
			return;
		}
		// パラメータをリセット
		animationElapsedTime = 0.0f;
		directionStep = 0;

		// カメラ演出を始める
		if (!pCamMove) return;	// カメラがないなら
		// カメラステートを切り替えるs
		pCamMove->OnPlayerWin(*_targetTransform);
	}

	void BattleDirection::OnBeginLoseDirection(const Transform* _targetTransform)
	{
#ifdef EDIT
		if (!isUIAnimation)	return;
#endif // EDIT

		// ほぼ同タイミングで起きた場合に重複しないようにする
		if (!_targetTransform || curState == DirectionState::Win)
		{
			OnEndDirection();
			return;
		}

		curState = DirectionState::Lose;

		animationElapsedTime = 0.0f;

		// カメラ演出を始める
		if (!pCamMove) return;	// カメラがないなら
		pCamMove->OnPlayerWin(*_targetTransform);
	}

	json BattleDirection::Save()
	{
		json data;

		data["fade"] = fadeObjName;
		data["start"] = startObjName;
		data["win"] = winObjName;
		data["gameOver"] = gameOverObjName;
		data["gameStartCurve"] = gameStartCurve.Save();
#ifdef EDIT
		data["uiAnim"] = isUIAnimation;
#endif // EDIT
		return data;
	}

	void BattleDirection::Load(const json& _data)
	{
		LoadJsonString("fade", fadeObjName, _data);
		LoadJsonString("start", startObjName, _data);
		LoadJsonString("win", winObjName, _data);
		LoadJsonString("gameOver", gameOverObjName, _data);

		json loadData;
		if (LoadJsonData("gameStartCurve", loadData, _data))
		{
			gameStartCurve.Load(loadData);
		}
#ifdef EDIT
		LoadJsonBoolean("uiAnim", isUIAnimation, _data);
#endif // EDIT
	}

	void BattleDirection::FindObject()
	{
		SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();

		GameObject* pGo = sceneObjs.GetSceneObject(fadeObjName);
		if (pGo)
		{
			pFade = pGo->GetComponent<CP_Fade>();
		}
		pGo = sceneObjs.GetSceneObject(winObjName);
		if (pGo)
		{
			pWinObj = pGo->GetComponent<CP_Fade>();
		}
		pGo = sceneObjs.GetSceneObject(gameOverObjName);
		if (pGo)
		{
			pGameOverObj = pGo->GetComponent<CP_Fade>();
		}

		pGameStartObj = sceneObjs.GetSceneObject(startObjName);
	}

	void BattleDirection::OnBeginStart()
	{
		curState = DirectionState::Start;

		// リセット
		animationElapsedTime = 0.0f;

		if (pGameStartObj)
		{
			// 文字を初期の位置に配置
			Transform& trans = pGameStartObj->GetTransform();
			DXSimp::Vector3 startPos = trans.GetPosition();
			startPos.x = START_START_POS_X;
			pGameStartObj->GetTransform().SetPosition(startPos);
		}
		else
			OnEndDirection();
	}

	void BattleDirection::StartUpdate(float _deltaTime)
	{
		animationElapsedTime += _deltaTime;

		float curPosX = Mathf::Lerp(
			START_START_POS_X,
			START_TARGET_POS_X,
			gameStartCurve.GetValue(animationElapsedTime / START_MOVE_TIME));

		// もじを移動
		Transform& trans = pGameStartObj->GetTransform();
		DXSimp::Vector3 startPos = trans.GetPosition();
		startPos.x = curPosX;
		pGameStartObj->GetTransform().SetPosition(startPos);

		// 時間が過ぎたら終了
		if (animationElapsedTime > START_MOVE_TIME)
		{
			pFade->OpenFade(0.0f);
			OnEndDirection();
		}
	}

	void BattleDirection::WinUpdate(float _deltaTime)
	{
		animationElapsedTime += _deltaTime;
		switch (directionStep)
		{
		case 0:
			if (animationElapsedTime > WIN_FADE_INTERVAL)
			{
				animationElapsedTime = 0.0f;
				pWinObj->CloseFade(WIN_FADE_TIME, 1.0f);
				pFade->CloseFade(WIN_FADE_TIME, WIN_FADE_ALPHA);
				directionStep = 1;
			}

			break;

		case 1:
			if (animationElapsedTime > WIN_FADE_TIME)
			{
				animationElapsedTime = 0.0f;
				directionStep = 2;
			}

			break;

		case 2:
			if (animationElapsedTime > WIN_DISPLAY_TIME)
			{
				animationElapsedTime = 0.0f;
				directionStep = 3;
				pWinObj->OpenFade(WIN_CLOSE_FADE_TIME);
				pFade->CloseFade(WIN_CLOSE_FADE_TIME);
			}

			break;

		case 3:
			// フェードが閉まり切ったら
			pFade->SetOnEndFunction([&]()
				{
					OnEndDirection();
				});

			break;
		}
	}

	void BattleDirection::LoseUpdate(float _deltaTime)
	{
		animationElapsedTime += _deltaTime;

		switch (directionStep)
		{
		case 0:
			if (animationElapsedTime > WIN_FADE_INTERVAL)
			{
				animationElapsedTime = 0.0f;
				pGameOverObj->CloseFade(WIN_FADE_TIME, 1.0f);
				pFade->CloseFade(WIN_FADE_TIME, WIN_FADE_ALPHA);
				directionStep = 1;
			}

			break;

		case 1:
			if (animationElapsedTime > WIN_FADE_TIME)
			{
				animationElapsedTime = 0.0f;
				directionStep = 2;
			}

			break;

		case 2:
			if (animationElapsedTime > WIN_DISPLAY_TIME)
			{
				animationElapsedTime = 0.0f;
				directionStep = 3;
				pGameOverObj->OpenFade(WIN_CLOSE_FADE_TIME);
				pFade->CloseFade(WIN_CLOSE_FADE_TIME);
			}

			break;

		case 3:
			// フェードが閉まり切ったら
			pFade->SetOnEndFunction([&]()
				{
					OnEndDirection();
				});

			break;
		}
	}

	void BattleDirection::OnEndDirection()
	{
		// 終了したことを通知
		NotifyAll(static_cast<int>(curState));

		// 待機に戻す
		curState = DirectionState::Wait;
	}

	void BattleDirection::ImGuiDebug()
	{
		gameStartCurve.ImGuiCall();

		static char input[IM_INPUT_BUF] = "\0";
		ImGui::InputText("ObjName", input, IM_INPUT_BUF);

		if (ImGui::Button("Set Fade")) fadeObjName = input;
		ImGui::SameLine();
		ImGui::Text(fadeObjName.c_str());

		if (ImGui::Button("Set Start")) startObjName = input;
		ImGui::SameLine();
		ImGui::Text(startObjName.c_str());

		if (ImGui::Button("Set Win")) winObjName = input;
		ImGui::SameLine();
		ImGui::Text(winObjName.c_str());

		if (ImGui::Button("Set GameOver")) gameOverObjName = input;
		ImGui::SameLine();
		ImGui::Text(gameOverObjName.c_str());

		ImGui::Checkbox("UIAnimation", &isUIAnimation);

	}
}
