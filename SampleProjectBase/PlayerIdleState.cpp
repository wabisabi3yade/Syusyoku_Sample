#include "pch.h"
#include "PlayerIdleState.h"

PlayerIdleState::PlayerIdleState()
	: PlayerActState_Base(StateType::Idle), leaveElapsedTime(0.0f), specialIdleTransTiime(3.0f)
{
}

void PlayerIdleState::OnStart()
{
	leaveElapsedTime = 0.0f;
}

void PlayerIdleState::Update()
{
	// �ړ��X�e�[�g�ɑJ�ڂł���Ȃ�
	if (IsCanMoveTransition())
	{
		ChangeState(StateType::Move);
	}

	ProgressLeaveElapse();
}

void PlayerIdleState::OnEnd()
{
}

void PlayerIdleState::ProgressLeaveElapse()
{
	/*leaveElapsedTime += MainApplication::DeltaTime();

	if (leaveElapsedTime > specialIdleTransTiime)
	{

	}*/
}

bool PlayerIdleState::IsCanMoveTransition()
{
	using namespace DirectX::SimpleMath;

	// ���͒l��0�Ȃ�
	Vector2 inputVal = GameInput::GetInstance()->GetValue(GameInput::ValueType::Player_Move);
	if (inputVal.Length() <= Mathf::epsilon)return false;

	return true;
}
