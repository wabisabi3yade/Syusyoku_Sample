#pragma once
#include "SingleMesh.h"

namespace HashiTaku
{
	// �����b�V���N���X
	class Sphere :
		public SingleMesh
	{
		void Make();	// ���_�f�[�^�Ȃǂ̍쐬

	public:
		Sphere();
		~Sphere() {}
	};
}