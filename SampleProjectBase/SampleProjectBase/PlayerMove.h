#pragma once
#include "Component.h"
class PlayerMove :
	public Component
{
	float moveSpeed;
	DirectX::SimpleMath::Vector3 moveVec;   // 移動方向

	void Input();   // 入力
public:
	using Component::Component;

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Draw() override;
	void SetParameter() override;
};

