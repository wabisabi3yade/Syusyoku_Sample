#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

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

	void SetShader(std::string _shaderName, std::unique_ptr<Shader> _pSetShader);

	// �擾����
	VertexShader* GetVertexShader(const std::string& _shaderName);
	PixelShader* GetPixelShader(const std::string& _shaderName);
};

