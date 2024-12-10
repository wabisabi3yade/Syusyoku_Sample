#include "pch.h"
#include "PlayerRushAttack.h"
#include "GameObject.h"

namespace DXSimp = DirectX::SimpleMath;

constexpr float BEGIN_JUMP_ANIMRATIO(0.380f);	// �W�����v�A�j���[�V��������
constexpr float BEGIN_SLASH_ANIMRATIO(0.465f);	// �a��n�߂�A�j���[�V��������

PlayerRushAttack::PlayerRushAttack() :
	pTargetObj(nullptr), 
	progressLengthHori(5.0f),
	prevDistanceHori(0.0f),
	slashBeginDistance(0.7f), 
	cancelBeginRatio(0.380f),
	slashBeginAnimRatio(0.465f),
	isProgressForward(false)
{
	distanceHoriCurve.SetCurveName("Horizon");
}

nlohmann::json PlayerRushAttack::Save()
{
	auto data = PlayerAttackState::Save();

	data["slashBegin"] = slashBeginDistance;
	data["horiDis"] = progressLengthHori;
	data["horiCurve"] = distanceHoriCurve.Save();
	data["cancelAnim"] = cancelBeginRatio;
	data["slashAnim"] = slashBeginAnimRatio;

	return data;
}

void PlayerRushAttack::Load(const nlohmann::json& _data)
{
	PlayerAttackState::Load(_data);

	HashiTaku::LoadJsonFloat("slashBegin", slashBeginDistance, _data);
	HashiTaku::LoadJsonFloat("horiDis", progressLengthHori, _data);
	HashiTaku::LoadJsonFloat("cancelAnim", cancelBeginRatio, _data);
	HashiTaku::LoadJsonFloat("slashAnim", slashBeginAnimRatio, _data);
	if (HashiTaku::IsJsonContains(_data, "horiCurve"))
	{
		distanceHoriCurve.Load(_data["horiCurve"]);
	}
}

void PlayerRushAttack::OnStartBehavior()
{
	PlayerAttackState::OnStartBehavior();

	// �^�[�Q�b�g����擾����
	pTargetObj = pActionController->GetTargetAccepter();

	// ���Z�b�g
	prevDistanceHori = 0.0f;
	isProgressForward = true;
}

void PlayerRushAttack::UpdateBehavior()
{
	using namespace DirectX::SimpleMath;

	PlayerAttackState::UpdateBehavior();

	// �O�ɓːi
	MoveForward();
}

void PlayerRushAttack::TransitionCheckUpdate()
{
	PlayerAttackState::TransitionCheckUpdate();
}

void PlayerRushAttack::MoveForward()
{
	// �O�ɐi�܂Ȃ��Ȃ�
	if (!isProgressForward) return;
	
	float deltaTime = DeltaTime();
	if (deltaTime < Mathf::epsilon) return;	// ��t��0�Ȃ�

	Transform& playerTrans = GetPlayer().GetGameObject().GetTransform();

	// �A�j���[�V�����̊������擾����
	CP_Animation* pAnimation = pActionController->GetAnimation();
	if (!pAnimation) return;
	float animPlayRatio = pAnimation->GetCurrentPlayRatio();

	// �J�[�u����i�ދ������擾
	float horiDistance = progressLengthHori * distanceHoriCurve.GetValue(animPlayRatio);

	// �O�񂩂�̍��������߂�
	float diffHoriDis = horiDistance - prevDistanceHori;

	// �ړ����x�����߂�
	float horiSpeed = diffHoriDis / deltaTime;

	// �i�߂�
	GetRB().SetVelocity(playerTrans.Forward() * horiSpeed);

	// ���t���[���ׂ̈ɍX�V
	prevDistanceHori = horiDistance;

	// �G�Ƃ̈�苗���ɓ�������
	if (CheckDistanceToEnemy(animPlayRatio))
	{
		// �O�ɐi�܂Ȃ�����
		isProgressForward = false;

		// �A�j���[�V�������a��n�߂�悤�ɂ���
		pAnimation->SetPlayRatio(slashBeginAnimRatio);

		GetRB().SetVelocity(DXSimp::Vector3::Zero);
	}
}

bool PlayerRushAttack::CheckDistanceToEnemy(float _currentPlayRatio)
{
	if (!pTargetObj) return false;

	// �W�����v���Ďa��n�߂�Ԃ����`�F�b�N
	if (_currentPlayRatio < cancelBeginRatio) return false;
	if (_currentPlayRatio > slashBeginAnimRatio) return false;

	DXSimp::Vector3 distance = GetMyTransform().GetPosition() -
		pTargetObj->GetWorldPos();
	distance.y = 0.0f;	// y�͍l�����Ȃ�

	if (distance.Length() > slashBeginDistance) return false;

	return true;
}

bool PlayerRushAttack::isExistTrgeter() const
{
	return pTargetObj != nullptr;
}

void PlayerRushAttack::ImGuiDebug()
{
	PlayerAttackState::ImGuiDebug();

	ImGui::DragFloat("SlashBeginDis", &slashBeginDistance, 0.01f, 0.0f, 1000.0f);
	ImGui::DragFloat("HoriDis", &progressLengthHori, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("cancelBeginRatio", &cancelBeginRatio, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("slashRatio", &slashBeginAnimRatio, 0.001f, 0.0f, 1.0f);


	distanceHoriCurve.ImGuiCall();
}
