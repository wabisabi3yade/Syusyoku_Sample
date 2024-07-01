#pragma once
#include "GameObject.h"

struct LightParameter
{
	DirectX::SimpleMath::Vector3 position{ 0.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
};

// ライトの基底
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
	// ライトの種類
	Type type;
	// ライトのパラメータ構造体
	LightParameter parameter;

	void Copy(const Light& _other);
public:
	Light() : type(Type::None) {}
	Light(const Light& _other);
	~Light() {};
	Light& operator=(const Light& _other);

	virtual void Draw() {};

	// 各パラメータセット
	void SetPosition(const DirectX::SimpleMath::Vector3& _position) { parameter.position = _position; }
	void SetColor(const DirectX::SimpleMath::Color& _color) { parameter.color = _color; }

	const DirectX::SimpleMath::Vector3& GetPosition() { return parameter.position; }
	const DirectX::SimpleMath::Color& GetColor() { return parameter.color; }
};