#pragma once
#include "GameObject.h"

// ゲーム内カメラクラス
class Camera : public GameObject
{
	// カメラの注視点
	DirectX::SimpleMath::Vector3 focusPos{ 0,0,0 };
	// カメラの上ベクトル
	DirectX::SimpleMath::Vector3 camUp{ 0,1.f,0 };

	float DEFAULT_FOV = 45.0f;
	float DEFAULT_NEARZ = 0.1f;
	float DEFAULT_FARZ = 1000.0f;

	void UpdateFocus();	// 注視点の座標をカメラの角度から更新する
public:
	Camera();
	~Camera();

	void LateUpdate() override;

	void UpdateViewMatrix();	// ビュー変換行列を更新

	// 
	void LookAt(DirectX::SimpleMath::Vector3 _targetPos);
};

