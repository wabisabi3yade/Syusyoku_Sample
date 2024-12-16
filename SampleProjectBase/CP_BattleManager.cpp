#include "pch.h"
#include "CP_BattleManager.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"
#include "Geometory.h"
#include "CP_CameraMove.h"
#include "SceneManager.h"

namespace HashiTaku
{
	// GameStart�֘A
	constexpr float START_FADE_TIME(0.6f);	// �t�F�[�h����
	constexpr float START_FADE_ALPHA(0.3f);	// �t�F�[�h�̈�U�~�߂郿�l
	constexpr float START_MOVE_TIME(2.4f);	// �����̈ړ�����
	constexpr float START_START_POS_X(1600.0f);	// �����̊J�n�ڕW����
	constexpr float START_TARGET_POS_X(-400.0f);	// �����̈ړ���ڕW����

	// ���������o
	constexpr float WIN_FADE_INTERVAL(3.0f);	// �t�F�[�h�܂ł̎���
	constexpr float WIN_FADE_ALPHA(0.5f);	// �t�F�[�h�̈�U�~�߂郿�l
	constexpr float WIN_FADE_TIME(2.0f);	// �����ƃt�F�[�h�̎���
	constexpr float WIN_DISPLAY_TIME(4.0f);	// �����ƃt�F�[�h�̎���
	constexpr float WIN_CLOSE_FADE_TIME(1.3f);	// ����t�F�[�h����

	void CP_BattleManager::SetPlayer(CP_Player& _playerObj)
	{
		pPlayer = &_playerObj;
	}

	void CP_BattleManager::AddEnemy(CP_Enemy& _enemyObj)
	{
		auto itr = std::find(enemyList.begin(), enemyList.end(), &_enemyObj);

		if (itr != enemyList.end()) return;

		enemyList.push_back(&_enemyObj);
	}

	void CP_BattleManager::SetCameraMove(CP_CameraMove& _camMove)
	{
		pCamMove = &_camMove;
	}

	void CP_BattleManager::RemovePlayer(CP_Player& _playerObj)
	{
		if (pPlayer != &_playerObj) return;

		pPlayer = nullptr;
	}

	void CP_BattleManager::RemoveEnemy(CP_Enemy& _enemyObj)
	{
		enemyList.remove(&_enemyObj);
	}

	void CP_BattleManager::RemoveCamMove(CP_CameraMove& _camMove)
	{
		if (pCamMove != &_camMove) return;
		pCamMove = nullptr;
	}


	CP_Player* CP_BattleManager::GetPlayerObject()
	{
		return pPlayer;
	}

	const CP_BattleManager::EnemyList& CP_BattleManager::GetEnemyList()
	{
		return enemyList;
	}

	CP_BattleManager::BattleState CP_BattleManager::GetCurState() const
	{
		return curBattleState;
	}

	void CP_BattleManager::OnPlayerWin()
	{
		// �قړ��^�C�~���O�ŋN�����ꍇ�ɏd�����Ȃ��悤�ɂ���
		if (curBattleState == BattleState::Lose) return;
		curBattleState = BattleState::Win;

		// �I�u�W�F�N�g���~�߂�
		StopObjects();
		animationElapsedTime = 0.0f;

		// �J�������o���n�߂�
		if (!pCamMove) return;	// �J�������Ȃ��Ȃ�
		if (static_cast<u_int>(enemyList.size()) == 0) return;	// �G�����Ȃ��Ȃ�

		Transform& bossTransform = (*enemyList.begin())->GetTransform();
		pCamMove->OnPlayerWin(bossTransform);
	}

	void CP_BattleManager::OnPlayerLose()
	{
		// �قړ��^�C�~���O�ŋN�����ꍇ�ɏd�����Ȃ��悤�ɂ���
		if (curBattleState == BattleState::Win) return;

		curBattleState = BattleState::Lose;

		// �I�u�W�F�N�g���~�߂�
		StopObjects();
		animationElapsedTime = 0.0f;

		// �J�������o���n�߂�
		if (!pCamMove) return;	// �J�������Ȃ��Ȃ�
		if (!pPlayer) return;	// �G�����Ȃ��Ȃ�

		Transform& playerTransform = pPlayer->GetTransform();
		pCamMove->OnPlayerWin(playerTransform);
	}

	nlohmann::json CP_BattleManager::Save()
	{
		auto data = SingletonComponent::Save();

		SaveJsonVector4("moveAreaRect", moveAreaRect, data);

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

	void CP_BattleManager::Load(const nlohmann::json& _data)
	{
		SingletonComponent::Load(_data);

		LoadJsonVector4("moveAreaRect", moveAreaRect, _data);

		LoadJsonString("fade", fadeObjName, _data);
		LoadJsonString("start", startObjName, _data);
		LoadJsonString("win", winObjName, _data);
		LoadJsonString("gameOver", gameOverObjName, _data);

		nlohmann::json loadData;
		if (LoadJsonData("gameStartCurve", loadData, _data))
		{
			gameStartCurve.Load(loadData);
		}

#ifdef EDIT
		LoadJsonBoolean("uiAnim", isUIAnimation, _data);
#endif // EDIT
	}

	void CP_BattleManager::Start()
	{
		FindObject();

		//FadeStart();
	}

	void CP_BattleManager::Update()
	{
		switch (curBattleState)
		{
		case CP_BattleManager::BattleState::Start:
			StartUpdate();
			break;
		case CP_BattleManager::BattleState::Win:
			WinUpdate();
			break;
		case CP_BattleManager::BattleState::Lose:
			LoseUpdate();
			break;
		default:
			break;
		}
	}

	void CP_BattleManager::LateUpdate()
	{
		// �ړ��͈͐���
		MoveAreaUpdate();
	}

	void CP_BattleManager::Draw()
	{
#ifdef EDIT
		if (!isDebugDisplay) return;

		constexpr float height = 0.3f;
		DXSimp::Color c = DXSimp::Color(1, 0, 0);

		DXSimp::Vector3 RT = { moveAreaRect.x, height, moveAreaRect.z };
		DXSimp::Vector3 RB = { moveAreaRect.x, height, moveAreaRect.w };
		DXSimp::Vector3 LT = { moveAreaRect.y, height, moveAreaRect.z };
		DXSimp::Vector3 LB = { moveAreaRect.y, height, moveAreaRect.w };

		Geometory::AddLine(RT, RB, c);
		Geometory::AddLine(RB, LB, c);
		Geometory::AddLine(LB, LT, c);
		Geometory::AddLine(LT, RT, c);
#endif // EDIT
	}

	void CP_BattleManager::FindObject()
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

	void CP_BattleManager::FadeStart()
	{
#ifdef EDIT
		// ���o�Ȃ�
		if (!isUIAnimation)
		{
			// �o�g���Ɉڍs����
			curBattleState = BattleState::Battle;
			return;
		}
#endif // EDIT

		// �t�F�[�h���Ȃ��Ȃ�
		if (!pFade)
		{
			// �o�g���Ɉڍs����
			curBattleState = BattleState::Battle;
			return;
		}

		// �t�F�[�h�J������X�^�[�g���o�ɓ���
		pFade->OpenFade(START_FADE_TIME, START_FADE_ALPHA);
		pFade->SetOnEndFunction([&]()
			{
				OnBeginStart();
			});

		// �I�u�W�F�N�g�𓮂��Ȃ��悤�ɂ���
		StopObjects();

		// �J�������������Ȃ�
		if (pCamMove)
			pCamMove->SetEnable(false);
	}

	void CP_BattleManager::OnBeginStart()
	{
		curBattleState = BattleState::Start;
		animationElapsedTime = 0.0f;

		if (pGameStartObj)
		{
			Transform& trans = pGameStartObj->GetTransform();
			DXSimp::Vector3 startPos = trans.GetPosition();
			startPos.x = START_START_POS_X;
			pGameStartObj->GetTransform().SetPosition(startPos);
		}
		else
			OnBeginBattle();
	}

	void CP_BattleManager::StartUpdate()
	{
		animationElapsedTime += DeltaTime();

		float curPosX = Mathf::Lerp(
			START_START_POS_X,
			START_TARGET_POS_X,
			gameStartCurve.GetValue(animationElapsedTime / START_MOVE_TIME));

		Transform& trans = pGameStartObj->GetTransform();
		DXSimp::Vector3 startPos = trans.GetPosition();
		startPos.x = curPosX;
		pGameStartObj->GetTransform().SetPosition(startPos);

		if (animationElapsedTime > START_MOVE_TIME)
		{
			OnBeginBattle();
			pFade->OpenFade(0.0f);
		}
	}

	void CP_BattleManager::OnBeginBattle()
	{
		curBattleState = BattleState::Battle;

		// ������Ԃɂ���
		ActiveObjects();
	}

	void CP_BattleManager::WinUpdate()
	{
		if (!pWinObj) return;
		if (!pFade) return;

		switch (step)
		{
		case 0:
			animationElapsedTime += DeltaTime();

			if (animationElapsedTime > WIN_FADE_INTERVAL)
			{
				animationElapsedTime = 0.0f;
				pWinObj->CloseFade(WIN_FADE_TIME, 1.0f);
				pFade->CloseFade(WIN_FADE_TIME, WIN_FADE_ALPHA);
				step = 1;
			}

			break;

		case 1:
			animationElapsedTime += DeltaTime();

			if (animationElapsedTime > WIN_FADE_TIME)
			{
				animationElapsedTime = 0.0f;
				step = 2;
			}

			break;

		case 2:
			animationElapsedTime += DeltaTime();

			if (animationElapsedTime > WIN_DISPLAY_TIME)
			{
				animationElapsedTime = 0.0f;
				step = 3;
				pWinObj->OpenFade(WIN_CLOSE_FADE_TIME);
				pFade->CloseFade(WIN_CLOSE_FADE_TIME);
			}

			break;

		case 3:
			animationElapsedTime += DeltaTime();


			pFade->SetOnEndFunction([&]()
				{
					SceneManager::GetInstance()->ChangeSceneRequest("Title");
				});

			break;
		}
	}

	void CP_BattleManager::LoseUpdate()
	{
		if (!pGameOverObj) return;
		if (!pFade) return;

		switch (step)
		{
		case 0:
			animationElapsedTime += DeltaTime();

			if (animationElapsedTime > WIN_FADE_INTERVAL)
			{
				animationElapsedTime = 0.0f;
				pGameOverObj->CloseFade(WIN_FADE_TIME, 1.0f);
				pFade->CloseFade(WIN_FADE_TIME, WIN_FADE_ALPHA);
				step = 1;
			}

			break;

		case 1:
			animationElapsedTime += DeltaTime();

			if (animationElapsedTime > WIN_FADE_TIME)
			{
				animationElapsedTime = 0.0f;
				step = 2;
			}

			break;

		case 2:
			animationElapsedTime += DeltaTime();

			if (animationElapsedTime > WIN_DISPLAY_TIME)
			{
				animationElapsedTime = 0.0f;
				step = 3;
				pGameOverObj->OpenFade(WIN_CLOSE_FADE_TIME);
				pFade->CloseFade(WIN_CLOSE_FADE_TIME);
			}

			break;

		case 3:
			animationElapsedTime += DeltaTime();


			pFade->SetOnEndFunction([&]()
				{
					SceneManager::GetInstance()->ChangeSceneRequest("Title");
				});

			break;
		}
	}

	void CP_BattleManager::MoveAreaUpdate()
	{
		// �ړ��͈͓��ɐ���

		if (pPlayer)
			PositionClamp(pPlayer->GetTransform());

		for (auto& enemy : enemyList)
		{
			PositionClamp(enemy->GetTransform());
		}
	}

	void CP_BattleManager::PositionClamp(Transform& _charaTransform)
	{
		//�@y���W���l�����Ȃ����������߂�
		DXSimp::Vector3 pos = _charaTransform.GetPosition();

		pos.x = std::clamp(pos.x, moveAreaRect.y, moveAreaRect.x);
		pos.z = std::clamp(pos.z, moveAreaRect.w, moveAreaRect.z);

		_charaTransform.SetPosition(pos);
	}

	void CP_BattleManager::ActiveObjects()
	{
		if (pPlayer)
			pPlayer->SetEnable(true);

		for (auto& enemy : enemyList)
			enemy->SetEnable(true);

		if (pCamMove)
			pCamMove->SetEnable(true);
	}

	void CP_BattleManager::StopObjects()
	{
		if (pPlayer)
			pPlayer->SetEnable(false);

		for (auto& enemy : enemyList)
			enemy->SetEnable(false);
	}

	void CP_BattleManager::ImGuiDebug()
	{
#ifdef EDIT

		ImGui::Checkbox("IsDisplay", &isDebugDisplay);
		ImGui::DragFloat4("AreaRect", &moveAreaRect.x, 0.01f);

		ImGuiMethod::LineSpaceSmall();

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

		ImGuiStart();
#endif // EDIT
	}

	void CP_BattleManager::ImGuiStart()
	{
		if (!ImGuiMethod::TreeNode("Start")) return;

		gameStartCurve.ImGuiCall();

		ImGui::TreePop();
	}
}