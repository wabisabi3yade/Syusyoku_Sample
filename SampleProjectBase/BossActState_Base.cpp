#include "pch.h"
#include "BossActState_Base.h"
#include "BossActionController.h"
#include "CP_Boss.h"
#include "CP_Player.h"

namespace DXSimp = DirectX::SimpleMath;

constexpr u_int MAX_WARP_STEPS(10);	//	ワープモーションの最大回数

BossActState_Base::BossActState_Base() :
	stateType(BossState::None),
	pActionController(nullptr),
	pWarpTargetPos(nullptr),
	lastProgressRatio(0.0f),
	allWarpSteps(0),
	curWarpStep(0),
	isUseWarpMotion(false),
	isWarpMoving(false)
{
}

void BossActState_Base::Init(BossState _stateType, BossActionController& _actController)
{
	stateType = _stateType;
	pActionController = &_actController;
}

void BossActState_Base::OnStart()
{
	EnemyActState_Base::OnStart();

	OnStartBehavior();

	// パラメータリセット
	lastProgressRatio = 0.0f;
	curWarpStep = 0;
}

void BossActState_Base::Update()
{
	EnemyActState_Base::Update();

	WarpMotionUpdate();

	UpdateBehavior();

	TransitionCheckUpdate();
}

void BossActState_Base::OnEnd()
{
	EnemyActState_Base::OnEnd();

	OnEndBehavior();

	// 速度をリセット
	if (CP_RigidBody* pRb = GetRB())
	{
		pRb->SetVelocity({ 0.0f,0.0f,0.0f });
	}

	isWarpMoving = false;
}

void BossActState_Base::OnDamage()
{
	// のけぞらないなら終える
	if (!pActionController->GetCanKnock()) return;

	// のけぞるように
	ChangeState(BossState::Damage_Small, true);
}

nlohmann::json BossActState_Base::Save()
{
	nlohmann::json data;

	data["warpMotion"] = SaveWarpParameters();

	return data;
}

void BossActState_Base::Load(const nlohmann::json& _data)
{
	nlohmann::json warpData;
	if (HashiTaku::LoadJsonData("warpMotion", warpData, _data))
	{
		LoadWarpParameters(warpData);
	}
}

void BossActState_Base::ChangeState(BossState _nextState, bool _isForce)
{
	pActionController->ChangeState(_nextState, _isForce);
}

void BossActState_Base::CalcWarpDistance(const DirectX::SimpleMath::Vector3& _targetWorldPos)
{
	if (!isUseWarpMotion) return;

	// 距離を求める
	disToWarpTargePos = _targetWorldPos - GetBossTransform().GetPosition();

	// 距離制限を掛ける(マイナスは無制限)
	float maxMovement = warpMotionParams[curWarpStep - 1].maxMovementXZ;
	if (maxMovement < 0.0f) return;

	DXSimp::Vector2 distanceXZ = { disToWarpTargePos.x, disToWarpTargePos.z };
	float disLength = distanceXZ.Length();

	if (disLength > maxMovement)
	{
		float times = maxMovement / disLength;
		disToWarpTargePos.x *= times;
		disToWarpTargePos.z *= times;
	}
}

void BossActState_Base::SetWarpTargetPosReference(const DirectX::SimpleMath::Vector3& _targetPosRef)
{
	pWarpTargetPos = &_targetPosRef;
}

Transform& BossActState_Base::GetBossTransform()
{
	return pActionController->GetBoss().GetTransform();
}

Transform& BossActState_Base::GetPlayerTransform()
{
	return pActionController->GetPlayer().GetTransform();
}

CP_Animation* BossActState_Base::GetAnimation()
{
	return pActionController->GetAnimation();
}

CP_RigidBody* BossActState_Base::GetRB()
{
	return pActionController->GetRB();
}

float BossActState_Base::DeltaTime() const
{
	return pActionController->GetCharacter().DeltaTime();
}

void BossActState_Base::ImGuiDebug()
{
	ImGuiWarpDebug();
}

void BossActState_Base::WarpMotionUpdate()
{
	if (!isUseWarpMotion) return;	// ワープモーションしないなら
	if (allWarpSteps == 0) return;	// 数が0個なら

	float deltaTime = DeltaTime();
	if (deltaTime < Mathf::epsilon)  return;	// Δtが0なら

	assert((u_int)warpMotionParams.size() == allWarpSteps &&
		"ワープのパラメータの個数と回数が一致してません");
	assert(curWarpStep <= allWarpSteps && "現在の回数が最大回数を超えています");

	// アニメーションの割合から移動量の割合を取得する
	float animRatio = GetAnimation()->GetCurrentPlayRatio();

	// 次のワープに移行するか確認
	CheckTransNextWarp(animRatio);

	if (!isWarpMoving) return;
	// 現在のワープ
	const WarpMotionParam& curWarp = warpMotionParams[curWarpStep - 1];

	// 現在の距離の進行割合を求める
	float curProgressRatio = 0.0f;	
	if (curWarp.isFromRootMotion) // ルートモーションから求めるなら
	{
		// 現在は正面の移動にのみ適用
		DXSimp::Vector3 curRMPos = GetAnimation()->GetCurAnimRMPos(animRatio);
		curProgressRatio = (curRMPos.z - startRMPos.z) / (endRMPos.z - startRMPos.z);
	}
	else // カーブから求めるなら
	{
		// 区間内の進行割合を求めて、カーブ値を取得
		float curProgressRatio = (animRatio - curWarp.beginAnimRatio) /
			(curWarp.endAnimRatio - curWarp.beginAnimRatio);
		curProgressRatio = curWarp.horiMovementCurve.GetValue(curProgressRatio);
	}

	// 前回からの差を求める
	float diffCurveValue = curProgressRatio - lastProgressRatio;
	lastProgressRatio = curProgressRatio;

	// 今回進む距離を求め、秒速に変換してRBに速度を渡す
	DXSimp::Vector3 moveSpeed = disToWarpTargePos * diffCurveValue / deltaTime;
	moveSpeed.y = 0.0f;
	GetRB()->SetVelocity(moveSpeed);
}

void BossActState_Base::CheckTransNextWarp(float _animRatio)
{
	// 最大回数出ない時、次のワープに割合に入っているか確認
	bool isMax = curWarpStep == allWarpSteps;
	if (!isMax && _animRatio > warpMotionParams[curWarpStep].beginAnimRatio)
	{
		// 移動開始
		isWarpMoving = true;
		curWarpStep++;
		lastProgressRatio = 0.0f;

		WarpMotionParam& nextWarp = warpMotionParams[curWarpStep - 1];
		if (nextWarp.isFromRootMotion)
		{
			// 開始と終了のルートモーションを求める
			CP_Animation* pAnim = GetAnimation();
			startRMPos = pAnim->GetCurAnimRMPos(nextWarp.beginAnimRatio);
			endRMPos = pAnim->GetCurAnimRMPos(nextWarp.endAnimRatio);
		}

		// ポインタ設定されているなら
		if (pWarpTargetPos)
			CalcWarpDistance(*pWarpTargetPos);
	}

	// 移動中　かつ　現在のワープが終わっているなら
	if (isWarpMoving && _animRatio > warpMotionParams[curWarpStep - 1].endAnimRatio)
	{
		isWarpMoving = false;
		GetRB()->SetVelocity(DXSimp::Vector3::Zero);	// 前の慣性を消す
	}
}

void BossActState_Base::ImGuiWarpDebug()
{
#ifdef EDIT
	ImGui::Checkbox("UseWarpMotion", &isUseWarpMotion);

	if (!isUseWarpMotion) return;

	ImGuiMethod::PushItemSmallWidth();
	// 回数指定
	if (ImGui::Button("+"))
	{
		if (allWarpSteps >= MAX_WARP_STEPS) return;

		// ワープモーションの数を増やす
		allWarpSteps++;
		warpMotionParams.resize(allWarpSteps);
	}
	else if (ImGui::Button("-"))
	{
		if (allWarpSteps == 0) return;

		// ワープモーションの数を減らす
		allWarpSteps--;
		warpMotionParams.resize(allWarpSteps);
	}

	// ワープモーションのパラメータを調整する
	for (u_int w_i = 0; w_i < allWarpSteps; w_i++)
	{
		WarpMotionParam& param = warpMotionParams[w_i];

		if (!ImGuiMethod::TreeNode("Step:" + std::to_string(w_i + 1))) continue;

		ImGui::Checkbox("FromRM", &param.isFromRootMotion);

		// アニメーション割合
		ImGui::DragFloatRange2("animRatio",
			&param.beginAnimRatio,
			&param.endAnimRatio,
			0.001f,
			0.0f,
			1.0f);

		ImGui::DragFloat("maxDisXZ", &param.maxMovementXZ, 0.01f, -10.0f, 1000.0f);

		if (!param.isFromRootMotion)
		{
			ImGuiMethod::LineSpaceSmall();
			param.horiMovementCurve.ImGuiCall();
		}

		//// 縦移動するときのみ表示
		//ImGui::Checkbox("UseVertical", &param.isUseVertical);
		//if (!param.isUseVertical)
		//{
		//	ImGui::TreePop();
		//	continue;
		//}

		//ImGui::DragFloat("maxDisY", &param.maxMovementY, 0.01f, -10.0f, 1000.0f);
		//param.vertMovementCurve.ImGuiCall();

		ImGui::TreePop();
	}
	ImGui::PopItemWidth();
#endif // EDIT
}

nlohmann::json BossActState_Base::SaveWarpParameters()
{
	nlohmann::json warpData;
	warpData["useWarp"] = isUseWarpMotion;

	if (!isUseWarpMotion) return warpData;
	warpData["allWarpStep"] = allWarpSteps;

	// 構造体のセーブ
	nlohmann::json& warpStructDatas = warpData["warpStructs"];
	for (u_int w_i = 0; w_i < allWarpSteps; w_i++)
	{
		nlohmann::json paramData;
		WarpMotionParam& warpParam = warpMotionParams[w_i];

		paramData["fromRootMotion"] = warpParam.isFromRootMotion;

		// 割合
		paramData["beginRatio"] = warpParam.beginAnimRatio;
		paramData["endRatio"] = warpParam.endAnimRatio;

		// 横移動
		paramData["moveCurveXZ"] = warpParam.horiMovementCurve.Save();
		paramData["movementXZ"] = warpParam.maxMovementXZ;

		//// 縦移動
		//paramData["useVert"] = warpParam.isUseVertical;
		//if (warpParam.isUseVertical)
		//{
		//	paramData["moveCurveY"] = warpParam.vertMovementCurve.Save();
		//	paramData["movementY"] = warpParam.maxMovementY;
		//}
		warpStructDatas.push_back(paramData);	// 追加
	}

	return warpData;
}

void BossActState_Base::LoadWarpParameters(const nlohmann::json& _warpData)
{
	HashiTaku::LoadJsonBoolean("useWarp", isUseWarpMotion, _warpData);
	if (!isUseWarpMotion) return;

	HashiTaku::LoadJsonUnsigned("allWarpStep", allWarpSteps, _warpData);
	allWarpSteps = std::clamp<u_int>(allWarpSteps, 0, MAX_WARP_STEPS);
	warpMotionParams.resize(allWarpSteps);

	nlohmann::json warpStructDatas;
	if (!HashiTaku::LoadJsonDataArray("warpStructs", warpStructDatas, _warpData)) return;

	// 構造体
	u_int dataCnt = static_cast<u_int>(warpStructDatas.size());
	for (u_int d_i = 0; d_i < dataCnt; d_i++)
	{
		const nlohmann::json& paramData = warpStructDatas[d_i];
		WarpMotionParam& warpParam = warpMotionParams[d_i];

		HashiTaku::LoadJsonBoolean("fromRootMotion", warpParam.isFromRootMotion, paramData);

		// 割合
		HashiTaku::LoadJsonFloat("beginRatio", warpParam.beginAnimRatio, paramData);
		HashiTaku::LoadJsonFloat("endRatio", warpParam.endAnimRatio, paramData);

		// 横移動
		if (HashiTaku::IsJsonContains(paramData, "moveCurveXZ"))
		{
			warpParam.horiMovementCurve.Load(paramData["moveCurveXZ"]);
		}
		HashiTaku::LoadJsonFloat("movementXZ", warpParam.maxMovementXZ, paramData);

		//// 縦移動
		//HashiTaku::LoadJsonBoolean("useVert", warpParam.isUseVertical, paramData);
		//if (!warpParam.isUseVertical) continue;
		//if (HashiTaku::IsJsonContains(paramData, "moveCurveY"))
		//{
		//	warpParam.vertMovementCurve.Load(paramData["moveCurveY"]);
		//}
		//HashiTaku::LoadJsonFloat("movementY", warpParam.maxMovementY, paramData);
	}
}