#pragma once
#include "Singleton_Base.h"

namespace HashiTaku
{
	class CreateVfxInfo;
	class VisualEffect;

	/// @brief DX11���ŏ����������Ȃǂ��s���N���X
	class DX11EffecseerManager : public Singleton_Base<DX11EffecseerManager>
	{
	private:
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

		/// @brief �G�t�F�N�g�����݂��Ă��邩�擾
		/// @param _vfxHandle �m�F����G�t�F�N�g�̃n���h��
		/// @return ���݂��Ă��邩�H
		bool ExistEffect(Effekseer::Handle _vfxHandle) const;

		/// @brief �G�t�F�N�g���Đ�����
		/// @param _effect �Đ�����G�t�F�N�g
		/// @param _playSpeed �Đ����x
		/// @param _pos ���W
		/// @param _scale �X�P�[��
		/// @param _eularAngles �p�x
		/// @param _startFrame �Đ��J�n�t���[��
		/// @return �Đ����Ă���n���h��
		Effekseer::Handle Play(
			const Effekseer::EffectRef& _effect,
			float _playSpeed,
			const DirectX::SimpleMath::Vector3& _pos,
			const DirectX::SimpleMath::Vector3& _scale = DirectX::SimpleMath::Vector3::One,
			const DirectX::SimpleMath::Vector3& _eularAngles = DirectX::SimpleMath::Vector3::Zero,
			int _startFrame = 0);

		Effekseer::Handle Play(const CreateVfxInfo& _createVfx,
			const DirectX::SimpleMath::Vector3& _pos,
			const DirectX::SimpleMath::Vector3& _eularAngles = DirectX::SimpleMath::Vector3::Zero);

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

	/// @brief �G�t�F�N�g�������
	class CreateVfxInfo : public IImGuiUser, public ISaveLoad
	{
	public:
		/// @brief �G�t�F�N�g�F
		DXSimp::Color effectColor{ DXSimp::Vector4::One };

		/// @brief �G�t�F�N�g��
		VisualEffect* pHitVfx{ nullptr };

		/// @brief  �X�P�[��
		float scale{ 1.0f };

		/// @brief ���x
		float speed{ 1.0f };

		/// @brief �J�n�t���[��
		int startFrame{ 0 };

		/// @brief �w��F�𔽉f����
		bool isApplyColor{ false };

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;

		void ImGuiDebug() override;
	};
}