#pragma once
#include "Singleton_Base.h"
#include "SoundAsset.h"

// XTK Audio URL:https://github.com/microsoft/DirectXTK/wiki/Audio

namespace HashiTaku
{
	/// @brief �T�E���h���Đ�����Ƃ��̃p�����[�^
	class PlaySoundParameter : public IImGuiUser, public ISaveLoad
	{
	public:
		/// @brief �Đ�����T�E���h
		SoundAsset* pSound;

		/// @brief ����(0.0�`1.0)
		float volume;

		/// @brief ���̍���(-1.0�`1.0�A����0.0)
		float pitch;

		/// @brief ���[�v�Đ����邩�H
		bool isLoop;

		/// @brief ���X�i�[��3D��ԏ�̉����Đ����邩�H
		bool isUseLisneter;
	public:
		PlaySoundParameter();
		~PlaySoundParameter() {}

		json Save() override;
		void Load(const json& _data) override;

	private:
		void ImGuiDebug() override;
	};

	/// @brief �T�E���h�}�l�W���[
	class DXSoundManager : public Singleton_Base<DXSoundManager>
	{
		friend class MainApplication;	// ���������s������
		friend class Singleton_Base<DXSoundManager>;	// ���g��s�C���X�^���X�쐬�̂���

		/// @brief �Đ����̃T�E���h�p�����[�^
		struct PlaySoundInstance
		{
			/// @brief �Đ����̃T�E���h�C���f�b�N�X
			int soundId{ -1 };

			/// @brief �T�E���h�C���X�^���X
			std::unique_ptr<DX::SoundEffectInstance> pSoundInstance;
		};

		/// @brief ���\�[�X���쐬����G���W��
		std::unique_ptr<DirectX::AudioEngine> pAudioEngine;

		/// @brief 3D��ԂōĐ�����Ƃ��̃��X�i�[(��{�J����)
		const DX::AudioListener* pAudioLisner;

		/// @brief �Đ����̃T�E���h���X�g
		std::list<PlaySoundInstance> playSoundInstances;
	public:
		/// @brief �X�V����
		void Update();

		/// @brief �Đ�����
		/// @param _playSound �Đ���������
		/// @param _volume ����(1.0�����{)
		/// @param _pitch ���̍���
		/// @param _isLoop ���[�v���邩�H
		/// @param _useListener 3D�������g�p���邩
		/// @param _soundPos �T�E���h�̃��[���h���W
		/// @return �Đ����Ă���T�E���h��ID
		int Play(const SoundAsset& _playSound,
			float _volume = 0.5f,
			float _pitch = 0.0f,
			bool _isLoop = false,
			bool _useListener = false,
			const DXSimp::Vector3& _soundPos = DXSimp::Vector3::Zero);

		/// @brief �Đ�����
		/// @param _soundParameter �Đ�����T�E���h���
		/// @param _soundPos �T�E���h�̃��[���h���W
		/// @return �Đ����Ă���T�E���h��ID
		int Play(const PlaySoundParameter& _soundParameter,
			const DXSimp::Vector3& _soundPos = DXSimp::Vector3::Zero);

		/// @brief �Đ����̉���S�Ē�~����
		void StopAll();

		/// @brief �w�肵���T�E���h���~�߂�
		/// @param _soundId �T�E���hID
		void StopSound(int _soundId);

		/// @brief 3D��ԂōĐ�����Ƃ��̃��X�i�[���Z�b�g
		/// @param _pSetListener ���X�i�[
		void SetAudioListener(const DX::AudioListener& _pSetListener);

		/// @brief ���X�i�[���폜����
		void RemoveAudioListener();

		/// @brief �G���W�����擾(���[�h���ɕK�v)
		/// @return �G���W��
		DirectX::AudioEngine& GetEngine();

	private:
		DXSoundManager();
		~DXSoundManager();

		/// @brief ������
		void Init();

		/// @brief �Đ����̉����I����Ă��邩�m�F
		void CheckEndSound();

		/// @brief �󂢂Ă���C���f�b�N�X�ԍ����擾����
		/// @return �󂢂Ă���C���f�b�N�X�ԍ�
		int GetEmptyIndex();
	};

}

