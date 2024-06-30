#pragma once
#include "Component.h"

class Camera;
class CameraMove : public Component
{
	const GameObject* pPlayer{ nullptr };	// �v���C���[�̃I�u�W�F�N�g
	Camera* camera{ nullptr };

	// UpdatePosition
	float nowHeight{ 0.0f };	//	���݂̍���
	float horizonDistance{ 0.0f };	//	XZ�̋���
	float moveSpeed{ 0.0f };	// �J�����̈ړ����x
	float playDistance{ 0.0f };	// �����̗V��

	// Updatelook
	float lookSpeed{ 0.0f };	// ���_�ړ����x
	float lookOffsetY{ 0.0f };	// Y���I�t�Z�b�g
	float lookPlayDistance{ 0.0f };	// Y���I�t�Z�b�g
	DirectX::SimpleMath::Vector3 lookPos;	// ���݂̒����_

	void UpdatePosition();	// �J�����̍��W���X�V����
	void UpdateLook();	// �J�����̒����_���X�V����

public:
	using Component::Component;

	virtual void Init();
	virtual void LateUpdate();

	virtual void ImGuiSetting() override;
	void SetPlayer(GameObject& _player) { pPlayer = &_player; }
};

