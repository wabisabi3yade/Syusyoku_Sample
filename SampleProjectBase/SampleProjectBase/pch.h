#pragma once
// �v���R���p�C���ς݃w�b�_�[
// �悭�g���C���N���[�h�t�@�C�����܂Ƃ߁A���Ԃƃ��\�[�X��ߖ�

// �}�N����` /////////////////
// ���\�[�X���������}�N��
#define SAFE_RELEASE(x)    { if(x) { (x)->Release(); (x) = nullptr; } }

// �N���X�̈��S�ɉ������}�N����`
#define CLASS_DELETE(p) {if(p != nullptr) { delete p; p = nullptr;} }

// max min�֐��𖳌��ɂ���
#define NOMINMAX

// �C���N���[�h�t�@�C�� /////////////////
#include <iostream>	// std�g�����߂�
#include <algorithm>
#include <string>
#include <unordered_map>	// ���Ԃ��C�ɂ��Ȃ�map�z��
#include <typeinfo>	// �^�����擾
#include <vector>	// ���I�z��
#include <d3d11.h>	// DX11�̋@�\
#pragma comment(lib, "d3d11.lib")	// ���C�u����

// �O�����C�u����
#include <DirectXMath.h>	// ���w�n�̋@�\
#include <SimpleMath.h>	// DirectXTK�̐��w�n
#pragma comment(lib, "DirectXTK.lib")	// DirectXTK���C�u����
#include <nlohmann/json.hpp>	// json�����o��

// ���O�N���X
#include "MathUseful.h"	// ���쐔�w�֗��֐�
#include "ImGuiMethod.h"	// ImGui�֗��֐�
#include "Transform.h"	// ���W�E��]�E�X�P�[���̃N���X
#include "MainApplication.h"
#include "Direct3D11.h"	// ���O��DirectX11�N���X
#include "ResourceCollection.h"	// ���\�[�X���Ǘ�����N���X


// �\���� /////////////////
// ���_���W
struct Vertex
{
	// ���W
	DirectX::SimpleMath::Vector3 position = {};
	// �F(RGBA)
	DirectX::SimpleMath::Color color = {};

	// UV���W�i�e�N�X�`�����W�j
	DirectX::SimpleMath::Vector2 uv = {};
	// ���_�̖@���x�N�g��
	DirectX::SimpleMath::Vector3 normal = {};
};


