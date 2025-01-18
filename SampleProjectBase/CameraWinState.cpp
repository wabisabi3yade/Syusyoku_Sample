#include "pch.h"
#include "CameraWinState.h"

#ifdef EDIT
#include "CP_BattleManager.h"
#include "CP_Player.h"
#endif // EDIT

namespace HashiTaku
{
	CameraWinState::CameraWinState() :
		pTargetTransform(nullptr),
		curStep(DirectionStep::Move),
		nextStep(DirectionStep::Move),
		moveEaseKind(EaseKind::Linear),
		lookEaseKind(EaseKind::Linear),
		fovEaseKindOnMove(EaseKind::Linear),
		fovEaseKindOnZoom(EaseKind::Linear),
		moveTime(0.5f),
		firstWaitTime(0.5f),
		zoomTime(0.5f),
		secondWaitTime(0.5f),
		curTargetWaitTime(0.0f),
		elapsedTime(0.0f),
		targetToDistance(3.0f),
		targetFovOnMove(45.0f),
		targetFovOnZoom(30.0f),
		beginFov(0.0f)
	{
	}

	void CameraWinState::SetTargetTransform(const Transform& _targetTransform)
	{
		pTargetTransform = &_targetTransform;

		// カメラの移動先の座標等を計算する
		// 中心点を求める
		DXSimp::Vector3 lookTargetPos = pTargetTransform->GetPosition() +
			centerPosOffset;

		// 中心点からのベクトルを乱数で求め、カメラの移動先を求める
		DXSimp::Vector3 targetToVecFromcCenter = GetRandomTargetVec();
		moveTargetPos = lookTargetPos + targetToVecFromcCenter * targetToDistance;

		// 視点対象までの回転量を求める
		DXSimp::Vector3 lookVec = lookTargetPos - moveTargetPos;
		lookVec.Normalize();
		lookTargetRotation = Quat::RotateToVector(lookVec);
	}

	json CameraWinState::Save()
	{
		auto data = CameraMoveState_Base::Save();

		// 移動
		data["moveEase"] = moveEaseKind;
		data["lookEase"] = lookEaseKind;
		data["fovEaseOnMove"] = fovEaseKindOnMove;
		data["moveTime"] = moveTime;
		data["targetFovOnMove"] = targetFovOnMove;
		data["targetToDistance"] = targetToDistance;
		SaveJsonVector3("centerPosOffset", centerPosOffset, data);
		SaveJsonVector3("randVecMax", randTargetVecMax, data);
		SaveJsonVector3("randVecMin", randTargetVecMin, data);


		// ズーム
		data["fovEaseOnZoom"] = fovEaseKindOnZoom;
		data["zoomTime"] = zoomTime;
		data["zoomTargetFov"] = targetFovOnZoom;

		// 待機
		data["firstWaitTime"] = firstWaitTime;
		data["secondWaitTime"] = secondWaitTime;
	
		return data;
	}

	void CameraWinState::Load(const json& _data)
	{
		CameraMoveState_Base::Load(_data);

		// 移動
		LoadJsonEnum<EaseKind>("moveEase", moveEaseKind, _data);
		LoadJsonEnum<EaseKind>("lookEase", lookEaseKind, _data);
		LoadJsonEnum<EaseKind>("fovEaseOnMove", fovEaseKindOnMove, _data);
		LoadJsonFloat("moveTime", moveTime, _data);
		LoadJsonFloat("targetFovOnMove", targetFovOnMove, _data);
		LoadJsonFloat("targetToDistance", targetToDistance, _data);
		LoadJsonVector3("centerPosOffset", centerPosOffset, _data);
		LoadJsonVector3("randVecMax", randTargetVecMax, _data);
		LoadJsonVector3("randVecMin", randTargetVecMin, _data);

		// ズーム
		LoadJsonEnum<EaseKind>("fovEaseOnZoom", fovEaseKindOnZoom, _data);
		LoadJsonFloat("zoomTime", zoomTime, _data);
		LoadJsonFloat("zoomTargetFov", targetFovOnZoom, _data);

		// 待機
		LoadJsonFloat("firstWaitTime", firstWaitTime, _data);
		LoadJsonFloat("secondWaitTime", secondWaitTime, _data);
	}

	void CameraWinState::OnStartBehavior()
	{
		// 移動状態から開始する
		BeginDirection(DirectionStep::Move);
		
		// 開始時のカメラの座標を求める
		beginCamPos = GetBasePosition();
	}

	void CameraWinState::UpdateBehavior()
	{
		HASHI_DEBUG_LOG(std::string(magic_enum::enum_name(curStep)));

		// 各ステップの更新処理
		switch (curStep)
		{
		case DirectionStep::Move:
			MoveUpdate();
			break;

		case DirectionStep::Zoom:
			ZoomUpdate();
			break;

		case DirectionStep::Wait:
			WaitUpdate();
			break;
		}
	}

	void CameraWinState::BeginDirection(DirectionStep _nextStep)
	{
		// 更新する
		curStep = _nextStep;
		elapsedTime = 0.0f;

		// 各ステップの更新処理
		switch (_nextStep)
		{
		case DirectionStep::Move:
			BeginMove();
			break;
			
		case DirectionStep::Zoom:
			BeginZoom();
			break;

		case DirectionStep::End:
			EndProcess();
			break;
		}
	}

	DXSimp::Vector3 CameraWinState::GetRandomTargetVec()
	{
		// 乱数で求める
		DXSimp::Vector3 retVec;
		retVec.x = Random::Range<float>(randTargetVecMin.x, randTargetVecMax.x);
		retVec.y = Random::Range<float>(randTargetVecMin.y, randTargetVecMax.y);
		retVec.z = Random::Range<float>(randTargetVecMin.z, randTargetVecMax.z);

		// ベクトルを正規化する
		retVec.Normalize();	

		return retVec;
	}

	void CameraWinState::BeginMove()
	{
		beginFov = GetCamera().GetFov();
	}

	void CameraWinState::MoveUpdate()
	{
		CP_Camera& camera = GetCamera();

		// シーンのタイムスケールに影響されたくないのでアプリケーションのΔtで計算
		elapsedTime += MainApplication::DeltaTime();
		float timeRate = elapsedTime / moveTime;

		// 移動の割合を求め、移動
		float moveRate = Easing::EaseValue(timeRate, moveEaseKind);
		DXSimp::Vector3 curCamPos = DXSimp::Vector3::Lerp(beginCamPos,
			moveTargetPos,
			moveRate
		);
		SetBasePosition(curCamPos);

		// 視点移動を行う
		float lookRate = Easing::EaseValue(timeRate, lookEaseKind);
		DXSimp::Quaternion curRotation = DXSimp::Quaternion::Slerp(
			beginCamRotation,
			lookTargetRotation,
			lookRate
		);
		camera.GetTransform().SetRotation(curRotation);

		// 視野角を移動
		float fovRate = Easing::EaseValue(timeRate, fovEaseKindOnMove);
		float curFov = std::lerp(beginFov, targetFovOnMove, fovRate);
		camera.SetFov(curFov);

		// 時間が過ぎれば次へ
		if (elapsedTime > moveTime)
		{
			// 待機をはさんだ後にズーム処理
			BeginWait(firstWaitTime, DirectionStep::Zoom);
		}
	}

	void CameraWinState::BeginWait(float _waitTime, DirectionStep _nextStep)
	{
		// 待機時間と次のステップを保存
		curTargetWaitTime = _waitTime;
		nextStep = _nextStep;

		elapsedTime = 0.0f;
		curStep = DirectionStep::Wait;
	}

	void CameraWinState::WaitUpdate()
	{
		elapsedTime += MainApplication::DeltaTime();

		// 待機時間が終了すれば
		if (elapsedTime > curTargetWaitTime)
		{
			// 次のステップへ
			BeginDirection(nextStep);
		}
	}

	void CameraWinState::BeginZoom()
	{
		beginFov = GetCamera().GetFov();
	}

	void CameraWinState::ZoomUpdate()
	{
		elapsedTime += MainApplication::DeltaTime();

		// 視野角を移動
		float fovRate = Easing::EaseValue(elapsedTime / zoomTime, fovEaseKindOnMove);
		float curFov = std::lerp(beginFov, targetFovOnZoom, fovRate);
		GetCamera().SetFov(curFov);

		// 時間が過ぎれば次へ
		if (elapsedTime > zoomTime)
		{
			// 待機をはさんだ後にズーム処理
			BeginWait(secondWaitTime, DirectionStep::End);
		}
	}

	void CameraWinState::EndProcess()
	{
		// 通常カメラのステート変更
		ChangeState(CameraState::Follow);
	}

	void CameraWinState::ImGuiDebug()
	{
#ifdef EDIT
		CameraMoveState_Base::ImGuiDebug();

		ImGuiMove();

		ImGuiZoom();

		ImGuiWait();

		if (ImGui::Button("Start"))
		{
			CP_BattleManager* pBattle = CP_BattleManager::GetInstance();
			if (!pBattle) return;

			CP_Player* pPlayer = pBattle->GetPlayerObject();
			if (!pPlayer) return;
			SetTargetTransform(pPlayer->GetTransform());
			ChangeState(CameraState::Win);
		}
#endif // EDIT	
	}

	void CameraWinState::ImGuiMove()
	{
#ifdef EDIT
		if (!ImGuiMethod::TreeNode("Move")) return;

		ImGui::DragFloat("moveTime", &moveTime, 0.1f, 0.01f, 100.0f);
		ImGui::DragFloat3("vecMax", &randTargetVecMax.x, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat3("vecMin", &randTargetVecMin.x, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat3("centerPosOffset", &centerPosOffset.x, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("targetDis", &targetToDistance, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("targetFov", &targetFovOnMove, 0.01f, 0.0f, 100.0f);
		Easing::ImGuiSelect(moveEaseKind, "move");
		Easing::ImGuiSelect(lookEaseKind, "look");
		Easing::ImGuiSelect(fovEaseKindOnMove, "fov");

		ImGui::TreePop();
#endif // EDIT
	}

	void CameraWinState::ImGuiZoom()
	{
#ifdef EDIT
		if (!ImGuiMethod::TreeNode("Zoom")) return;

		ImGui::DragFloat("zoomTime", &zoomTime, 0.1f, 0.01f, 100.0f);
		ImGui::DragFloat("targetFov", &targetFovOnZoom, 0.1f, 0.01f, 100.0f);
		Easing::ImGuiSelect(fovEaseKindOnZoom, "fov");

		ImGui::TreePop();

#endif // EDIT
	}

	void CameraWinState::ImGuiWait()
	{
#ifdef EDIT
		if (!ImGuiMethod::TreeNode("Wait")) return;

		ImGui::DragFloat("first", &firstWaitTime, 0.1f, 0.01f, 100.0f);
		ImGui::DragFloat("second", &secondWaitTime, 0.1f, 0.01f, 100.0f);

		ImGui::TreePop();
#endif // EDIT
	}
}

