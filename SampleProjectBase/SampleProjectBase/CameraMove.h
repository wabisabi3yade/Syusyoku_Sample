#pragma once
#include "Component.h"

class Camera;
class CameraMove : public Component
{
	const GameObject* pPlayer{ nullptr };	// プレイヤーのオブジェクト

	// カメラオブジェクト
	Camera* camera{ nullptr };

	// カメラの移動速度
	float moveSpeed{ 0.0f };

	// 視点移動速度
	float lookSpeed{ 0.0f };

	// 移動方向ベクトル
	DirectX::SimpleMath::Vector3 moveVec;

	// 視点移動ベクトル
	DirectX::SimpleMath::Vector3 rotateVec;

	void UpdateVector();
	void Move();

public:
	using Component::Component;

	virtual void Init();
	virtual void LateUpdate();

	

	virtual void ImGuiSetting() override;
};

