#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

struct SceneLightsParam;

// �V�F�[�_�[���\�[�X��ۗL����N���X
class ShaderCollection : public Singleton_Base<ShaderCollection>
{
	friend class Singleton_Base<ShaderCollection>;

	// ���_�V�F�[�_�[���X�g
	std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;

	ShaderCollection() : defaultVS(""), defaultPS("") {};
	~ShaderCollection(){};
	void Load();	// cso�t�@�C�������[�h����
public:
	// �f�t�H���g�̃V�F�[�_�[�̖��O
	std::string defaultVS;	// ���_�V�F�[�_�[
	std::string defaultPS;	// �s�N�Z���V�F�[�_�[

	void Init();

	/// @brief �I�u�W�F�N�g���Ƃɕς��Ȃ��o�b�t�@�͍X�V����(���C�g�A�J�����Ȃ�)
	void UniqueBufferUpdate();

	// �擾����
	VertexShader* GetVertexShader(const std::string& _shaderName);
	PixelShader* GetPixelShader(const std::string& _shaderName);
};

