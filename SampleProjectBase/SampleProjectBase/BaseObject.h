#pragma once
#include "Cube.h"

class BaseObject
{
	static Transform transform;	// トランスフォーム
	static DirectX::SimpleMath::Color color;	// 色
	static std::unique_ptr<Cube> pCube;	// キューブ

	BaseObject() {};
	~BaseObject() {};
public:
	static void Init();	// 初期化

	// 座標を設定
	static void SetPosition(const DirectX::SimpleMath::Vector3& _position)
	{
		transform.position = _position;
	}
	// 回転を設定
	static void SetRotation(const DirectX::SimpleMath::Vector3& _rotation)
	{
		transform.rotation = _rotation;
	}
	// 大きさを設定
	static void SetScale(const DirectX::SimpleMath::Vector3& _scale)
	{
		transform.scale = _scale;
	}
	// 色
	static void SetColor(const DirectX::SimpleMath::Color& _color) { color = _color; }

	static void DrawCube();	// キューブ描画
};

