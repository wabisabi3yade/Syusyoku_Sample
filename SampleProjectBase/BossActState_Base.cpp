#include "pch.h"
#include "BossActState_Base.h"
#include "BossActionController.h"
#include "CP_Boss.h"
#include "CP_Player.h"

namespace DXSimp = DirectX::SimpleMath;

constexpr u_int MAX_WARP_STEPS(10);	//	���[�v���[�V�����̍ő��

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

	// �p�����[�^���Z�b�g
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

	// ���x�����Z�b�g
	if (CP_RigidBody* pRb = GetRB())
	{
		pRb->SetVelocity({ 0.0f,0.0f,0.0f });
	}

	isWarpMoving = false;
}

void BossActState_Base::OnDamage()
{
	// �̂�����Ȃ��Ȃ�I����
	if (!pActionController->GetCanKnock()) return;

	// �̂�����悤��
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

	// ���������߂�
	disToWarpTargePos = _targetWorldPos - GetBossTransform().GetPosition();

	// �����������|����(�}�C�i�X�͖�����)
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
	if (!isUseWarpMotion) return;	// ���[�v���[�V�������Ȃ��Ȃ�
	if (allWarpSteps == 0) return;	// ����0�Ȃ�

	float deltaTime = DeltaTime();
	if (deltaTime < Mathf::epsilon)  return;	// ��t��0�Ȃ�

	assert((u_int)warpMotionParams.size() == allWarpSteps &&
		"���[�v�̃p�����[�^�̌��Ɖ񐔂���v���Ă܂���");
	assert(curWarpStep <= allWarpSteps && "���݂̉񐔂��ő�񐔂𒴂��Ă��܂�");

	// �A�j���[�V�����̊�������ړ��ʂ̊������擾����
	float animRatio = GetAnimation()->GetCurrentPlayRatio();

	// ���̃��[�v�Ɉڍs���邩�m�F
	CheckTransNextWarp(animRatio);

	if (!isWarpMoving) return;
	// ���݂̃��[�v
	const WarpMotionParam& curWarp = warpMotionParams[curWarpStep - 1];

	// ���݂̋����̐i�s���������߂�
	float curProgressRatio = 0.0f;	
	if (curWarp.isFromRootMotion) // ���[�g���[�V�������狁�߂�Ȃ�
	{
		// ���݂͐��ʂ̈ړ��ɂ̂ݓK�p
		DXSimp::Vector3 curRMPos = GetAnimation()->GetCurAnimRMPos(animRatio);
		curProgressRatio = (curRMPos.z - startRMPos.z) / (endRMPos.z - startRMPos.z);
	}
	else // �J�[�u���狁�߂�Ȃ�
	{
		// ��ԓ��̐i�s���������߂āA�J�[�u�l���擾
		float curProgressRatio = (animRatio - curWarp.beginAnimRatio) /
			(curWarp.endAnimRatio - curWarp.beginAnimRatio);
		curProgressRatio = curWarp.horiMovementCurve.GetValue(curProgressRatio);
	}

	// �O�񂩂�̍������߂�
	float diffCurveValue = curProgressRatio - lastProgressRatio;
	lastProgressRatio = curProgressRatio;

	// ����i�ދ��������߁A�b���ɕϊ�����RB�ɑ��x��n��
	DXSimp::Vector3 moveSpeed = disToWarpTargePos * diffCurveValue / deltaTime;
	moveSpeed.y = 0.0f;
	GetRB()->SetVelocity(moveSpeed);
}

void BossActState_Base::CheckTransNextWarp(float _animRatio)
{
	// �ő�񐔏o�Ȃ����A���̃��[�v�Ɋ����ɓ����Ă��邩�m�F
	bool isMax = curWarpStep == allWarpSteps;
	if (!isMax && _animRatio > warpMotionParams[curWarpStep].beginAnimRatio)
	{
		// �ړ��J�n
		isWarpMoving = true;
		curWarpStep++;
		lastProgressRatio = 0.0f;

		WarpMotionParam& nextWarp = warpMotionParams[curWarpStep - 1];
		if (nextWarp.isFromRootMotion)
		{
			// �J�n�ƏI���̃��[�g���[�V���������߂�
			CP_Animation* pAnim = GetAnimation();
			startRMPos = pAnim->GetCurAnimRMPos(nextWarp.beginAnimRatio);
			endRMPos = pAnim->GetCurAnimRMPos(nextWarp.endAnimRatio);
		}

		// �|�C���^�ݒ肳��Ă���Ȃ�
		if (pWarpTargetPos)
			CalcWarpDistance(*pWarpTargetPos);
	}

	// �ړ����@���@���݂̃��[�v���I����Ă���Ȃ�
	if (isWarpMoving && _animRatio > warpMotionParams[curWarpStep - 1].endAnimRatio)
	{
		isWarpMoving = false;
		GetRB()->SetVelocity(DXSimp::Vector3::Zero);	// �O�̊���������
	}
}

void BossActState_Base::ImGuiWarpDebug()
{
#ifdef EDIT
	ImGui::Checkbox("UseWarpMotion", &isUseWarpMotion);

	if (!isUseWarpMotion) return;

	ImGuiMethod::PushItemSmallWidth();
	// �񐔎w��
	if (ImGui::Button("+"))
	{
		if (allWarpSteps >= MAX_WARP_STEPS) return;

		// ���[�v���[�V�����̐��𑝂₷
		allWarpSteps++;
		warpMotionParams.resize(allWarpSteps);
	}
	else if (ImGui::Button("-"))
	{
		if (allWarpSteps == 0) return;

		// ���[�v���[�V�����̐������炷
		allWarpSteps--;
		warpMotionParams.resize(allWarpSteps);
	}

	// ���[�v���[�V�����̃p�����[�^�𒲐�����
	for (u_int w_i = 0; w_i < allWarpSteps; w_i++)
	{
		WarpMotionParam& param = warpMotionParams[w_i];

		if (!ImGuiMethod::TreeNode("Step:" + std::to_string(w_i + 1))) continue;

		ImGui::Checkbox("FromRM", &param.isFromRootMotion);

		// �A�j���[�V��������
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

		//// �c�ړ�����Ƃ��̂ݕ\��
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

	// �\���̂̃Z�[�u
	nlohmann::json& warpStructDatas = warpData["warpStructs"];
	for (u_int w_i = 0; w_i < allWarpSteps; w_i++)
	{
		nlohmann::json paramData;
		WarpMotionParam& warpParam = warpMotionParams[w_i];

		paramData["fromRootMotion"] = warpParam.isFromRootMotion;

		// ����
		paramData["beginRatio"] = warpParam.beginAnimRatio;
		paramData["endRatio"] = warpParam.endAnimRatio;

		// ���ړ�
		paramData["moveCurveXZ"] = warpParam.horiMovementCurve.Save();
		paramData["movementXZ"] = warpParam.maxMovementXZ;

		//// �c�ړ�
		//paramData["useVert"] = warpParam.isUseVertical;
		//if (warpParam.isUseVertical)
		//{
		//	paramData["moveCurveY"] = warpParam.vertMovementCurve.Save();
		//	paramData["movementY"] = warpParam.maxMovementY;
		//}
		warpStructDatas.push_back(paramData);	// �ǉ�
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

	// �\����
	u_int dataCnt = static_cast<u_int>(warpStructDatas.size());
	for (u_int d_i = 0; d_i < dataCnt; d_i++)
	{
		const nlohmann::json& paramData = warpStructDatas[d_i];
		WarpMotionParam& warpParam = warpMotionParams[d_i];

		HashiTaku::LoadJsonBoolean("fromRootMotion", warpParam.isFromRootMotion, paramData);

		// ����
		HashiTaku::LoadJsonFloat("beginRatio", warpParam.beginAnimRatio, paramData);
		HashiTaku::LoadJsonFloat("endRatio", warpParam.endAnimRatio, paramData);

		// ���ړ�
		if (HashiTaku::IsJsonContains(paramData, "moveCurveXZ"))
		{
			warpParam.horiMovementCurve.Load(paramData["moveCurveXZ"]);
		}
		HashiTaku::LoadJsonFloat("movementXZ", warpParam.maxMovementXZ, paramData);

		//// �c�ړ�
		//HashiTaku::LoadJsonBoolean("useVert", warpParam.isUseVertical, paramData);
		//if (!warpParam.isUseVertical) continue;
		//if (HashiTaku::IsJsonContains(paramData, "moveCurveY"))
		//{
		//	warpParam.vertMovementCurve.Load(paramData["moveCurveY"]);
		//}
		//HashiTaku::LoadJsonFloat("movementY", warpParam.maxMovementY, paramData);
	}
}