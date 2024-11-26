#pragma once
#include "Light.h"

struct DirectionLParameter
{
	LightParameter base;
	DirectX::SimpleMath::Color ambeimt{ 0.5f, 0.5f, 0.5f, 1.0f };	// 環境光
	DirectX::SimpleMath::Vector3 direction{ 0.0f, -1.0f, 1.0f };	// 方向
	float dummy;
};

class DirectionLight
	: public Light
{
	// 環境色
	DirectX::SimpleMath::Vector4 ambiemt;

	// 方向ベクトル
	DirectX::SimpleMath::Vector3 direction;
public:
	DirectionLight() : ambiemt(0.6f, 0.6f, 0.6f, 1.0f), direction(0.0f, -1.0f, 1.0f) { type = Type::Direction; }
	~DirectionLight() {}

	virtual void Draw() override;

	void SetDirection(const DirectX::SimpleMath::Vector3& _dir);
	void SetAmbiemt(const DirectX::SimpleMath::Color& _color);

	DirectX::SimpleMath::Vector3 GetDirection();
	DirectX::SimpleMath::Color GetAmbiemt();

	DirectionLParameter GetParameter();
};

/// @brief ディレクションライトのNullオブジェクト
class NullDirectionLight : public DirectionLight
{
public:
	NullDirectionLight() {}
	~NullDirectionLight() {}

	void Draw() override { /*assert(!"Nullのディレクションライトが使われています");*/ };
};

