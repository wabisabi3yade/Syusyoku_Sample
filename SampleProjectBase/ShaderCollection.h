#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

struct SceneLightsParam;

// �V�F�[�_�[���\�[�X��ۗL����N���X
class ShaderCollection : public Singleton_Base<ShaderCollection>
{
	friend class Singleton_Base<ShaderCollection>;

	// �V�F�[�_�[�z��
	std::unordered_map<std::string, std::unique_ptr<Shader>> pShaders;

	ShaderCollection() : defaultVS(""), defaultPS("") {};
	~ShaderCollection(){};

public:
	// �f�t�H���g�̃V�F�[�_�[�̖��O
	std::string defaultVS;	// ���_�V�F�[�_�[
	std::string defaultPS;	// �s�N�Z���V�F�[�_�[

	void Init();

	/// @brief �I�u�W�F�N�g���Ƃŕς��Ȃ��o�b�t�@�����X�V����(���C�g�A�J�����Ȃ�)
	void UniqueUpdateBuffer();

	// �擾����
	VertexShader* GetVertexShader(const std::string& _shaderName);
	PixelShader* GetPixelShader(const std::string& _shaderName);

private:
	
	/// @brief CSO�t�@�C������ǂݍ���
	void LoadFromCSO();	

	/// @brief ���_�V�F�[�_�[
	void LoadVS();

	/// @brief �s�N�Z���V�F�[�_�[
	void LoadPS();

};

