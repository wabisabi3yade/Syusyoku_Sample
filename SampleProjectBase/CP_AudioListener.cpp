#include "pch.h"
#include "CP_AudioListener.h"

namespace HashiTaku
{
	CP_AudioListener::CP_AudioListener()
	{
	}

	void CP_AudioListener::Init()
	{
		// インスタンス作成し、サウンドマネジャーにセット
		pDXAudioListener = std::make_unique<DX::AudioListener>();
		DXSoundManager::GetInstance()->SetAudioListener(*pDXAudioListener);
	}

	void CP_AudioListener::OnDestroy()
	{
		// インスタンスを解除
		DXSoundManager::GetInstance()->RemoveAudioListener();
	}

	void CP_AudioListener::LateUpdate()
	{
		Transform& transform = GetTransform();

		// 座標
		DXSimp::Vector3 pos = transform.GetPosition();
		pos.z = -pos.z;	// 右手系なのでzをマイナス
		pDXAudioListener->SetPosition(pos);

		// 正面方向
		DXSimp::Vector3 forward = transform.Forward();
		forward.z = -forward.z; // 右手系なのでzをマイナス
		pDXAudioListener->OrientFront = forward;
	}
}

