#include "pch.h"
#include "DXSoundManager.h"

constexpr u_int MAX_PLAY_CNT(100);	// 音を再生する最大個数

void HashiTaku::DXSoundManager::Update()
{
	assert(pAudioEngine && "初期化を行ってください");

	// 音の更新
	if (!pAudioEngine->Update())
	{
		// オーディオがアクティブ状態でない
		if (pAudioEngine->IsCriticalError())
		{
			HASHI_DEBUG_LOG("オーディオがアクティブ状態でありません");
		}
	}

	// 再生終了しているか確認
	CheckEndSound();
}

void HashiTaku::DXSoundManager::CheckEndSound()
{
	// 終了しているか確認
	auto playItr = playSoundParams.begin();
	for (; playItr != playSoundParams.end(); playItr++)
	{
		auto state = (*playItr).pSoundInstance->GetState();

		// 終了していたなら
		if (state == DX::SoundState::STOPPED)
			playItr = playSoundParams.erase(playItr);
	}
}

int HashiTaku::DXSoundManager::Play(const SoundAsset& _playSound,
	bool _isLoop,
	bool _useListener,
	const DXSimp::Vector3& _soundPos)
{
	if (static_cast<u_int>(playSoundParams.size()) > MAX_PLAY_CNT)
	{
		HASHI_DEBUG_LOG("再生音が最大数を超えています");
		return -1;
	}

	DX::SoundEffect* soundFx = _playSound.GetSoundData();

	// サウンドデータからインスタンスを作成
	auto effect = soundFx->CreateInstance(
		DX::SoundEffectInstance_Use3D
		| DX::SoundEffectInstance_ReverbUseFilters
	);

	// 再生する
	effect->Play(_isLoop);

	// 3D空間で再生するなら
	if (_useListener)
	{
		if (pAudioLisner)
		{
			DX::AudioEmitter emitter;
			emitter.SetPosition(_soundPos);
		}
		else
			HASHI_DEBUG_LOG("リスナーが設定されていません");
	}

	// IDを取得
	int playId = GetEmptyIndex();

	// 再生リストに追加
	PlaySoundParam param;
	param.soundId = playId;
	param.pSoundInstance = std::move(effect);
	playSoundParams.push_back(std::move(param));

	return playId;
}

void HashiTaku::DXSoundManager::StopAll()
{
	// 全ての音を止める
	for (auto& play : playSoundParams)
	{
		play.pSoundInstance->Stop();
	}
}

void HashiTaku::DXSoundManager::StopSound(int _soundId)
{
	auto itr = std::find_if(playSoundParams.begin(), playSoundParams.end(),
		[=](const PlaySoundParam& _play)
		{
			return _play.soundId == _soundId;
		});

	// 見つからないなら
	if (itr == playSoundParams.end()) return;

	// 再生を止める
	(*itr).pSoundInstance->Stop();
	playSoundParams.erase(itr);
}

void HashiTaku::DXSoundManager::SetAudioListener(const DX::AudioListener& _pSetListener)
{
	pAudioLisner = &_pSetListener;
}

void HashiTaku::DXSoundManager::RemoveAudioListener()
{
#ifdef EDIT
	if (pAudioLisner)
		HASHI_DEBUG_LOG("リスナーを削除しました");
#endif

	pAudioLisner = nullptr;
}

HashiTaku::DXSoundManager::DXSoundManager() : pAudioLisner(nullptr)
{
}

void HashiTaku::DXSoundManager::Init()
{
	// 初期化
	DX::AUDIO_ENGINE_FLAGS eflags = DX::AudioEngine_Default;
#ifdef _DEBUG
	eflags |= DX::AudioEngine_Debug;
#endif
	pAudioEngine = std::make_unique<DX::AudioEngine>(eflags);
}

int HashiTaku::DXSoundManager::GetEmptyIndex()
{
	// インデックス番号を探す
	int idx = 0;
	for (; idx < MAX_PLAY_CNT; idx++)
	{
		bool isDupplicate = false;	// 重複しているかフラグ
		for (auto& play : playSoundParams)
		{
			if (idx != play.soundId) continue;

			// 重複したら
			isDupplicate = true;
			break;
		}
		if (!isDupplicate) break;
	}

	return idx;
}

DirectX::AudioEngine& HashiTaku::DXSoundManager::GetEngine()
{
	return *pAudioEngine;
}
