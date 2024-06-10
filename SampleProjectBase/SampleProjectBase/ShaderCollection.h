#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

// �V�F�[�_�[���\�[�X��ۗL����N���X
class ShaderCollection : public Singleton_Base<ShaderCollection>
{
	friend class Singleton_Base<ShaderCollection>;

	// ���_�V�F�[�_�[���X�g
	std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
	// ���̓��C�A�E�g�I�u�W�F�N�g�iVRAM�ɏ�����͂���I�u�W�F�N�g�j
	ID3D11InputLayout* pInputLayout = nullptr;

	ShaderCollection(){};
	~ShaderCollection(){};
	void Load();	// cso�t�@�C�������[�h����
public:
	void Init();

	void SetShader(std::string _shaderName, std::unique_ptr<Shader> _pSetShader);

	void SetInputLayout(ID3D11InputLayout* _pSetInput);

	// �擾����
	VertexShader* GetVertexShader(std::string _shaderName);
	PixelShader* GetPixelShader(std::string _shaderName);
	ID3D11InputLayout* GetInputLayout() { return pInputLayout; };
};

