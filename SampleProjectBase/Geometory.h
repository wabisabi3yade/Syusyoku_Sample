#pragma once
#include "Cube.h"
#include "DebugCube.h"
#include "Sphere.h"

class Material;

class Geometory
{	
	// トランスフォーム
	static Transform transform;

	// Material
	static Material* pMaterial;

	// 色
	static DirectX::SimpleMath::Color color;	

	// キューブ
	static Cube* pCube;	
	
	// 球
	static Sphere* pSphere;	

	// デバッグのキューブ 
	static std::unique_ptr<DebugCube> pDebugCube;


	Geometory() {};
	~Geometory() {};

	static void DrawSetup();
public:
	static void Init();	// 初期化
	static void Release();

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

	static void DrawCube(bool _isWireFrame = false);	// キューブ描画

	static void DrawSphere(bool _isWireFrame = false);	// 球描画
};

