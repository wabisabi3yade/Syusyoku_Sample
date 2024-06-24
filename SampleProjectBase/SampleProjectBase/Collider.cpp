#include "pch.h"
#include "Collider.h"
#include "CollisionRegister.h"
using namespace DirectX::SimpleMath;

const Color Collider::normalColor = { 0,1,0,1 };
const Color Collider::hitColor = { 1,0,0,1 };

Collider::~Collider()
{
	// �����蔻��`�F�b�J�[����폜����
	CollisionRegister::GetInstance()->PopCollider(*this);
}

void Collider::Init()
{
	// �V�[�����̓����蔻��`�F�b�J�[�ɒǉ�����
	CollisionRegister::GetInstance()->AddCollider(*this);
}
