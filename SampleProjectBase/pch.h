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

// //ImGui��\�������`
//#define EDIT_DISPLAY
#if defined(_DEBUG) || defined(EDIT_DISPLAY)
#define EDIT
#endif

// �C���N���[�h�t�@�C�� /////////////////
#include <iostream>	// std�g�����߂�
#include <fstream>
#include <random>	// ����
#include <algorithm>
#include <string>
#include <unordered_map>	// ���Ԃ��C�ɂ��Ȃ�map�z��
#include <unordered_set>	// ����L�[���i�[�ł��Ȃ��z��
#include <typeinfo>	// �^�����擾
#include <filesystem>	// �p�X��
#include <vector>	// ���I�z��
#include <variant>	// variant�^
#include <numbers>	// ���w
#include <limits>
#include <d3d11.h>	// DX11�̋@�\
#include <wrl.h>	// ComPtr
#pragma comment(lib, "d3d11.lib")

// �O�����C�u����
#include <DirectXMath.h>	// ���w�n�̋@�\
#include <SimpleMath.h>	// DirectXTK�̐��w�n
#include <Audio.h>	// XAudio2��XTK�Ń��b�v�����t�@�C��
#pragma comment(lib, "DirectXTK.lib")	// DirectXTK���C�u����
#include <nlohmann/json.hpp>	// json�����o��
#include <magic_enum.hpp>
#include <Effekseer.h>	// �G�t�F�N�V�A
#include <EffekseerRendererDX11.h>	// �G�t�F�N�V�ADx11�Ή�

// ImGui
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
// ImPlot
#include <implot.h>

// ���O��Ԓ�`
namespace HashiTaku
{
	namespace DX = DirectX;
	namespace DXSimp = DirectX::SimpleMath;
	using json = nlohmann::json;
}

// ���O�N���X
#include "IClassNameGetter.h"
#include "MathUseful.h"	// ���쐔�w�֗��֐�
#include "ImGuiMethod.h"	// ImGui�֗��֐�
#include "ImGuiDebugLog.h"	// �f�o�b�O���O
#include "IImGuiUser.h"	// ImGui���g�p����N���X�̃C���^�[�t�F�[�X
#include "Transform.h"	// ���W�E��]�E�X�P�[���̃N���X
#include "MainApplication.h"
#include "HashiTaku_Random.h"	// �����N���X
#include "Direct3D11.h"	// ���O��DirectX11
#include "DX11BulletPhisics.h"	// DX11�����G���W��
#include "DX11EffecseerManager.h"	// �G�t�F�N�g�}�l�W���[
#include "DXSoundManager.h"	// �T�E���h�}�l�W���[