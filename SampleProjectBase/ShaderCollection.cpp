#include "ShaderCollection.h"
#include "ShaderDefine.h"

namespace HashiTaku
{
	std::vector<const std::string*> ShaderCollection::GetVSNameList()
	{
		std::vector<const std::string*> nameList;

		// ���_�V�F�[�_�[�̖��O���擾
		for (auto& shader : shaderList)
		{
			if (shader.second->GetShaderType() == Shader::Type::Vertex)
				nameList.push_back(&shader.first);
		}

		return nameList;
	}

	std::vector<const std::string*> ShaderCollection::GetPSNameList()
	{
		std::vector<const std::string*> nameList;

		// ���_�V�F�[�_�[�̖��O���擾
		for (auto& shader : shaderList)
		{
			if (shader.second->GetShaderType() == Shader::Type::Pixel)
				nameList.push_back(&shader.first);
		}

		return nameList;
	}

	void ShaderCollection::LoadFromCSO()
	{
		// ���_�V�F�[�_�[
		LoadShader<VS_Object>("VS_Object.cso");
		LoadShader<VS_Gouraud>("VS_Gouraud.cso");
		LoadShader<VS_Primitive>("VS_Primitive.cso");
		LoadShader<VS_SkinnedAnimation>("VS_SkinAnimation.cso");
		LoadShader<VertexShader>("VS_Line.cso");
		LoadShader<VS_WorldPosition>("VS_WorldPosition.cso");
		LoadShader<VertexShader>("VS_Sprite.cso");
		LoadShader<VertexShader>("VS_UI.cso");
		LoadShader<VertexShader>("VS_OutLine.cso");
		LoadShader<VS_WorldPosition>("VS_BackObject.cso");
		defaultVS = "VS_WorldPosition";

		// �s�N�Z���V�F�[�_�[
		LoadShader<PS_Toon>("PS_Toon.cso");
		LoadShader<PS_TexColor>("PS_TexColor.cso");
		LoadShader<PS_Unlit>("PS_Unlit.cso");
		LoadShader<PixelShader>("PS_Primitive.cso");
		LoadShader<PixelShader>("PS_Line.cso");
		LoadShader<PixelShader>("PS_UI.cso");
		LoadShader<PixelShader>("PS_OutLine.cso");
		LoadShader<PS_BackObject>("PS_BackObject.cso");
		defaultPS = "PS_Unlit";
	}

	void ShaderCollection::Init()
	{
		LoadFromCSO();	// cso�t�@�C�������[�h����
	}

	void ShaderCollection::InitAssets()
	{
		for (auto& shader : shaderList)
		{
			shader.second->InitAssets();
		}
	}

	void ShaderCollection::UniqueUpdateBuffer()
	{
		// �I�u�W�F�N�g���ƂɈˑ����Ȃ��o�b�t�@�X�V
		for (auto& shader : shaderList)
		{
			shader.second->OnceUpdateBuffer();
		}
	}

	VertexShader* ShaderCollection::GetVertexShader(const std::string& _shaderName)
	{
		auto itr = shaderList.find(_shaderName);
		if (itr == shaderList.end())	// �V�F�[�_�[���Ȃ����
		{
			HASHI_DEBUG_LOG(_shaderName + " ��������܂���ł���");
			return nullptr;
		}

		VertexShader* retPtr = dynamic_cast<VertexShader*>(itr->second.get());
#ifdef EDIT
		if (retPtr == nullptr)
		{
			HASHI_DEBUG_LOG(_shaderName + "�w�肵���V�F�[�_�[���͒��_�V�F�[�_�[�ł͂���܂���");
			return nullptr;
		}
#endif

		return retPtr;
	}

	PixelShader* ShaderCollection::GetPixelShader(const std::string& _shaderName)
	{
		auto itr = shaderList.find(_shaderName);
		if (itr == shaderList.end())	// �������O�̃V�F�[�_�[����������
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
}