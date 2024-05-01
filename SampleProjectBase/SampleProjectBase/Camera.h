#pragma once

// ゲーム内カメラクラス
class Camera
{
	// カメラの座標
	Float3 eyePos{ 0,0,0 };
	// カメラの注視点
	Float3 focusPos{ 0,0,0 };
	// カメラの上ベクトル
	Float3 camUp{ 0,1.f,0 };

public:
	Camera();
	~Camera();

	void Update();

	DirectX::XMMATRIX GetViewMatrix()const;
};

