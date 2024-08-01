#include "ShaderCollection.h"

#include "ShaderDefine.h"

// cso�t�@�C���������Ă���t�H���_�܂ł̃p�X��
constexpr const char* FILE_FOLDER = "assets/Shader/";

void ShaderCollection::LoadFromCSO()
{
	LoadVS();
	
	LoadPS();
}

void ShaderCollection::LoadVS()
{
	// ���_�V�F�[�_�[���쐬
	// cso�t�@�C���̖��O
	std::vector<std::string> vFileNames
	{
		"VS_Gouraud.cso",
		"VS_Primitive.cso",
		"VS_SkinAnimation.cso"
	};

	// �Z�b�g���閼�O
	std::vector<std::string> vShaderNames
	{
		"VS_Gouraud",
		"VS_Primitive",
		"VS_SkinAnimation"
	};
	defaultVS = vShaderNames[0];

	// ���_�V�F�[�_�[�ǂݍ��ݔz��
	std::vector<std::unique_ptr<VertexShader>> pVsShaderList;

	// �O���[
	std::unique_ptr<VertexShader> pVShader = std::make_unique<VS_Gouraud>();
	pVsShaderList.push_back(std::move(pVShader));

	pVShader = std::make_unique<VS_Primitive>();
	pVsShaderList.push_back(std::move(pVShader));

	pVShader = std::make_unique<VS_SkinnedAnimation>();
	pVsShaderList.push_back(std::move(pVShader));

	for (int vsCount = 0; vsCount < static_cast<int>(pVsShaderList.size()); vsCount++)
	{
		// ���[�h����S�̂̃p�X��
		std::string loadFilePath = FILE_FOLDER + vFileNames[vsCount];
		pVsShaderList[vsCount]->LoadCsoFile(loadFilePath.c_str());
		pVsShaderList[vsCount]->SetName(vShaderNames[vsCount]);

		// �z��ɒǉ�����
		pShaders.emplace(vShaderNames[vsCount], std::move(pVsShaderList[vsCount]));
	}
}

void ShaderCollection::LoadPS()
{
	// �s�N�Z���V�F�[�_�[���쐬
	// cso�t�@�C���̖��O
	std::vector<std::string> pFileNames
	{
		"PS_TexColor.cso",
		"PS_Unlit.cso",
		"PS_Primitive.cso"
	};
	// �Z�b�g���閼�O
	std::vector<std::string> pShaderNames
	{
		"PS_TexColor",
		"PS_Unlit",
		"PS_Primitive"
	};
	defaultPS = pShaderNames[0];


	std::vector<std::unique_ptr<PixelShader>> pPsShaderList;

	// �e�N�X�`��
	std::unique_ptr<PixelShader> pPShader = std::make_unique<PS_TexColor>();
	pPsShaderList.push_back(std::move(pPShader));

	// �A�����b�g
	pPShader = std::make_unique<PS_Unlit>();
	pPsShaderList.push_back(std::move(pPShader));

	pPShader = std::make_unique<PixelShader>();
	pPsShaderList.push_back(std::move(pPShader));

	for (int psCount = 0; psCount < static_cast<int>(pPsShaderList.size()); psCount++)
	{
		// ���[�h����S�̂̃p�X��
		std::string loadFilePath = FILE_FOLDER + pFileNames[psCount];
		pPsShaderList[psCount]->LoadCsoFile(loadFilePath.c_str());
		pPsShaderList[psCount]->SetName((pShaderNames[psCount]));

		// �z��ɒǉ�����
		pShaders.emplace(pShaderNames[psCount], std::move(pPsShaderList[psCount]));
	}
}

void ShaderCollection::Init()
{
	LoadFromCSO();	// cso�t�@�C�������[�h����
}

void ShaderCollection::UniqueUpdateBuffer()
{
	// �I�u�W�F�N�g���ƂɈˑ����Ȃ��o�b�t�@�X�V
	for (auto& shader : pShaders)
	{
		shader.second->OnceUpdateBuffer();
	}
}

VertexShader* ShaderCollection::GetVertexShader(const std::string& _shaderName)
{
	auto itr = pShaders.find(_shaderName);
	if (itr == pShaders.end())	// �V�F�[�_�[���Ȃ����
	{
		HASHI_DEBUG_LOG(_shaderName + " ��������܂���ł���");
		return nullptr;
	}

	VertexShader* retPtr = dynamic_cast<VertexShader*>(itr->second.get());
	if (retPtr == nullptr)
	{
		HASHI_DEBUG_LOG(_shaderName + "�w�肵���V�F�[�_�[���͒��_�V�F�[�_�[�ł͂���܂���");
		return nullptr;
	}

	return retPtr;
}

PixelShader* ShaderCollection::GetPixelShader(const std::string& _shaderName)
{
	auto itr = pShaders.find(_shaderName);
	if (itr == pShaders.end())	// �������O�̃V�F�[�_�[����������
	{
		HASHI_DEBUG_LOG(_shaderName + " ��������܂���ł���");
		return nullptr;
	}

	PixelShader* retPtr = dynamic_cast<PixelShader*>(itr->second.get());
#ifdef EDIT
	if (retPtr == nullptr)
	{

		HASHI_DEBUG_LOG(_shaderName + "�w�肵���V�F�[�_�[�̓s�N�Z���V�F�[�_�[�ł͂���܂���");
		return nullptr;
	}
#endif

	return retPtr;
}


