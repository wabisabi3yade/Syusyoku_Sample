#pragma once
#include <vector>
#include "ModelSettings.h"

class Mesh;
class Material;

// 3D���f���N���X(�����̃��b�V���ō\�������\����)
class Model
{
	std::vector<Mesh*> meshes;	// ���f���Ɏg���郁�b�V���z��
	u_int meshNum;	// ���b�V���̐�

	std::vector<Material*> materials;	// �}�e���A��

	bool Load(const ModelSettings& _settings, D3D11_Renderer& _renderer);
	// ���W�ϊ��s����쐬
	void SetupTransform();
public:
	Model(const ModelSettings& _settings);
	~Model();

	void Draw();
	void Release();

};

