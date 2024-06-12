#pragma once
#include "ModelSettings.h"
#include "Model.h"
#include "VertexShader.h"
#include "PixelShader.h"

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

	// ���̃��f���Ŏg���V�F�[�_�[
	VertexShader* pVertexShader;
	PixelShader* pPixelShader;

	bool LoadProcess(const ModelSettings& _settings, D3D11_Renderer& _renderer);

	void SetupDraw(const Transform& _transform) const;	// ���W�ϊ��s����쐬

	void ResetParam();	// ���b�V���Ȃǂ̏���S�ď���������
public:
	Model();
	~Model();

	// ���f���̏������g�̃��f���ɐݒ肷��(���b�V���Ȃ�)
	bool SetModel(const Model& _setModel);

	void Draw(const Transform& _transform) const;	// ���f���̕`�������
	void Release();	// ���f���̃C���X�^���X�������

	bool Load(const ModelSettings& _settings);	// �O��������n����ă��f�������[�h���鏈��

	bool GetImported()const { return isImported; }	// ���f���̏�񂪂��邩�t���O���擾
	u_int GetMeshNum()const { return meshNum; }	// ���f���̃��b�V�������擾
	const std::vector<Mesh*> GetMeshes()const { return meshes; }	// ���b�V���̔z����擾����
	const ModelSettings GetModelData()const { return modelData; }	// ���f���̃f�[�^���擾����

	void SetVertexShader(Shader* _vertexSh);
	void SetPixelShader(Shader* _pixelSh);
};

