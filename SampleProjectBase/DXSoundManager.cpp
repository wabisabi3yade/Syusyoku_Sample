#include "pch.h"
#include "DXSoundManager.h"
#include "AssetGetter.h"

namespace HashiTaku
{
	constexpr u_int MAX_PLAY_CNT(100);	// 音を再生する最大個数

	void DXSoundManager::Update()
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

	void DXSoundManager::CheckEndSound()
	{
		// 終了しているか確認
		auto playItr = playSoundInstances.begin();
		for (; playItr != playSoundInstances.end();)
		{
			auto state = (*playItr).pSoundInstance->GetState();

			// 終了していたなら
			if (state == DX::SoundState::STOPPED)
				playItr = playSoundInstances.erase(playItr);
			else
				playItr++;
		}
	}

	int DXSoundManager::Play(const SoundAsset& _playSound,
		float _volume,
		float _pitch,
		bool _isLoop,
		bool _useListener,
		const DXSimp::Vector3& _soundPos)
	{
		if (static_cast<u_int>(playSoundInstances.size()) > MAX_PLAY_CNT)
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

		// 音のパラメータ調整
		effect->SetVolume(_volume);
		effect->SetPitch(_pitch);

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
		PlaySoundInstance param;
		param.soundId = playId;
		param.pSoundInstance = std::move(effect);
		playSoundInstances.push_back(std::move(param));

		return playId;
	}

	int DXSoundManager::Play(const PlaySoundParameter& _soundParameter, const DXSimp::Vector3& _soundPos)
	{
		if (!_soundParameter.pSound) return -1;

		// 再生する
		return Play(*_soundParameter.pSound,
			_soundParameter.volume,
			_soundParameter.pitch,
			_soundParameter.isLoop,
			_soundParameter.isUseLisneter,
			_soundPos
			);
	}

	void DXSoundManager::StopAll()
	{
		// 全ての音を止める
		for (auto& play : playSoundInstances)
		{
			play.pSoundInstance->Stop();
		}

		playSoundInstances.clear();
	}

	void DXSoundManager::StopSound(int _soundId)
	{
		auto itr = std::find_if(playSoundInstances.begin(), playSoundInstances.end(),
			[=](const PlaySoundInstance& _play)
			{
				return _play.soundId == _soundId;
			});

		// 見つからないなら
		if (itr == playSoundInstances.end()) return;

		// 再生を止める
		(*itr).pSoundInstance->Stop();
		playSoundInstances.erase(itr);
	}

	void DXSoundManager::SetAudioListener(const DX::AudioListener& _pSetListener)
	{
		pAudioLisner = &_pSetListener;
	}

	void DXSoundManager::RemoveAudioListener()
	{
#ifdef EDIT
		if (pAudioLisner)
			HASHI_DEBUG_LOG("リスナーを削除しました");
#endif

		pAudioLisner = nullptr;
	}

	DXSoundManager::DXSoundManager() : pAudioLisner(nullptr)
	{
	}

	DXSoundManager::~DXSoundManager()
	{
		playSoundInstances.clear();

		// 内部で処理を起こさないといけない
		pAudioEngine->Update();
	}

	void DXSoundManager::Init()
	{
		// 初期化
		DX::AUDIO_ENGINE_FLAGS eflags = DX::AudioEngine_Default;
#ifdef _DEBUG
		eflags |= DX::AudioEngine_Debug;
#endif
		pAudioEngine = std::make_unique<DX::AudioEngine>(eflags);
	}

	int DXSoundManager::GetEmptyIndex()
	{
		// インデックス番号を探す
		int idx = 0;
		for (; idx < MAX_PLAY_CNT; idx++)
		{
			bool isDupplicate = false;	// 重複しているかフラグ
			for (auto& play : playSoundInstances)
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

	DirectX::AudioEngine& DXSoundManager::GetEngine()
	{
		return *pAudioEngine;
	}

	PlaySoundParameter::PlaySoundParameter() :
		pSound(nullptr),
		volume(0.5f),
		pitch(0.0f),
		isLoop(false),
		isUseLisneter(false)
	{
	}

	json PlaySoundParameter::Save()
	{
		json data;

		if (pSound)
			data["soundName"] = pSound->GetAssetName();
		data["volume"] = volume;
		data["pitch"] = pitch;
		data["loop"] = isLoop;
		data["useListener"] = isUseLisneter;

		return data;
	}

	void PlaySoundParameter::Load(const json& _data)
	{
		std::string soundName;
		if (LoadJsonString("soundName", soundName, _data))
		{
			pSound = AssetGetter::GetAsset<SoundAsset>(soundName);
		}
		LoadJsonFloat("volume", volume, _data);
		LoadJsonFloat("pitch", pitch, _data);
		LoadJsonBoolean("loop", isLoop, _data);
		LoadJsonBoolean("useListener", isUseLisneter, _data);
	}

	void PlaySoundParameter::ImGuiDebug()
	{
		std::string str = "Null";
		if (pSound)
			str = pSound->GetAssetName();

		if (AssetGetter::ImGuiGetCombobox<SoundAsset>("Sound", str))
		{
			pSound = AssetGetter::GetAsset<SoundAsset>(str);
		}

		ImGui::Checkbox("Loop", &isLoop);
		ImGui::Checkbox("UseListener", &isUseLisneter);
		ImGui::DragFloat("Volume", &volume, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Pitch", &pitch, 0.01f, -1.0f, 1.0f);
	}
}