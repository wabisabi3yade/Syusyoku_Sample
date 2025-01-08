#include "pch.h"
#include "CP_AudioListener.h"

namespace HashiTaku
{
	CP_AudioListener::CP_AudioListener()
	{
	}

	void CP_AudioListener::Init()
	{
		// �C���X�^���X�쐬���A�T�E���h�}�l�W���[�ɃZ�b�g
		pDXAudioListener = std::make_unique<DX::AudioListener>();
		DXSoundManager::GetInstance()->SetAudioListener(*pDXAudioListener);
	}

	void CP_AudioListener::OnDestroy()
	{
		// �C���X�^���X������
		DXSoundManager::GetInstance()->RemoveAudioListener();
	}

	void CP_AudioListener::LateUpdate()
	{
		Transform& transform = GetTransform();

		// ���W
		DXSimp::Vector3 pos = transform.GetPosition();
		pos.z = -pos.z;	// �E��n�Ȃ̂�z���}�C�i�X
		pDXAudioListener->SetPosition(pos);

		// ���ʕ���
		DXSimp::Vector3 forward = transform.Forward();
		forward.z = -forward.z; // �E��n�Ȃ̂�z���}�C�i�X
		pDXAudioListener->OrientFront = forward;
	}
}

