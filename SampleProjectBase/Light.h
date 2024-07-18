#pragma once
#include "GameObject.h"

struct LightParameter
{
	DirectX::SimpleMath::Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::SimpleMath::Vector3 position{ 0.0f, 0.0f, 0.0f };
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
private:
	// ライトの座標に目印
	bool isDebugDisplay;
protected:
	// ライトの種類
	Type type;

	// ライトのパラメータ構造体
	LightParameter parameter;
public:
	Light() : type(Type::None), isDebugDisplay(false) {}
	~Light() {};

	virtual void Draw() {};

	// 各パラメータセット
	void SetDisplay(bool _isDisplay) { isDebugDisplay = _isDisplay; }
	void SetPosition(const DirectX::SimpleMath::Vector3& _position) { parameter.position = _position; }
	void SetColor(const DirectX::SimpleMath::Color& _color) { parameter.color = _color; }

	bool GetDisplay() { return isDebugDisplay; }
	const DirectX::SimpleMath::Vector3& GetPosition() { return parameter.position; }
	const DirectX::SimpleMath::Color& GetColor() { return parameter.color; }
};