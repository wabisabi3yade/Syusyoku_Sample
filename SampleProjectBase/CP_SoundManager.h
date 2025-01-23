#pragma once
#include "SingletonComponent.h"
namespace HashiTaku
{
	/// @brief �V�[�����Ŏg���₷���悤�ɂ����T�E���h�}�l�W���[
	class CP_SoundManager : public SingletonComponent<CP_SoundManager>
	{
		/// @brief �T�E���h���̃}�l�W���[
		DXSoundManager* pDXSoundManager{ nullptr };

		/// @brief ���ݍĐ����Ă���BGM��ID
		int curPlayBGMId{ -1 };

		/// @brief ���ʂ̔{��
		float volumeTimes{ 1.0f };
	public:
		/// @brief ����������
		void Awake() override;

		/// @brief �폜������
		void OnDestroy() override;

		/// @brief BGM���Đ�����
		/// @param _bgmParameter �Đ�����BGM
		int PlayBGM(const PlaySoundParameter& _bgmParameter);

		/// @brief SE���Đ�����
		/// @param _seParameter �Đ�����SE
		/// @param _soundPos se���Đ�������W
		int PlaySE(const PlaySoundParameter& _seParameter, const DXSimp::Vector3& _soundPos = DXSimp::Vector3::Zero);

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const json& _data) override;

	private:
		void ImGuiDebug() override;
	};

}

