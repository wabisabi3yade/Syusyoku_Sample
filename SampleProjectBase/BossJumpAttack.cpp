#include "pch.h"
#include "BossJumpAttack.h"

#include "Geometory.h"

namespace DXSimp = DirectX::SimpleMath;

// ���ʃx�N�g���ƃv���C���[�Ƃ̊p�x�����̒l�ȉ��Ȃ�u���Ɍ���
constexpr float LOOKMOMENT_DEGREE_RANGE{ 20.0f };

// ���ړ��J�[�u�̒l������ȏゾ�Əu���ɐU��������Ƃ��ł���
constexpr float CAN_LOOKMOMENT_HORICURVE_VAL{ 0.5f };

BossJumpAttack::BossJumpAttack() :
	maxHoriSpeed(12.0f), 
	maxJumpHeight(5.0f),
	prevJumpHeight(0.0f),
	horiSpeedCofficient(2.0f),
	fallPosOffset(0.0f)
{
}

void BossJumpAttack::OnStartBehavior()
{
	BossAttackState::OnStartBehavior();

	prevJumpHeight = 0.0f;
}

void BossJumpAttack::UpdateBehavior()
{
	BossAttackState::UpdateBehavior();

	// �W�����v�̓���
	JumpMove();
}

void BossJumpAttack::DebugDisplay()
{
#ifdef EDIT
	const Transform& t = GetBossTransform();
	Geometory::SetPosition(t.GetPosition() + t.Forward() * fallPosOffset);
	Geometory::SetScale(DXSimp::Vector3::One * 0.1f);
	Geometory::DrawSphere();
#endif
}

nlohmann::json BossJumpAttack::Save()
{
	auto data = BossAttackState::Save();

	data["jumpHeight"] = maxJumpHeight;
	data["jumpHeightCurve"] = jumpHeightDisCurve.Save();

	data["fallOffset"] = fallPosOffset;
	data["maxHoriSpeed"] = maxHoriSpeed;
	data["speedCof"] = horiSpeedCofficient;
	data["jumpHoriCurve"] = horiSpeedCofficientCurve.Save();

	return data;
}

void BossJumpAttack::Load(const nlohmann::json& _data)
{
	BossAttackState::Load(_data);

	HashiTaku::LoadJsonFloat("fallOffset", fallPosOffset, _data);
	HashiTaku::LoadJsonFloat("jumpHeight", maxJumpHeight, _data);
	HashiTaku::LoadJsonFloat("maxHoriSpeed", maxHoriSpeed, _data);
	HashiTaku::LoadJsonFloat("speedCof", horiSpeedCofficient, _data);

	nlohmann::json curveData;
	if (HashiTaku::LoadJsonData("jumpHeightCurve", curveData, _data))
	{
		jumpHeightDisCurve.Load(curveData);
	}
	if (HashiTaku::LoadJsonData("jumpHoriCurve", curveData, _data))
	{
		horiSpeedCofficientCurve.Load(curveData);
	}
}

void BossJumpAttack::JumpMove()
{
	// ���x�����Z�b�g
	curMoveSpeed = DXSimp::Vector3::Zero;

	// ���̈ړ�
	HorizonMove();

	// �c�̈ړ�
	VertivalMove();

	// ���x��Rb�ɓn��
	GetRB().SetVelocity(curMoveSpeed);
}

void BossJumpAttack::HorizonMove()
{
	// �v���C���[�Ƃ̋��������đ��x��ύX���Ȃ���A�߂Â��Ă���
	Transform& bossTrans = GetBossTransform();
	const DXSimp::Vector3& playerPos = GetPlayerTransform().GetPosition();
	float deltaTime = DeltaTime();
	float animRatio = GetAnimation()->GetCurrentPlayRatio();

	// ���x�W�����擾����
	float curSpeedCofficient = horiSpeedCofficientCurve.GetValue(animRatio);

	// �v���C���[�ɍ��킹����W���擾
	DXSimp::Vector3 playerMatchPos = bossTrans.GetPosition() + bossTrans.Forward() * fallPosOffset;

	// �v���C���[�Ƃ̋������擾����
	DXSimp::Vector3 toPlayerDistance = playerPos - playerMatchPos;
	toPlayerDistance.y = 0.0f;	// �������̋���

	// �{�X���v���C���[�֌����Ă���
	DXSimp::Vector3 toPlayerVec;
	toPlayerDistance.Normalize(toPlayerVec);

	// �v���C���[�Ƃ̋����̒���
	float toPlayerLength = toPlayerDistance.Length();

	// �������瑬�x�����߂�(�ő呬�x�𐧌�)
	float speed = toPlayerLength * curSpeedCofficient * horiSpeedCofficient;
	if (speed > maxHoriSpeed)
		speed = maxHoriSpeed;

	// �ړ����x�����߂�iXZ���������Z�b�g�j
	DXSimp::Vector3 moveSpeedXZ = toPlayerVec * speed;
	curMoveSpeed.x = moveSpeedXZ.x; curMoveSpeed.z = moveSpeedXZ.z;
}

void BossJumpAttack::VertivalMove()
{
	if (DeltaTime() < Mathf::epsilon) return;

	Transform& bossTrans = GetBossTransform();
	float deltaTime = DeltaTime();
	float animRatio = GetAnimation()->GetCurrentPlayRatio();

	// �����̈ړ�
	// ���x���v�Z
	float curJumpHeight = jumpHeightDisCurve.GetValue(animRatio) * maxJumpHeight;
	float jumpSpeed = (curJumpHeight - prevJumpHeight) / deltaTime;

	// ���x���
	curMoveSpeed.y = jumpSpeed;

	// ���t���[���ׂ̈ɍX�V
	prevJumpHeight = curJumpHeight;
}

void BossJumpAttack::ImGuiDebug()
{
	BossAttackState::ImGuiDebug();

	// Hori
	ImGui::DragFloat("FallOffset", &fallPosOffset, 0.01f);
	ImGui::DragFloat("MaxHoriSpd", &maxHoriSpeed, 1.0f, 0.0f, 1000.0f);
	ImGui::DragFloat("HoriSpeed", &horiSpeedCofficient, 1.0f, 0.0f, 1000.0f);
	horiSpeedCofficientCurve.ImGuiCall();

	// Vert
	ImGui::DragFloat("Height", &maxJumpHeight, 0.01f, 0.0f, 1000.0f);
	jumpHeightDisCurve.ImGuiCall();
}
