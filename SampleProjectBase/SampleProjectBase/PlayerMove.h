#pragma once
#include "Component.h"

class Camera;
class PlayerMove :
	public Component
{
	float moveSpeed;
	DirectX::SimpleMath::Vector3 moveVec;   // 移動方向
	Camera* camera{nullptr};	// カメラ

	void Move();	// 移動
public:
	using Component::Component;

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Draw() override;
	void ImGuiSetting() override;

	// 移動方向決定
	void DecideMoveVector(DirectX::SimpleMath::Vector2 _input);   
};

