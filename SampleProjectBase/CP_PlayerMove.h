#pragma once
#include "Component.h"

class CP_Camera;
class CP_PlayerMove :
	public Component
{
	float moveSpeed;
	DirectX::SimpleMath::Vector3 moveVec;   // �ړ�����
	CP_Camera* camera{nullptr};	// �J����

	void Move();	// �ړ�
public:
	using Component::Component;

	void Init() override;
	void Update() override;
	void ImGuiSetting() override;
};

