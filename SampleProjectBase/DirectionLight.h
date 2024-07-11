#pragma once
#include "Light.h"

struct DirectionLParameter
{
	LightParameter base;
	DirectX::SimpleMath::Vector3 direction{ 0.0f, -1.0f, 1.0f };
};

class DirectionLight
	: public Light
{
	// �����x�N�g��
	DirectX::SimpleMath::Vector3 direction;

	void Copy(const DirectionLight& _other);
public:
	DirectionLight() : direction(0.0f, -1.0f, 1.0f) { type = Type::Direction; }
	DirectionLight(const DirectionLight& _other);
	~DirectionLight() {}

	DirectionLight& operator=(const DirectionLight& _other);

	virtual void Draw() override;

	void SetDirection(const DirectX::SimpleMath::Vector3& _dir);

	const DirectX::SimpleMath::Vector3& GetDirection() { return direction; }
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

