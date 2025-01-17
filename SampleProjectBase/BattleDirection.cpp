#include "pch.h"
#include "BattleDirection.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	// GameStart�֘A
	constexpr float START_FIRST_FADE_TIME(0.6f);	// �t�F�[�h����(�����J����)
	constexpr float START_FIRST_FADE_ALPHA(0.3f);	// �t�F�[�h�̈�U�~�߂郿�l(�����J����)
	constexpr float START_MOVE_TIME(2.4f);	// �����̈ړ�����
	constexpr float START_START_POS_X(1600.0f);	// �����̊J�n�ڕW����
	constexpr float START_TARGET_POS_X(-400.0f);	// �����̈ړ���ڕW����
	constexpr float START_FADE_POS_X(-400.0f);	// �����̈ړ���ڕW����
	constexpr float START_SECOND_FADE_TIME(0.1f);	// �t�F�[�h���J���鎞��(���S�ɊJ����)

	// ���������o
	constexpr float WIN_FADE_INTERVAL(7.0f);	// �t�F�[�h�܂ł̎���
	constexpr float WIN_FADE_ALPHA(0.5f);	// �t�F�[�h�̈�U�~�߂郿�l
	constexpr float WIN_FADE_TIME(2.0f);	// �����ƃt�F�[�h�̎���
	constexpr float WIN_DISPLAY_TIME(4.0f);	// �����ƃt�F�[�h�̎���
	constexpr float WIN_CLOSE_FADE_TIME(1.3f);	// ����t�F�[�h����

	BattleDirection::BattleDirection() :
		pFade(nullptr),
		pGameStartObj(nullptr),
		pGameOverObj(nullptr),
		pWinObj(nullptr),
		pCamMove(nullptr),
		curState(DirectionState::Wait),
		animationElapsedTime(0.0f),
		winTimeScale(0.2f),
		directionStep(0),
		isUIAnimation(true),
		isStartFading(false)
	{
	}

	void BattleDirection::Init(CP_CameraMove* _pCamMove)
	{
		pCamMove = _pCamMove;

		// �I�u�W�F�N�g��T��
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
		curState = DirectionState::Start;

#ifdef EDIT
		// ���o�Ȃ�
		if (!isUIAnimation)
		{
			// �J�n���o���I���������Ƃ�`����
			OnEndDirection();
			return;
		}
#endif // EDIT

		// �t�F�[�h���Ȃ��Ȃ�
		if (!pFade)
		{
			// �J�n���o���I���������Ƃ�`����
			OnEndDirection();
			return;
		}

		isStartFading = true;

		// �t�F�[�h�J������X�^�[�g���o�ɓ���
		pFade->CloseFade(0.0f, 1.0f);	// �h��Ԃ�����J�n
		pFade->OpenFade(START_FIRST_FADE_TIME, START_FIRST_FADE_ALPHA);
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

		// �قړ��^�C�~���O�ŋN�����ꍇ�ɏd�����Ȃ��悤�ɂ���
		if (!_targetTransform || curState == DirectionState::Lose || !pWinObj)
		{
			OnEndDirection();
			return;
		}
		// �p�����[�^�����Z�b�g
		animationElapsedTime = 0.0f;
		directionStep = 0;

		// �^�C���X�P�[����x������
		InSceneSystemManager::GetInstance()->SetTimeScale(winTimeScale);

		// �������A�N�e�B�u��Ԃ�
		pWinObj->GetGameObject().SetActive(true);

		// �J�������o���n�߂�
		if (!pCamMove) return;	// �J�������Ȃ��Ȃ�
		// �J�����X�e�[�g��؂�ւ���s
		pCamMove->OnPlayerWin(*_targetTransform);
	}

	void BattleDirection::OnBeginLoseDirection(const Transform* _targetTransform)
	{
#ifdef EDIT
		if (!isUIAnimation)	return;
#endif // EDIT

		// �قړ��^�C�~���O�ŋN�����ꍇ�ɏd�����Ȃ��悤�ɂ���
		if (!_targetTransform || curState == DirectionState::Win)
		{
			OnEndDirection();
			return;
		}

		curState = DirectionState::Lose;

		animationElapsedTime = 0.0f;

		// �������A�N�e�B�u��Ԃ�
		pGameOverObj->GetGameObject().SetActive(true);

		// �J�������o���n�߂�
		if (!pCamMove) return;	// �J�������Ȃ��Ȃ�
		pCamMove->OnPlayerWin(*_targetTransform);
	}

	BattleDirection::DirectionState BattleDirection::GetDirectionState() const
	{
		return curState;
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
		// ���Z�b�g
		animationElapsedTime = 0.0f;
		isStartFading = false;

		if (pGameStartObj)
		{
			// �����������̈ʒu�ɔz�u
			pGameStartObj->SetActive(true);
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
		if (isStartFading) return;	// �ŏ��̃t�F�[�h���͕����𓮂����Ȃ�

		animationElapsedTime += _deltaTime;

		float curPosX = Mathf::Lerp(
			START_START_POS_X,
			START_TARGET_POS_X,
			gameStartCurve.GetValue(animationElapsedTime / START_MOVE_TIME));

		// �������ړ�
		Transform& trans = pGameStartObj->GetTransform();
		DXSimp::Vector3 startPos = trans.GetPosition();
		startPos.x = curPosX;
		pGameStartObj->GetTransform().SetPosition(startPos);

		// ���Ԃ��߂�����I��
		if (animationElapsedTime > START_MOVE_TIME)
		{
			// �t�F�[�h���J����
			pGameStartObj->SetActive(false);
			pFade->OpenFade(START_SECOND_FADE_TIME);
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
				// �t�F�[�h���s��
				pWinObj->CloseFade(WIN_FADE_TIME, 1.0f);
				pFade->CloseFade(WIN_FADE_TIME, WIN_FADE_ALPHA);
				// �^�C���X�P�[�������ɖ߂�
				InSceneSystemManager::GetInstance()->SetTimeScale(1.0f);
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
			// �t�F�[�h���܂�؂�����
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
			// �t�F�[�h���܂�؂�����
			pFade->SetOnEndFunction([&]()
				{
					OnEndDirection();
				});

			break;
		}
	}

	void BattleDirection::OnEndDirection()
	{
		// �I���������Ƃ�ʒm
		NotifyAll(static_cast<int>(curState));

		// �ҋ@�ɖ߂�
		curState = DirectionState::Wait;
	}

	void BattleDirection::ImGuiDebug()
	{
#ifdef EDIT
		ImGui::Checkbox("UIAnimation", &isUIAnimation);

		// �Q�[���X�^�[�g�̈ړ�����
		gameStartCurve.ImGuiCall();

		// �I�u�W�F�N�g
		ImGuiSetObject();
#endif EDIT
	}

	void BattleDirection::ImGuiSetObject()
	{
#ifdef EDIT
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
#endif // EDIT

	}
}
