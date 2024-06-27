#include "pch.h"
#include "Object_2D.h"
#include "ShaderCollection.h"

using namespace DirectX::SimpleMath;

void Object_2D::MaterialSetting()
{
	// 2D�p�}�e���A�����擾����
	const std::string MATERIAL_NAME = "M_Object_2D";
	ResourceCollection* reCollect = ResourceCollection::GetInstance();
	pMaterial = reCollect->GetResource<MaterialClass>(MATERIAL_NAME);

	if (pMaterial == nullptr)	// �}�e���A��������������쐬
	{
		std::unique_ptr<MaterialClass> setMaterial = std::make_unique<MaterialClass>();
		ShaderCollection* shCol = ShaderCollection::GetInstance();

		// �g�p����V�F�[�_�[���}�e���A���ɐݒ�
		setMaterial->SetVertexShader(shCol->GetVertexShader(shCol->defaultVS));
		setMaterial->SetPixelShader(shCol->GetPixelShader(shCol->defaultPS));

		pMaterial = setMaterial.get();	// �|�C���^���󂯎��
		// ���\�[�X�Ǘ���2D�p�}�e���A�����Z�b�g����
		reCollect->SetResource<MaterialClass>(MATERIAL_NAME, std::move(setMaterial));
	}

}
void Object_2D::MakeVerticies()
{
	// �e�N�X�`�����͂�|���S���𐶐�����(TRIANGLESTRIP)
	// ���_���W
	constexpr int vertexNum = 4;
	Vertex* pVerticies = new Vertex[vertexNum];
	pVerticies[0].position = { -0.5f, 0.5f, 0.0f };	// ����
	pVerticies[1].position = { 0.5f, 0.5f, 0.0f };	// �E��
	pVerticies[2].position = { -0.5f, -0.5f, 0.0f };	// ����
	pVerticies[3].position = { 0.5f, -0.5f, 0.0f };	// �E��

	// uv���W
	pVerticies[0].uv = { 0.0f, 0.0f };	// ����
	pVerticies[1].uv = { 1.0f, 0.0f };	// �E��
	pVerticies[2].uv = { 0.0f, 1.0f };	// ����
	pVerticies[3].uv = { 1.0f, 1.0f };	// �E��

	// �C���f�b�N�X
	constexpr int indexNum = 4;
	u_int* pIndicies = new u_int[indexNum];
	pIndicies[0] = 0;
	pIndicies[1] = 1;
	pIndicies[2] = 2;
	pIndicies[3] = 3;

	// �F�A���_
	for (int i = 0; i < vertexNum; i++)
	{
		pVerticies[i].color = Vector4::One;	// �F
		pVerticies[i].normal = Vector3(0.0f, 0.0f, -1.0f);
	}

	// �l�p�|���S��
	pSquareMesh = std::make_unique<Mesh>();
	pSquareMesh->SetVertexData(pVerticies, vertexNum, pIndicies, indexNum);
}

Object_2D::Object_2D() : pTexture(nullptr), pMaterial(nullptr)
{
	MaterialSetting();
	MakeVerticies();
}

Object_2D::~Object_2D()
{
}

void Object_2D::Draw()
{
	D3D11_Renderer* renederer = Direct3D11::GetInstance()->GetRenderer();
	DrawSetup(*renederer);	// �`�揀��

	// ���b�V���`��
	pSquareMesh->Draw(*renederer);
}

void Object_2D::DrawSetup(D3D11_Renderer& _renderer)
{
	// �g�|���W�[��ݒ�
	Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���[���h�ϊ��s������߂�
	Matrix worldMatrix = _renderer.GetWorldMtx(transform);
	RenderParam::WVP wvp = _renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	// �}�e���A���̃p�����[�^���擾
	MaterialParameter& material = pMaterial->GetMaterialParameter();
	// ���_�V�F�[�_�[
	VertexShader& vtxSh = pMaterial->GetVertexShader();
	vtxSh.UpdateBuffer(0, &wvp);	// wvp�s��𑗂�
	vtxSh.UpdateBuffer(1, &material);	// �}�e���A�����𑗂�

	// �s�N�Z���V�F�[�_�[
	if (material.isTextureEnable)
		pMaterial->GetPixelShader().SetTexture(0, pTexture);	//�e�N�X�`���𑗂�
	pMaterial->GetPixelShader().UpdateBuffer(1, &material);	// �}�e���A�����𑗂�

	// �V�F�[�_�[���Œ�
	pMaterial->GetVertexShader().Bind();
	pMaterial->GetPixelShader().Bind();
}

void Object_2D::SetTexture(Texture* _texture)
{
	pMaterial->GetMaterialParameter().isTextureEnable = true;
	pTexture = _texture;
}


//void Object_2D::SetSizeWidth(float _width)
//{
//	size.x = _width;
//	MakeVerticies();
//}
//
//void Object_2D::SetSizeHeight(float _height)
//{
//	size.y = _height;
//	MakeVerticies();
//}
//
//void Object_2D::SetSize(const DirectX::SimpleMath::Vector2& _size)
//{
//	size = _size;
//	MakeVerticies();
//}
