#pragma once
#include "Component.h"
class PlayerMove :
	public Component
{
	float moveSpeed;
	DirectX::SimpleMath::Vector3 moveVec;   // ˆÚ“®•ûŒü

	void Input();   // “ü—Í
public:
	using Component::Component;

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Draw() override;
	void SetParameter() override;
};

