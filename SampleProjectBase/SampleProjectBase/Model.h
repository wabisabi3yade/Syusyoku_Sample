#pragma once
#include "ModelSettings.h"
#include "Model.h"

class Mesh;
class Material;

// 3D���f���N���X(�����̃��b�V���ō\�������\����)
class Model
{
	// ���f���̖��O
	std::string modelName;
	// ���f������������Ȃ��悤�ɂ���
	bool isPermanent;
	// ���f����񂪂��邩�t���O
	bool isImported;

	// ���f���̐ݒ���
	ModelSettings modelData;

	std::vector<Mesh*> meshes;	// ���f���Ɏg���郁�b�V���z��
	u_int meshNum;	// ���b�V���̐�

	std::vector<Material*> materials;	// �}�e���A��

	bool Load(const ModelSettings& _settings, D3D11_Renderer& _renderer);
	// ���W�ϊ��s����쐬
	void SetupTransform(const Transform& _transform) const;
public:
	Model();
	~Model();

	// ���f���̕`�������
	void Draw(const Transform& _transform) const;
	void Release();

	// �O��������n����ă��f�������[�h���鏈��
	bool SetSetting(const ModelSettings& _settings);

	// ���f���̏�񂪂��邩�t���O���擾
	bool GetImported()const { return isImported; }
};

