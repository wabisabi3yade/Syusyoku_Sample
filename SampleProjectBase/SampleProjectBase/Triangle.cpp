#include "pch.h"
#include "Triangle.h"
#include "D3D11_Renderer.h"

Triangle::Triangle()
{
	// ���W
	auto sqrt3 = sqrtf(3);
	vertices[0].position = { 0.f, sqrt3 / 3.f, 0.f };
	vertices[1].position = { 0.5f, -sqrt3 / 6.f, 0.f };
	vertices[2].position = { -0.5f, -sqrt3 / 6.f, 0.f };

	// �F
	constexpr float ALPHA = 0.5f;
	vertices[0].color = { 1.f, 0.f, 0.f, ALPHA };
	vertices[1].color = { 0.f, 1.f, 0.f, ALPHA };
	vertices[2].color = { 0.f, 0.f, 1.f, ALPHA };
}

Triangle::~Triangle()
{
	SAFE_RELEASE(pVertexBuffer);
}

bool Triangle::CreateVertexBuffer(D3D11_Renderer& _renderer)
{
	// ���_�o�b�t�@��`�̍\���̂�ݒ�
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 3;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// �f�[�^�{�̂ł���T�u���\�[�X��`�𒸓_�f�[�^�ŏ�����
	// �T�u���\�[�X��` �� DirectX�ł̓o�b�t�@���\�[�X���쐬����Ƃ��A�K���T�u���\�[�X��`�����B
	D3D11_SUBRESOURCE_DATA vertexSubData;
	vertexSubData.pSysMem = vertices;

	// ���_�o�b�t�@���擾
	auto hr = _renderer.GetDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexSubData,
		&pVertexBuffer
	);

	if (FAILED(hr)) return false;

	return true;
}

void Triangle::SetupTransform(D3D11_Renderer& _renderer)
{
	CbTransformSet& cb = _renderer.GetParameter().cbTransformSet;

	auto mtx = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0);

	DirectX::XMStoreFloat4x4(&cb.data.transform, XMMatrixTranspose(mtx));
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	//// CBuffer�ɂЂ��Â��n�[�h�E�F�A���\�[�X�}�b�v�擾�i���b�N���Ď擾�j
	auto pDeviceContext = _renderer.GetDeviceContext();

	HRESULT hr = pDeviceContext->Map(
		cb.pBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource);
	if (FAILED(hr)) {
		//DXTRACE_ERR(L"DrawSceneGraph failed", hr);
		return;
	}

	CopyMemory(mappedResource.pData, &cb.data, sizeof(cb.data));
	// �}�b�v����
	pDeviceContext->Unmap(cb.pBuffer, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &cb.pBuffer);

}

void Triangle::Draw(D3D11_Renderer& _renderer)
{
	auto pDeviceContext = _renderer.GetDeviceContext();
	// Draw���钸�_�o�b�t�@�i���f���j���w�肷��
	uint32_t strides = sizeof(Vertex);   // ���_�̃T�C�Y
	uint32_t offsets = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &strides, &offsets);
	// �ϊ��s��
	SetupTransform(_renderer);

	// �����ɒ��_���A�ŏ��̒��_�̃C���f�b�N�X��������
	pDeviceContext->Draw(VERTEX_NUM, 0);
}
