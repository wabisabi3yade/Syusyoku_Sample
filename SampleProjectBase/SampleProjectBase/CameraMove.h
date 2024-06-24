#pragma once
#include "Component.h"

class Camera;
class CameraMove : public Component
{
	const GameObject* pPlayer{nullptr};	// �v���C���[�̃I�u�W�F�N�g
	Camera* camera{ nullptr };
	//Roll
	float rotateSpeed{ 0.0f };	// ��]���x
	float centerAngle{ 0.0f };	// ���S�Ƃ̃��[���h���W�n�p�x
	float heightSpeed{ 0.0f };	// �������x
	float maxHeight{ 0.0f };	// �����̍ő�l�i�v���C���[����j
	float minHeight{ 0.0f };	// �����̍ŏ��l
	float nowHeight{ 0.0f };	//	���݂̍���

	// UpdatePosition
	float moveSpeed{ 0.0f };	// �J�����̈ړ����x
	float playDistance{ 0.0f };	// �v���C���[�Ƃ̗V�т̋���
	float distance{ 0.0f };	// �v���C���[�Ƃ̋���(XZ����)

	// Updatelook
	float lookSpeed{ 0.0f };	// ���_�ړ����x
	float lookOffsetY{0.0f};	// Y���I�t�Z�b�g
	DirectX::SimpleMath::Vector3 lookPos;	// ���݂̒����_
	
	void UpdatePosition();	// �J�����̍��W���X�V����
	void UpdateLook();	// �J�����̒����_���X�V����
	
public:
	using Component::Component;

	virtual void Init();
	virtual void LateUpdate();

	void Roll(DirectX::SimpleMath::Vector2 _input);	// �R���g���[���[���͂���J��������]����

	virtual void SetParameter();
	void SetPlayer(GameObject& _player) { pPlayer = &_player; }
};

