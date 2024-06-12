#pragma once
#include "GameObject.h"

// ゲーム内カメラクラス
class Camera : public GameObject
{
	// カメラの注視点
	DirectX::SimpleMath::Vector3 focusPos{ 0,0,0 };
	// カメラの上ベクトル
	DirectX::SimpleMath::Vector3 camUp{ 0,1.f,0 };
public:
	Camera();
	~Camera();

	void LateUpdate() override;

	void UpdateViewMatrix();	// ビュー変換行列を更新
};

