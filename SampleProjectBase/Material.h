#pragma once
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"


struct MaterialParameter
{
	// �F
	DirectX::SimpleMath::Color diffuse;	

	// ����
	DirectX::SimpleMath::Color ambient;

	// ���ʔ���
	DirectX::SimpleMath::Color specular;	

	// ������
	DirectX::SimpleMath::Color emissive;	

	// ����
	float shininess;

	// �e�N�X�`���g�p�t���O
	int isTextureEnable;	
};

// ���f���̕\�ʃf�[�^
class Material
{
	// �}�e���A���̃p�����[�^
	MaterialParameter parameter;	

	// �F�e�N�X�`��
	Texture* pDiffuseTexture;	
	// �@���e�N�X�`��
	Texture* pNormalTexture;

	// �g�p����e�V�F�[�_�[
	// ���_	
	VertexShader* pVertexShader;
	// �s�N�Z��
	PixelShader* pPixelShader;	
public:
	Material();
	~Material();

	// �V�F�[�_�[��ݒ�
	void SetVertexShader(VertexShader* _vs) { pVertexShader = _vs; }
	void SetPixelShader(PixelShader* _ps) { pPixelShader = _ps; }
	
	// �}�e���A���E�e�p�����[�^���Z�b�g
	void SetMaterialParameter(const MaterialParameter& _mat) { parameter = _mat; }
	void SetDiffuse(const DirectX::SimpleMath::Color& _diffuse) { parameter.diffuse = _diffuse; }
	void SetAmbeint(const DirectX::SimpleMath::Color& _ambient) { parameter.ambient = _ambient; }
	void SetSpecular(const DirectX::SimpleMath::Color& _specular) { parameter.specular = _specular; }
	void SetEmissive(const DirectX::SimpleMath::Color& _emmisive) { parameter.emissive = _emmisive; }
	void SetTextureEnable(bool _isTextureEnable) { parameter.isTextureEnable = _isTextureEnable; }

	// �e�e�N�X�`�����Z�b�g
	void SetDiffuseTexture(Texture& _diffuseTex);
	void SetNormalTexture(Texture& _normalTex) { pNormalTexture = &_normalTex; }

	VertexShader& GetVertexShader()const { return *pVertexShader; }
	PixelShader& GetPixelShader()const { return *pPixelShader; }

	MaterialParameter& GetMaterialParameter() { return parameter; }

	Texture& GetDiffuseTexture()const { return *pDiffuseTexture; }
	Texture& GetNormalTexture()const { return *pNormalTexture; }

	void ImGuiSetting();
};

