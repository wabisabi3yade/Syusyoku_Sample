#pragma once
#include "Component.h"
// �e�̈ړ��R���|�[�l���g
class BulletMove :
	public Component
{
	float moveSpeed{ 0.0f };    // �ړ����x
	DirectX::SimpleMath::Vector3 moveVector;    // �ړ�����
	float destroyTime = 0.0f;   //���˂��ďI���܂ł̎���
	float elapsedTime = 0.0f;

public:
	using Component::Component;
	BulletMove& operator=(const BulletMove& _other);

	void Update()override;

	// �e�̈ړ����x�E�������Z�b�g����
	void SetMoveParameter(float _speed, DirectX::SimpleMath::Vector3 _vector, float _destroyTime);
};

