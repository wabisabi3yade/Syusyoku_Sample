#include "pch.h"
#include "CP_Collider.h"
#include "Tag.h"

#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

const Color CP_Collider::normalColor = { 0,1,0,1 };
const Color CP_Collider::hitColor = { 1,0,0,1 };

CP_Collider::~CP_Collider()
{
	// �����蔻��`�F�b�J�[����폜����
	CollisionChecker& colCheck = InSceneSystemManager::GetInstance()->GetCollisonChecker();
	colCheck.PopCollider(*this);
}

CP_Collider& CP_Collider::operator=(const CP_Collider& _other)
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

void CP_Collider::Init()
{
	// �V�[�����̓����蔻��`�F�b�J�[�ɒǉ�����
	CollisionChecker& colCheck = InSceneSystemManager::GetInstance()->GetCollisonChecker();
	colCheck.AddCollider(*this);
}

void CP_Collider::SetTagColor(const Tag& _tag)
{
#ifdef EDIT







#endif // EDIT
}

void CP_Collider::SetLayerColor(const Layer& _layer)
{
#ifdef EDIT
	






#endif // EDIT
}
