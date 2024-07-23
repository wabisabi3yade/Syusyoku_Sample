#pragma once
#include "Light.h"

struct DirectionLParameter
{
	LightParameter base;
	DirectX::SimpleMath::Color ambeimt{ 0.3f, 0.3f, 0.3f, 1.0f };	// ����
	DirectX::SimpleMath::Vector3 direction{ 0.0f, -1.0f, 1.0f };	// ����
};

class DirectionLight
	: public Light
{
	// ���F
	DirectX::SimpleMath::Vector4 ambiemt;

	// �����x�N�g��
	DirectX::SimpleMath::Vector3 direction;
public:
	DirectionLight() : direction(0.0f, -1.0f, 1.0f) { type = Type::Direction; }
	~DirectionLight() {}

	virtual void Draw() override;

	void SetDirection(const DirectX::SimpleMath::Vector3& _dir);
	void SetAmbeimt(const DirectX::SimpleMath::Color& _color);

	DirectX::SimpleMath::Vector3 GetDirection();
	DirectX::SimpleMath::Color GetAmbiemt();

	DirectionLParameter GetParameter();
};

/// @brief �f�B���N�V�������C�g��Null�I�u�W�F�N�g
class NullDirectionLight : public DirectionLight
{
public:
	NullDirectionLight() {}
	~NullDirectionLight() {}

	void Draw() override { /*assert(!"Null�̃f�B���N�V�������C�g���g���Ă��܂�");*/ };
};

