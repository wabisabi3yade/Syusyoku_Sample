#pragma once
#include "Mesh.h"

// �����b�V���N���X
class Sphere :
    public Mesh
{
	void Make();	// ���_�f�[�^�Ȃǂ̍쐬

public:
	Sphere();
	~Sphere() override;
};

