#include "pch.h"
#include "Collider.h"
#include "CollisionRegister.h"
#include "Tag.h"

using namespace DirectX::SimpleMath;

const Color Collider::normalColor = { 0,1,0,1 };
const Color Collider::hitColor = { 1,0,0,1 };

Collider::~Collider()
{
	// �����蔻��`�F�b�J�[����폜����
	CollisionRegister::GetInstance()->PopCollider(*this);
}

Collider& Collider::operator=(const Collider& _other)
{
	if (this == &_other) return *this;
	Component::operator=(_other);

	this->type = _other.type;

#ifdef EDIT
	// ����ɓ�����ƐF��ς���
	colorTags = _other.colorTags;
	colorLayers = _other.colorLayers;
#endif // EDIT

	return *this;
}

void Collider::Init()
{
	// �V�[�����̓����蔻��`�F�b�J�[�ɒǉ�����
	CollisionRegister::GetInstance()->AddCollider(*this);
}

void Collider::SetTagColor(const Tag& _tag)
{
#ifdef EDIT







#endif // EDIT
}

void Collider::SetLayerColor(const Layer& _layer)
{
#ifdef EDIT
	






#endif // EDIT
}
