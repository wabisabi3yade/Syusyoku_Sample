#pragma once
#include "Component.h"

class Camera;
class CameraMove : public Component
{
	const GameObject* pPlayer{ nullptr };	// プレイヤーのオブジェクト
	Camera* camera{ nullptr };

	// UpdatePosition
	float nowHeight{ 0.0f };	//	現在の高さ
	float horizonDistance{ 0.0f };	//	XZの距離
	float moveSpeed{ 0.0f };	// カメラの移動速度
	float playDistance{ 0.0f };	// 距離の遊び

	// Updatelook
	float lookSpeed{ 0.0f };	// 視点移動速度
	float lookOffsetY{ 0.0f };	// Y軸オフセット
	float lookPlayDistance{ 0.0f };	// Y軸オフセット
	DirectX::SimpleMath::Vector3 lookPos;	// 現在の注視点

	void UpdatePosition();	// カメラの座標を更新する
	void UpdateLook();	// カメラの注視点を更新する

public:
	using Component::Component;

	virtual void Init();
	virtual void LateUpdate();

	virtual void ImGuiSetting() override;
	void SetPlayer(GameObject& _player) { pPlayer = &_player; }
};

