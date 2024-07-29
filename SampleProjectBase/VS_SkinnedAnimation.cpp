#include "pch.h"
#include "VS_SkinnedAnimation.h"


#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

void VS_SkinnedAnimation::MakeInputLayout(const char* _pData, u_int _dataSize)
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	ID3D11Device* pDevice = renderer->GetDevice();

	//// ���̓��C�A�E�g�I�u�W�F�N�g�쐬
	//// ���@���_�V�F�[�_�[�ɂ͂ǂ��������f�[�^�\���̒��_�f�[�^���n���Ă��邩���`�������̓��C�A�E�g
	//// Vertex�\���̂̃����o�ϐ��̃T�C�Y���w�肵�āA�V�F�[�_�[�̉��̏��Ȃ̂��𔻕ʂ���
	//D3D11_INPUT_ELEMENT_DESC layout[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "BONEINDEX",	0, DXGI_FORMAT_R32G32B32A32_SINT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "BONEWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};

	//HRESULT hr = pDevice->CreateInputLayout(
	//	layout,
	//	_countof(layout),
	//	_pData,
	//	_dataSize,
	//	&pInputLayout
	//);

	//if (FAILED(hr))
	//{
	//	HASHI_DEBUG_LOG("���s");
	//	return;
	//}

	/*
	�V�F�[�_�쐬���ɃV�F�[�_���t���N�V������ʂ��ăC���v�b�g���C�A�E�g���擾
	�Z�}���e�B�N�X�̔z�u�Ȃǂ��环�ʎq���쐬
	���ʎq���o�^�ρ��ė��p�A�Ȃ���ΐV�K�쐬
	https://blog.techlab-xe.net/dxc-shader-reflection/
	*/

	ID3D11ShaderReflection* pReflection;
	D3D11_SHADER_DESC shaderDesc;
	D3D11_INPUT_ELEMENT_DESC* pInputDesc;
	D3D11_SIGNATURE_PARAMETER_DESC sigDesc;

	DXGI_FORMAT formats[][4] =
	{
		{
			DXGI_FORMAT_R32_UINT,
			DXGI_FORMAT_R32G32_UINT,
			DXGI_FORMAT_R32G32B32_UINT,
			DXGI_FORMAT_R32G32B32A32_UINT,
		}, {
			DXGI_FORMAT_R32_SINT,
			DXGI_FORMAT_R32G32_SINT,
			DXGI_FORMAT_R32G32B32_SINT,
			DXGI_FORMAT_R32G32B32A32_SINT,
		}, {
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
		},
	};

	HRESULT hr = D3DReflect(_pData, _dataSize, IID_PPV_ARGS(&pReflection));
	if (FAILED(hr)) { return ; }

	pReflection->GetDesc(&shaderDesc);
	pInputDesc = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];
	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		pReflection->GetInputParameterDesc(i, &sigDesc);
		pInputDesc[i].SemanticName = sigDesc.SemanticName;
		pInputDesc[i].SemanticIndex = sigDesc.SemanticIndex;

		// http://marupeke296.com/TIPS_No17_Bit.html
		BYTE elementCount = sigDesc.Mask;
		elementCount = (elementCount & 0x05) + ((elementCount >> 1) & 0x05);
		elementCount = (elementCount & 0x03) + ((elementCount >> 2) & 0x03);

		switch (sigDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			pInputDesc[i].Format = formats[0][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			pInputDesc[i].Format = formats[1][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			pInputDesc[i].Format = formats[2][elementCount - 1];
			break;
		}
		pInputDesc[i].InputSlot = 0;
		pInputDesc[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
		pInputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputDesc[i].InstanceDataStepRate = 0;
	}

	hr = pDevice->CreateInputLayout(
		pInputDesc, shaderDesc.InputParameters,
		_pData, _dataSize, &pInputLayout
	);

	delete[] pInputDesc;
}

VS_SkinnedAnimation::VS_SkinnedAnimation()
{
	bufferTypes.resize(2);
	bufferTypes[0] = BufferType::WVP;
	bufferTypes[1] = BufferType::BoneMatricies;
}

