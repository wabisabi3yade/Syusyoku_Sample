#include "pch.h"
#include "PlayerRollingMove.h"
#include "PlayerGroundActionController.h"
#include "CP_Player.h"

namespace HashiTaku
{
	constexpr auto ROLLING_ANIMNODE_NAME("RollingMove");	// ���[�����O�̃A�j���[�V�����m�[�h��

	PlayerRollingMove::PlayerRollingMove() :
		rollingDistance(5.0f), invicibleTime(0.4f), elapsedTime(0.0f),
		prevProgressDistance(0.0f)
	{
		pDistanceCurve = std::make_unique<AnimationCurve>();
	}

	void PlayerRollingMove::OnStartBehavior()
	{
		PlayerGroundState::OnStartBehavior();

		// ���͂̌����ɑ����Ɍ�����
		TurnInputVec();

		BeginParametar();
	}

	void PlayerRollingMove::UpdateBehavior()
	{
		using namespace DXSimp;
		PlayerGroundState::UpdateBehavior();

		// �ړ�
		Move();

		// ���G���Ԃ̍X�V
		UpdateInvicible();
	}

	void PlayerRollingMove::OnEndBehavior()
	{
		ClearVelocity(false);
		GetPlayer().SetIsInvicible(false);
	}

	void PlayerRollingMove::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
	{
		// �A�j���[�V�����̑J�ڐ悪�ҋ@��ԂȂ�ҋ@�ɖ߂�
		if (_toAnimNodeName == IDLE_ANIM_NAME)
			ChangeState(PlayerState::Idle);
	}

	void PlayerRollingMove::TurnInputVec()
	{
		using namespace DXSimp;
		Transform& transform = GetPlayer().GetTransform();
		PlayerGroundActionController& actCon = GetGroundController();

		// �������������߂�
		Transform& camTransform = actCon.GetCamera().GetTransform();
		Vector3 camForwardVec = camTransform.Forward();
		Vector3 camRightVec = camTransform.Right();
		Vector2 input = GetInputLeftStick();
		Vector3 turnVec;
		turnVec = camRightVec * input.x;
		turnVec += camForwardVec * input.y;
		turnVec.y = 0.0f;
		turnVec.Normalize();

		// �I�u�W�F�N�g��������
		Quaternion targetRotation = Quat::RotateToVector(turnVec);
		transform.SetRotation(targetRotation);
	}

	void PlayerRollingMove::BeginParametar()
	{
		elapsedTime = 0.0f;
		prevProgressDistance = 0.0f;

		// ���G
		GetPlayer().SetIsInvicible(true);

		// �A�j���[�V�����̔��f
		GetActionController().SetAnimationTrigger(ROLLING_ANIMPARAM);
	}

	void PlayerRollingMove::Move()
	{
		using namespace DXSimp;

		float deltaTime = DeltaTime();
		if (deltaTime < Mathf::epsilon) return;

		Transform& transform = GetPlayer().GetTransform();
		IActionController& actCon = GetActionController();

		// �A�j���[�V�����̍Đ���������i�ދ��������߂�
		float animPlayRatio = actCon.GetAnimation()->GetCurrentPlayRatio();
		float curveDistance = pDistanceCurve->GetValue(animPlayRatio) * rollingDistance;

		// �O��Ƃ̍����l�����߂�
		float moveDistance = curveDistance - prevProgressDistance;

		actCon.SetVelocity(transform.Forward() * moveDistance / deltaTime);

		// ���t���[���ׂ̈ɍX�V
		prevProgressDistance = curveDistance;
	}

	void PlayerRollingMove::UpdateInvicible()
	{
		// ���G���ԓ�������
		elapsedTime += DeltaTime();
		CP_Player& player = GetPlayer();

		if (player.GetIsInvicible() && elapsedTime > invicibleTime)
			GetGroundController(). GetPlayer().SetIsInvicible(false);
	}

	void PlayerRollingMove::ImGuiDebug()
	{
		PlayerGroundState::ImGuiDebug();

		ImGui::DragFloat("Distance", &rollingDistance, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat("InvicibleTime", &invicibleTime, 0.01f, 0.0f, 5.0f);
		pDistanceCurve->ImGuiCall();
	}

	json PlayerRollingMove::Save()
	{
		auto data = PlayerGroundState::Save();

		data["rollingDistance"] = rollingDistance;
		data["invicibleTime"] = invicibleTime;
		data["speedCurve"] = pDistanceCurve->Save();

		return data;
	}

	void PlayerRollingMove::Load(const json& _data)
	{
		using namespace HashiTaku;

		PlayerGroundState::Load(_data);

		LoadJsonFloat("rollingDistance", rollingDistance, _data);
		LoadJsonFloat("invicibleTime", invicibleTime, _data);

		json speedCurveData;
		if (LoadJsonData("speedCurve", speedCurveData, _data))
		{
			pDistanceCurve->Load(speedCurveData);
		}
	}
}