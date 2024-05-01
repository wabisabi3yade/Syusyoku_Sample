#pragma once
#include <vector>

#include "ModelSettings.h"
class Mesh;

// 3D���f���N���X(�����̃��b�V���ō\�������\����)
class Model
{
	std::vector<Mesh*> pMeshes;	// ���f���Ɏg���郁�b�V���z��
	u_int meshNum = 0;	// ���b�V���̐�

	bool Load(const ModelSettings& _settings, D3D11_Renderer& _renderer);
	// ���W�ϊ��s����쐬
	void SetupTransform();
public:
	Model(const ModelSettings& _settings);
	~Model();

	void Draw();
	void Release();

};

