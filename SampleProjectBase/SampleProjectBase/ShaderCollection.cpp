#include "ShaderCollection.h"

void ShaderCollection::Load()
{
	// cso�t�@�C���������Ă���t�H���_�܂ł̃p�X��
	const std::string shaderPath = "assets/Shader/";

	// ���_�V�F�[�_�[���쐬
	// cso�t�@�C���̖��O
	std::vector<std::string> vFileNames
	{
		"VS_Basic.cso",
		"VS_UVScroll.cso",
		"VS_BaseObject.cso",
		"VS_WorldPosition.cso",
		"VS_Gouraud.cso"
	};
	// �Z�b�g���閼�O
	std::vector<std::string> vShaderNames
	{
		"VS_Basic",
		"VS_UVScroll",
		"VS_BaseObject",
		"VS_WorldPosition",
		"VS_Gouraud"
	};
	defaultVS = "VS_Basic";

	for (int vsCount = 0; vsCount < static_cast<int>(vShaderNames.size()); vsCount++)
	{
		std::unique_ptr<VertexShader> vShader = std::make_unique<VertexShader>();
		std::string loadFilePath = shaderPath + vFileNames[vsCount];	// ���[�h����S�̂̃p�X��
		vShader->LoadCsoFile(loadFilePath.c_str());
		vShader->SetName(vShaderNames[vsCount]);
		shaders.emplace(vShaderNames[vsCount], std::move(vShader));	// �z��ɒǉ�����
	}

	// �s�N�Z���V�F�[�_�[���쐬
	// cso�t�@�C���̖��O
	std::vector<std::string> pFileNames
	{
		"PS_Basic.cso",
		"PS_VertexColor.cso",
		"PS_TexNotEnable.cso",
		"PS_Unlit.cso"
	};
	// �Z�b�g���閼�O
	std::vector<std::string> pShaderNames
	{
		"PS_Basic",
		"PS_VertexColor",
		"PS_TexNotEnable",
		"PS_Unlit"
	};
	defaultPS = "PS_Basic";

	for (int psCount = 0; psCount < static_cast<int>(pShaderNames.size()); psCount++)
	{
		std::unique_ptr<PixelShader> pShader = std::make_unique<PixelShader>();
		std::string loadFilePath = shaderPath + pFileNames[psCount];	// ���[�h����S�̂̃p�X��
		pShader->LoadCsoFile(loadFilePath.c_str());
		pShader->SetName((pShaderNames[psCount]));
		shaders.emplace(pShaderNames[psCount], std::move(pShader));	// �z��ɒǉ�����
	}
}

void ShaderCollection::Init()
{
	Load();	// cso�t�@�C�������[�h����
}

void ShaderCollection::SetShader(std::string _shaderName, std::unique_ptr<Shader> _pSetShader)
{
	auto itr = shaders.find(_shaderName);
	if (itr != shaders.end())	// �������O�̃V�F�[�_�[����������
	{
		ImGuiDebugLog::Add(_shaderName + " �����ɒ��_�V�F�[�_�[�z��ɂ���܂�");
		return;
	}

	// �z��ɒǉ�
	shaders.emplace(_shaderName, std::move(_pSetShader));
}

VertexShader* ShaderCollection::GetVertexShader(std::string _shaderName)
{
	auto itr = shaders.find(_shaderName);
	if (itr == shaders.end())	// �������O�̃V�F�[�_�[����������
	{
		ImGuiDebugLog::Add(_shaderName + " ��������܂���ł���");
		return nullptr;
	}

	VertexShader* retPtr = dynamic_cast<VertexShader*>(itr->second.get());
#ifdef EDIT
	if (retPtr == nullptr)
	{

		ImGuiDebugLog::Add(_shaderName + "�w�肵���V�F�[�_�[���͒��_�V�F�[�_�[�ł͂���܂���");
		return nullptr;
	}
#endif

	return retPtr;
}

PixelShader* ShaderCollection::GetPixelShader(std::string _shaderName)
{
	auto itr = shaders.find(_shaderName);
	if (itr == shaders.end())	// �������O�̃V�F�[�_�[����������
	{
		ImGuiDebugLog::Add(_shaderName + " ��������܂���ł���");
		return nullptr;
	}

	PixelShader* retPtr = dynamic_cast<PixelShader*>(itr->second.get());
#ifdef EDIT
	if (retPtr == nullptr)
	{

		ImGuiDebugLog::Add(_shaderName + "�w�肵���V�F�[�_�[�̓s�N�Z���V�F�[�_�[�ł͂���܂���");
		return nullptr;
	}
#endif

	return retPtr;
}


