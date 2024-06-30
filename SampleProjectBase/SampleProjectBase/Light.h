#pragma once
#include "GameObject.h"

struct LightParameter
{
	DirectX::SimpleMath::Vector3 position{ 0.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
};

// ���C�g�̊��
class Light
{
public:
	enum class Type
	{
		Direction,
		Point,
		Spot,
		Num,
		None
	};

protected:
	// ���C�g�̎��
	Type type;
	// ���C�g�̃p�����[�^�\����
	LightParameter parameter;

	void Copy(const Light& _other);
public:
	Light() : type(Type::None) {}
	Light(const Light& _other);
	~Light() {};
	Light& operator=(const Light& _other);

	// �e�p�����[�^�Z�b�g
	void SetPosition(const DirectX::SimpleMath::Vector3& _position) { parameter.position = _position; }
	void SeColor(const DirectX::SimpleMath::Color& _color) { parameter.color = _color; }
	Type GetType() { return type; }
};