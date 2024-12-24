#pragma once
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace HashiTaku
{
	struct MaterialParameter
	{
		/// @brief �F
		DirectX::SimpleMath::Color diffuse;

		/// @brief ����
		DirectX::SimpleMath::Color ambient;

		/// @brief ���ʔ���
		DirectX::SimpleMath::Color specular;

		/// @brief ������
		DirectX::SimpleMath::Color emissive;

		/// @brief ����
		float shiness;

		/// @brief �e�N�X�`���g�p�t���O
		int isTextureEnable;

		float dummy1, dummy2;
	};

	// ���f���̕\�ʃf�[�^
	class Material : public Asset_Base, public IImGuiUser
	{
		/// @brief �p�����[�^
		MaterialParameter parameter;

		/// @brief �e�N�X�`��
		Texture* pDiffuseTexture;

		/// @brief �@���e�N�X�`��
		Texture* pNormalTexture;

		/// @brief ���_�V�F�[�_�[
		VertexShader* pVertexShader;

		/// @brief �s�N�Z���V�F�[�_�[
		PixelShader* pPixelShader;
	public:
		Material();
		Material(const std::string& _vsName, const std::string& _psName);
		~Material();

		/// @brief �`�揀��
		/// @param _wvp wvp�s��
		void DrawSetup(RenderParam::WVP& _wvp);

		// �}�e���A���E�e�p�����[�^���Z�b�g
		void SetMaterialParameter(const MaterialParameter& _mat) { parameter = _mat; }
		void SetDiffuse(const DirectX::SimpleMath::Color& _diffuse) { parameter.diffuse = _diffuse; }
		void SetAmbeint(const DirectX::SimpleMath::Color& _ambient) { parameter.ambient = _ambient; }
		void SetSpecular(const DirectX::SimpleMath::Color& _specular) { parameter.specular = _specular; }
		void SetEmissive(const DirectX::SimpleMath::Color& _emmisive) { parameter.emissive = _emmisive; }
		void SetShiness(float _shiness) { parameter.shiness = _shiness; }
		void SetTextureEnable(bool _isTextureEnable) { parameter.isTextureEnable = _isTextureEnable; }

		// �e�e�N�X�`�����Z�b�g
		void SetDiffuseTexture(Texture& _diffuseTex);
		void SetNormalTexture(Texture& _normalTex) { pNormalTexture = &_normalTex; }

		// ���_�E�s�N�Z���V�F�[�_�[���Z�b�g
		void SetVertexShader(const std::string& _vsName);
		void SetVertexShader(VertexShader& _vsShader);
		void SetPixelShader(const std::string& _psName);
		void SetPixelShader(PixelShader& _psShader);

		VertexShader& GetVertexShader()const { return *pVertexShader; }
		PixelShader& GetPixelShader()const { return *pPixelShader; }

		MaterialParameter& GetMaterialParameter() { return parameter; }

		Texture* GetDiffuseTexture()const { return pDiffuseTexture; }
		Texture* GetNormalTexture()const { return pNormalTexture; }

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief �}�e���A���̃p�����[�^������
		void InitParameter();

		/// @brief �V�F�[�_�[�̏���
		void ShaderSetup(Shader& _shader, RenderParam::WVP& _wvp);

		void ImGuiDebug() override;
	};
}