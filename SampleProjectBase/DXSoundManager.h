#pragma once
#include "Singleton_Base.h"
#include "SoundAsset.h"

// XTK Audio URL:https://github.com/microsoft/DirectXTK/wiki/Audio

namespace HashiTaku
{
	/// @brief �T�E���h�}�l�W���[
	class DXSoundManager : public Singleton_Base<DXSoundManager>
	{
		friend class MainApplication;	// ���������s������
		friend class Singleton_Base<DXSoundManager>;	// ���g��s�C���X�^���X�쐬�̂���

		/// @brief �Đ����̃T�E���h�p�����[�^
		struct PlaySoundParam
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

		/// @brief �Đ����̃T�E���h�p�����[�^�S
		std::list<PlaySoundParam> playSoundParams;
	public:
		/// @brief �X�V����
		void Update();

		/// @brief �Đ�����
		/// @param _playSound �Đ���������
		/// @param _isLoop ���[�v���邩�H
		/// @param _useListener 3D�������g�p���邩
		/// @param _soundPos �T�E���h�̃��[���h���W
		/// @return �Đ����Ă���T�E���h��ID
		int Play(const SoundAsset& _playSound,
			bool _isLoop,
			bool _useListener,
			const DXSimp::Vector3& _soundPos);

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
		~DXSoundManager() {}

		/// @brief ������
		void Init();

		/// @brief �Đ����̉����I����Ă��邩�m�F
		void CheckEndSound();

		/// @brief �󂢂Ă���C���f�b�N�X�ԍ����擾����
		/// @return �󂢂Ă���C���f�b�N�X�ԍ�
		int GetEmptyIndex();
	};

}

