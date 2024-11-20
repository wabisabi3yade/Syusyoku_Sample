#pragma once
#include "Singleton_Base.h"

class InSceneSystemManager;

/// @brief DX11���ŏ����������Ȃǂ��s���N���X
class DX11EffecseerManager : public Singleton_Base<DX11EffecseerManager>
{
	friend class Singleton_Base<DX11EffecseerManager>;

	/// @brief �G�t�F�N�V�A�̕`��ϐ�
	EffekseerRendererDX11::RendererRef renderer;

	/// @brief �}�l�W���[�ϐ�
	Effekseer::ManagerRef manager;

	/// @brief D3D�`��N���X
	D3D11_Renderer* pRenderer;
public:
	/// @brief �������֐�
	void Init();

	/// @brief �`��J�n�����i�J�����s��X�V�O�j
	void BeginDraw();

	/// @brief �`�悷��
	void EffectDraw();

	/// @brief �X�V����
	void Update();

	/// @brief �Đ����̃G�t�F�N�g���폜
	void AllEffectDestroy();

	/// @brief �G�t�F�N�g���Đ�����
	/// @param _effect �Đ�����G�t�F�N�g
	/// @return �Đ����Ă���n���h��
	Effekseer::Handle Play(
		const Effekseer::EffectRef& _effect,
		const DirectX::SimpleMath::Vector3 _effectPos);

	/// @brief �}�l�W���[���擾
	/// @return �G�t�F�N�V�A�}�l�W���[�ϐ�
	const Effekseer::ManagerRef& GetManager() const;
private:
	DX11EffecseerManager();
	~DX11EffecseerManager();

	/// @brief �J�����s����Z�b�g
	void UpdateCamMatrix();

	/// @brief �G�t�F�N�V�A�̍s����쐬
	/// @param _dxMtx DIrectX�̍s��
	/// @param �G�t�F�N�V�A�̍s��
	void CreateEffekseerMtx(const DirectX::SimpleMath::Matrix& _dxMtx,
		Effekseer::Matrix44& _outMtx);

	/// @brief �G�t�F�N�V�A��Vector3���쐬
	/// @param _dxMtx DIrectX��Vector3
	/// @param �G�t�F�N�V�A��Vector3
	Effekseer::Vector3D CreateEffekseerVector3(const DirectX::SimpleMath::Vector3& _dxVec3);
};

