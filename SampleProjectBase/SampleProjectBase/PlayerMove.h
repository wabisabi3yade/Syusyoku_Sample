#pragma once
#include "Component.h"

class Camera;
class PlayerMove :
	public Component
{
	float moveSpeed;
	DirectX::SimpleMath::Vector3 moveVec;   // �ړ�����
	Camera* camera{nullptr};	// �J����

	void Move();	// �ړ�
public:
	using Component::Component;

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Draw() override;
	void ImGuiSetting() override;

	// �ړ���������
	void DecideMoveVector(DirectX::SimpleMath::Vector2 _input);   
};

