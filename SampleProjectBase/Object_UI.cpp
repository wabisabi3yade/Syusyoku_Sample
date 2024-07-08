#include "pch.h"
#include "Object_UI.h"
#include "ShaderCollection.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Material* Object_UI::pShareMaterial = nullptr;

void Object_UI::MakePoligon()
{
	// �e�N�X�`�����͂�|���S���𐶐�����(TRIANGLESTRIP)
	// ���_���W
	constexpr int vertexNum = 4;
	Vertex* pVerticies = new Vertex[vertexNum];
	pVerticies[0].position = Vector3(100, 100, 0);
	pVerticies[1].position = Vector3(200, 100, 0);
	pVerticies[2].position = Vector3(100, 300, 0);
	pVerticies[3].position = Vector3(200, 300, 0);

	// uv���W
	pVerticies[0].uv = { 0.0f, 0.0f };	// ����
	pVerticies[1].uv = { 1.0f / 3.0f, 0.0f };	// �E��
	pVerticies[2].uv = { 0.0f, 1.0f / 4.0f };	// ����
	pVerticies[3].uv = { 1.0f / 3.0f, 1.0f / 4.0f };	// �E��

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
	poligon = std::make_unique<Mesh>();	// ���b�V���쐬
	poligon->SetVertexData(pVerticies, vertexNum, pIndicies, indexNum);

}

Object_UI::Object_UI() : pTexture(nullptr)
{
	MakePoligon();

	if (pShareMaterial == nullptr)	// ���ʂɃ}�e���A�����ݒ肳��Ă��Ȃ��Ȃ�
	{
		MakeMaterial();	// �}�e���A�����쐬����
	}
	
	layer.SetType(Layer::Type::UI);	// UI���C���[�ɕς���
}

Object_UI::~Object_UI()
{
}

void Object_UI::MakeMaterial()
{
	// �}�e���A���𐶐�
	std::unique_ptr<Material> pMakeMaterial = std::make_unique<Material>();

	// �V�F�[�_�[��ݒ�
	ShaderCollection* shCol = ShaderCollection::GetInstance();
	pMakeMaterial->SetVertexShader(shCol->GetVertexShader("VS_UVScroll"));
	pMakeMaterial->SetPixelShader(shCol->GetPixelShader("PixelShader"));
	pShareMaterial = pMakeMaterial.get();	// �}�e���A�������炤

	// ����
	ResourceCollection::GetInstance()->SetResource<Material>("M_ObjectUI", std::move(pMakeMaterial));
}

void Object_UI::Update()
{
}

void Object_UI::SetupDraw()
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	// �g�|���W�[��ݒ�
	renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// WVP��W��V��P�ʍs��ɂ���
	// �ϊ��s����쐬
	// �ړ��s��
	Matrix t = XMMatrixTranslation(
		transform.position.x, 
		transform.position.y, 
		transform.position.z
	);

	// �X�P�[�����O�s��
	Matrix s = XMMatrixScaling(
		transform.scale.x,
		transform.scale.y,
		transform.scale.z
	);

	// ��]�s��
	Matrix r = Matrix::CreateFromYawPitchRoll(
		transform.rotation.y * Mathf::degToRad,
		transform.rotation.x * Mathf::degToRad,
		transform.rotation.z * Mathf::degToRad
	);

	Matrix worldMtx = s * r * t;	// ���[���h�ϊ��s����쐬
	worldMtx.Transpose();

	RenderParam::WVP wvp;
	wvp.world = worldMtx;
	wvp.view = Matrix::Identity;
	wvp.view = wvp.view.Transpose();

	pShareMaterial->GetVertexShader().UpdateBuffer(0, &wvp);	// WVP�s���n��
	pShareMaterial->GetPixelShader().SetTexture(0, pTexture);	// �V�F�[�_�[�Ƀe�N�X�`���Z�b�g

	// �V�F�[�_�[���Œ�
	pShareMaterial->GetVertexShader().Bind();
	pShareMaterial->GetPixelShader().Bind();
}

void Object_UI::LateUpdate()
{
}

void Object_UI::Draw()
{
	SetupDraw();
	poligon->Draw();
}
