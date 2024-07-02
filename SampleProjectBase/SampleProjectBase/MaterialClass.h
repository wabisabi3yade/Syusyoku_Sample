#pragma once
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"


struct MaterialParameter
{
	DirectX::SimpleMath::Color diffuse;	// �F
	DirectX::SimpleMath::Color ambient;	// ����
	DirectX::SimpleMath::Color specular;	// ���ʔ���
	DirectX::SimpleMath::Color emissive;	// ������
	float shininess;	// ����
};

// ���f���̕\�ʃf�[�^
class MaterialClass
{
	MaterialParameter parameter;	// �}�e���A���̃p�����[�^

	// �g�p����e�V�F�[�_�[
	VertexShader* pVertexShader;	// ���_	
	PixelShader* pPixelShader;	// �s�N�Z��
public:
	MaterialClass();
	~MaterialClass();

	// �V�F�[�_�[��ݒ�
	void SetVertexShader(VertexShader* _vs) { pVertexShader = _vs; }
	void SetPixelShader(PixelShader* _ps) { pPixelShader = _ps; }
	
	// �}�e���A���E�e�p�����[�^��ݒ�
	void SetMaterialParameter(const MaterialParameter& _mat) { parameter = _mat; }
	void SetDiffuse(const DirectX::SimpleMath::Color& _diffuse) { parameter.diffuse = _diffuse; }
	void SetAmbeint(const DirectX::SimpleMath::Color& _ambient) { parameter.ambient = _ambient; }
	void SetSpecular(const DirectX::SimpleMath::Color& _specular) { parameter.specular = _specular; }
	void SetEmissive(const DirectX::SimpleMath::Color& _emmisive) { parameter.emissive = _emmisive; }

	VertexShader& GetVertexShader()const { return *pVertexShader; }
	PixelShader& GetPixelShader()const { return *pPixelShader; }
	MaterialParameter& GetMaterialParameter() { return parameter; }
};

