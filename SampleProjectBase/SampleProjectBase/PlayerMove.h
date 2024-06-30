#pragma once
#include "Component.h"

class Camera;
class PlayerMove :
	public Component
{
	float moveSpeed;
	DirectX::SimpleMath::Vector3 moveVec;   // ˆÚ“®•ûŒü
	Camera* camera{nullptr};	// ƒJƒƒ‰

	void Move();	// ˆÚ“®
public:
	using Component::Component;

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Draw() override;
	void ImGuiSetting() override;

	// ˆÚ“®•ûŒüŒˆ’è
	void DecideMoveVector(DirectX::SimpleMath::Vector2 _input);   
};

