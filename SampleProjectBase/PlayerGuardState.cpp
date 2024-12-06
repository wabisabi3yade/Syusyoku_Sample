#include "pch.h"
#include "PlayerGuardState.h"
#include "CP_Player.h"

namespace DXSimp = DirectX::SimpleMath;

constexpr auto GUARDEND_ANIM("Guard_End");
constexpr auto GUARDKNOCK_ANIM("Guard_Knock");
constexpr auto PARRYTRIGGER_NAME("parryTrigger");

PlayerGuardState::PlayerGuardState() : 
	canParry(false), sustainParryFrame(4), parryElapsedFrame(0), parryAddGuardGage(25.0f),
	canParryForwardAngle(180.0f)
{
}

bool PlayerGuardState::GetCanParry(const DirectX::SimpleMath::Vector3& _enemyPos)
{
	if (!canParry) return false;

	// �U���𐳖ʂŎ󂯂�Ă��邩�m�F(�G�Ƃ̍��W)
	// �v���C���[�̐��ʂɓG�����邩�m�F
	Transform& transform = GetTransform();
	DXSimp::Vector3 toEnemyVec = _enemyPos - transform.GetPosition();
	toEnemyVec.y = 0.0f; toEnemyVec.Normalize();
	float dot = toEnemyVec.Dot(transform.Forward());
	
	if (dot < cos(canParryForwardAngle * Mathf::degToRad * 0.5f)) return false;

	return true;
}

void PlayerGuardState::OnParry()
{
	// �p���B�ŃK�[�h�Q�[�W�𑝂₷
	GetPlayer().AddGuardGage(parryAddGuardGage);

	// �p���B�g���K�[��true
	GetAnimation()->SetTrigger(PARRYTRIGGER_NAME);

	// �A�j���[�V����
	GetAnimation()->SetBool(GUARD_PARAMNAME, false);

	// �O���͂���Ă�����
	if (IsInputVector(InputVector::Forward))
		ReleaseAttack();	// �U���ɔh��
	else
		GuardParry();
}

nlohmann::json PlayerGuardState::Save()
{
	auto data = PlayerActState_Base::Save();
	data["canParryTime"] = sustainParryFrame;
	data["parryAngle"] = canParryForwardAngle;
	return data;
}

void PlayerGuardState::Load(const nlohmann::json& _data)
{
	PlayerActState_Base::Load(_data);
	HashiTaku::LoadJsonUnsigned("canParryTime", sustainParryFrame, _data);
	HashiTaku::LoadJsonFloat("parryAngle", canParryForwardAngle, _data);
}

void PlayerGuardState::OnStartBehavior()
{
	// �p���B�ł����Ԃɂ���
	canParry = true;
	parryElapsedFrame = 0;

	// �A�j���[�V����
	GetAnimation()->SetBool(GUARD_PARAMNAME, true);
}

void PlayerGuardState::UpdateBehavior()
{
	// �p���B�ł��鎞�Ԃ̍X�V
	ParryTimeUpdate();

	CP_RigidBody& rb = GetRB();
	DXSimp::Vector3 velocity;
	velocity.y = rb.GetVelocity().y;
	GetRB().SetVelocity(velocity);
}

void PlayerGuardState::OnEndBehavior()
{
	// �p���B�ł��Ȃ�����
	canParry = false;
	
	// �A�j���[�V�����ɃK�[�h�I���ʒm
	GetAnimation()->SetBool(GUARD_PARAMNAME, false);
}

void PlayerGuardState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// �K�[�h�I���A�j���[�V�������I����
	if (_fromAnimNodeName == GUARDEND_ANIM || _fromAnimNodeName == GUARDKNOCK_ANIM)
		ChangeState(PlayerState::Idle);
}

void PlayerGuardState::ParryTimeUpdate()
{
	if (!canParry) return;

	// �i�߂�
	parryElapsedFrame++;

	// �p���B�ł���t���[���łȂ��Ȃ�����p���B�ł��Ȃ���Ԃ�
	if (parryElapsedFrame > sustainParryFrame)
	{
		canParry = false;
		GetAnimation()->SetBool(GUARD_PARAMNAME, false);
	}
}

void PlayerGuardState::ReleaseAttack()
{
	// �K�[�h�Q�[�W�������
	GetPlayer().ResetGuardGage();

	// �U���ɕύX
	ChangeState(PlayerState::SpecialAtkGuard);
}

void PlayerGuardState::GuardParry()
{
}

void PlayerGuardState::ImGuiDebug()
{
	PlayerActState_Base::ImGuiDebug();

	// �p���B�t���[��
	int imInt = static_cast<int>(sustainParryFrame);
	ImGui::DragInt("canParry", &imInt, 1, 0, 100);
	sustainParryFrame = static_cast<u_int>(imInt);

	ImGui::DragFloat("parryAngle", &canParryForwardAngle, 0.1f, 0.0f, 360.0f);
}
