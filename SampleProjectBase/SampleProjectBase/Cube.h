#pragma once
#include "BasicObject_Base.h"

// �L���[�u�̃��b�V����\������N���X
class Cube : BasicObject_Base
{
	DirectX::SimpleMath::Vector3 length;	// ����

	void Make() override;	// ���_�f�[�^�Ȃǂ̍쐬
public:
	Cube();
	~Cube() override;

	void Draw() override;
};