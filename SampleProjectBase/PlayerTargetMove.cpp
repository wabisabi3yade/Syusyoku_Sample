#include "pch.h"
#include "PlayerTargetMove.h"
#include "GameInput.h"
#include "GameObject.h"
#include "Geometory.h"

const float BLEND_OFFSET(0.5f);	// �u�����h��0.0f�`1.0f�Ɏ��߂�

PlayerTargetMove::PlayerTargetMove()
	: rotateSpeed(180.0f)
{
}

nlohmann::json PlayerTargetMove::Save()
{
	auto data = PlayerMoveState::Save();

	return data;
}

void PlayerTargetMove::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	PlayerMoveState::Load(_data);
}

void PlayerTargetMove::UpdateBehavior()
{
	//UpdateForward();

	Move();

	ApplyBlendAnim();

	ApplyRootMotion();
}

void PlayerTargetMove::OnEndBehavior()
{
	pAnimation->SetFloat(SPEEDRATIO_PARAMNAME, 0.0f);
}

void PlayerTargetMove::TransitionCheckUpdate()
{
	using enum GameInput::ButtonType;

	if (!IsRunning())
		ChangeState(PlayerState::Idle);

	else if (!pActionController->GetIsTargeting())
		ChangeState(PlayerState::Move);

	//else if (pPlayerInput->GetButtonDown(Player_Attack) && GetInputLeftStick().y > 0.8f)
	//	ChangeState(PlayerState::SpecialAtkHi);

	else if (pPlayerInput->GetButtonDown(Player_Attack))
		ChangeState(PlayerState::Attack11);
}

void PlayerTargetMove::UpdateForward()
{
	using namespace DirectX::SimpleMath;

	CP_BattleManager* pBattle = CP_BattleManager::GetInstance();
	if (!pBattle) return;

	float deltaTime = MainApplication::DeltaTime();
	Transform& myTransform = pActionController->GetPlayer().GetTransform();
	const Vector3& myPos = myTransform.GetPosition();

	// �G���X�g���擾
	CP_BattleManager::EnemyList enemyList = pBattle->GetEnemyList();

	CP_Enemy* enemy = *enemyList.begin();
	const Vector3& enemyPos = enemy->GetTransform().GetPosition();

	Vector3 targetVec = enemyPos - myPos;
	targetVec.y = 0.0f;
	targetVec.Normalize();

	// ���ʂƖڕW�x�N�g���Ƃ̊p�x�̍��������߂�
	float diffAng = acosf(targetVec.Dot(myTransform.Forward()));
	diffAng = std::max(diffAng, Mathf::smallValue);
	float deltaAngle = rotateSpeed * Mathf::degToRad * MainApplication::DeltaTime();

	// ��]���x�������𒴂�����
	if (diffAng < deltaAngle)
	{
		deltaAngle = diffAng;

	}

	// �ǂ��������ɉ�]�����邩
	float dotRight = targetVec.Dot(myTransform.Right());
	if (dotRight < 0.0f)
		deltaAngle *= -1;

	// ��]������
	Quaternion deltaRot = Quaternion::CreateFromAxisAngle(Vec3::Up, deltaAngle);
	Quaternion rot = myTransform.GetRotation();
	rot = Quat::Multiply(rot, deltaRot);
	myTransform.SetRotation(rot);
}

void PlayerTargetMove::ApplyBlendAnim()
{
	using namespace DirectX::SimpleMath;

	GameObject& playerObj = pActionController->GetPlayer().GetGameObject();

	// �A�j���[�V�����̃u�����h�������Z�b�g
	Vector2 playerFwd =
	{
		playerObj.GetTransform().Forward().x,
		playerObj.GetTransform().Forward().z
	};
	playerFwd.Normalize();

	Vector2 playerRight =
	{
		playerObj.GetTransform().Right().x,
		playerObj.GetTransform().Right().z
	};
	playerRight.Normalize();

	Vector2 moveVector2D =
	{
		moveVector.x,
		moveVector.z
	};

	Vector2 moveAxis = Vector2::One * BLEND_OFFSET;
	if (moveVector2D.Length() > Mathf::epsilon)
	{
		// �i�s�����ƉE�x�N�g���Ƃ̊p�x�����߂�
		float dotVR = moveVector2D.Dot(playerRight);
		float ang = acosf(dotVR);

		float dotVF = moveVector2D.Dot(playerFwd);
		if (dotVF < 0.0f)
			ang *= -1;

		moveAxis.x = cos(ang) * 0.5f + BLEND_OFFSET;
		moveAxis.y = sin(ang) * 0.5f + BLEND_OFFSET;	
	}

	pAnimation->SetFloat(MOVEAXIS_X_PARAMNAME, moveAxis.x);
	pAnimation->SetFloat(MOVEAXIS_Y_PARAMNAME, moveAxis.y);
}

void PlayerTargetMove::ApplyRootMotion()
{
	float rootMotion = pAnimation->GetMotionPosSpeedPerSec().Length();

	if (rootMotion > Mathf::epsilon)
	{
		float animPlaySpeed = currentSpeed / rootMotion;

		pAnimation->SetCurNodePlayerSpeed(animPlaySpeed);
	}

}

void PlayerTargetMove::ImGuiDebug()
{
	PlayerMoveState::ImGuiDebug();
}
