#pragma once
#include "BasicObject_Base.h"

// �L���[�u�̃��b�V����\������N���X
class Cube : public Mesh
{
	void Make();	// ���_�f�[�^�Ȃǂ̍쐬
public:
	Cube();
	~Cube() override;
};