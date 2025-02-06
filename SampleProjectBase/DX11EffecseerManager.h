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

		struct PlayHandleParameter
		{
			/// @brief �G�t�F�N�g�̃n���h��
			Effekseer::Handle handle{ -1 };

			/// @brief �x�[�X�̍Đ����x
			float baseSpeed{ 1.0f };
		};

		/// @brief ���ݍĐ����Ă���G�t�F�N�g�̃n���h�����X�g
		std::list<PlayHandleParameter> playingHandleList;

		/// @brief �G�t�F�N�V�A�̕`��ϐ�
		EffekseerRendererDX11::RendererRef renderer;

		/// @brief �}�l�W���[�ϐ�
		Effekseer::ManagerRef manager;

		/// @brief D3D�`��N���X
		D3D11_Renderer* pRenderer;

		/// @brief �O�t���[���̃^�C���X�P�[��
		float prevTimeScale;
		
		/// @brief �G�t�F�N�g�����S�ɒ�~������
		bool isPause;

	public:
		/// @brief �G�t�F�N�g�̉����Ȃ���\���n���h��
		static constexpr int NONE_VFX_HANDLE{ -9999 };


#ifdef EDIT
		// �G�t�F�N�g��\�������邩�H
		bool isVfxDisplay{ true };
#endif // EDIT
	public:
		/// @brief �������֐�
		void Init();

		/// @brief ��Ԃ����Z�b�g����
		void Reset();

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
			const DXSimp::Vector3& _pos,
			const DXSimp::Vector3& _scale = DXSimp::Vector3::One,
			const DXSimp::Vector3& _eularAngles = DXSimp::Vector3::Zero,
			int _startFrame = 0);

		Effekseer::Handle Play(const CreateVfxInfo& _createVfx,
			const DXSimp::Vector3& _pos,
			const DXSimp::Vector3& _eularAngles = DXSimp::Vector3::Zero);

		/// @brief �S�ẴG�t�F�N�g���~����
		void SetPause(bool _setPause);

		/// @brief �F��ύX����Ȃ�
		/// @param _efkHandle �G�t�F�N�g�̃n���h��
		/// @param _color �F
		void ChangeColor(Effekseer::Handle _efkHandle, const DXSimp::Color& _color);

		/// @brief �w�肵���G�t�F�N�g���폜����
		/// @param _deleteHandle �폜����G�t�F�N�g�n���h��
		void DestroyVfx(const Effekseer::Handle _deleteHandle);

		/// @brief �}�l�W���[���擾
		/// @return �G�t�F�N�V�A�}�l�W���[�ϐ�
		const Effekseer::ManagerRef& GetManager() const;
	private:
		DX11EffecseerManager();
		~DX11EffecseerManager();

		/// @brief �G�t�F�N�g�S�̂̃^�C���X�P�[�����V�[���ɍ��킹��
		void UpdateTimeScale();

		/// @brief �G�t�F�N�g�����݂��Ă��邩�ǂ����`�F�b�N
		void UpdateCheckExist();

		/// @brief �J�����s����Z�b�g
		void UpdateCamMatrix();

		/// @brief �J�����������p�x�����߂�
		/// @param _efkPos �G�t�F�N�g�̍��W
		/// @param _outAngles ����
		void CalcLookCameraAngles(const DXSimp::Vector3& _efkPos, DXSimp::Vector3& _outAngles);

		/// @brief �G�t�F�N�V�A�̍s����쐬
		/// @param _dxMtx DIrectX�̍s��
		/// @param �G�t�F�N�V�A�̍s��
		void CreateEffekseerMtx(const DXSimp::Matrix& _dxMtx,
			Effekseer::Matrix44& _outMtx);

		/// @brief �G�t�F�N�V�A��Vector3���쐬
		/// @param _dxMtx DIrectX��Vector3
		/// @param �G�t�F�N�V�A��Vector3
		Effekseer::Vector3D CreateEffekseerVector3(const DXSimp::Vector3& _dxVec3);
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

		/// @brief �J���������������邩�H
		bool isLookCamera{ false };

		json Save() override;
		void Load(const json& _data) override;

		void ImGuiDebug() override;
	};
}