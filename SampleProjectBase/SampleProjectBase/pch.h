#pragma once

// �v���R���p�C���ς݃w�b�_�[
// �悭�g���C���N���[�h�t�@�C�����܂Ƃ߁A���Ԃƃ��\�[�X��ߖ�

// �C���N���[�h�t�@�C�� /////////////////
#include <d3d11.h>	// DX11�̋@�\
#pragma comment(lib, "d3d11.lib")	// ���C�u����
#include <DirectXMath.h>	// ���w�n�̋@�\
#include <iostream>	// std�g�����߂�
#include <vector>	// ���I�z��
#include <unordered_map>	// ���Ԃ��C�ɂ��Ȃ�map�z��
#include "Float2.h"	// float�^�ϐ�2�N���X
#include "Float3.h"	// float�^�ϐ�3�N���X
#include "Float4.h"	// float�^�ϐ�3�N���X
#include "Transform.h"	// ���W�E��]�E�X�P�[���̃N���X

#include "MessageError.h"	// �G���[�e�L�X�g���o��

// �N���X�O���錾
class D3D11_Renderer;	// Direct3D�̕`��N���X

// �}�N����` /////////////////
// ���\�[�X���������}�N��
#define SAFE_RELEASE(x)    { if(x) { (x)->Release(); (x) = nullptr; } }
// �N���X�̈��S�ɉ������}�N����`
#define CLASS_DELETE(p) {if(p != nullptr) { delete p; p = nullptr;} }

// �\���� /////////////////
// ���_���W
struct Vertex
{
	// ���W
	Float3 position = {};
	// �F(RGBA)
	Float4 color = {};

	// UV���W�i�e�N�X�`�����W�j
	DirectX::XMFLOAT2 uv = {};
	// ���_�̖@���x�N�g��
	Float3 normal = {};
};


