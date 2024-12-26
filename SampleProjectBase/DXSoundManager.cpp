#include "pch.h"
#include "DXSoundManager.h"
#include "AssetGetter.h"

namespace HashiTaku
{
	constexpr u_int MAX_PLAY_CNT(100);	// �����Đ�����ő��

	void DXSoundManager::Update()
	{
		assert(pAudioEngine && "���������s���Ă�������");

		// ���̍X�V
		if (!pAudioEngine->Update())
		{
			// �I�[�f�B�I���A�N�e�B�u��ԂłȂ�
			if (pAudioEngine->IsCriticalError())
			{
				HASHI_DEBUG_LOG("�I�[�f�B�I���A�N�e�B�u��Ԃł���܂���");
			}
		}

		// �Đ��I�����Ă��邩�m�F
		CheckEndSound();
	}

	void DXSoundManager::CheckEndSound()
	{
		// �I�����Ă��邩�m�F
		auto playItr = playSoundInstances.begin();
		for (; playItr != playSoundInstances.end();)
		{
			auto state = (*playItr).pSoundInstance->GetState();

			// �I�����Ă����Ȃ�
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
			HASHI_DEBUG_LOG("�Đ������ő吔�𒴂��Ă��܂�");
			return -1;
		}

		DX::SoundEffect* soundFx = _playSound.GetSoundData();

		// �T�E���h�f�[�^����C���X�^���X���쐬
		auto effect = soundFx->CreateInstance(
			DX::SoundEffectInstance_Use3D
			| DX::SoundEffectInstance_ReverbUseFilters
		);

		// ���̃p�����[�^����
		effect->SetVolume(_volume);
		effect->SetPitch(_pitch);

		// �Đ�����
		effect->Play(_isLoop);

		// 3D��ԂōĐ�����Ȃ�
		if (_useListener)
		{
			if (pAudioLisner)
			{
				DX::AudioEmitter emitter;
				emitter.SetPosition(_soundPos);
			}
			else
				HASHI_DEBUG_LOG("���X�i�[���ݒ肳��Ă��܂���");
		}

		// ID���擾
		int playId = GetEmptyIndex();

		// �Đ����X�g�ɒǉ�
		PlaySoundInstance param;
		param.soundId = playId;
		param.pSoundInstance = std::move(effect);
		playSoundInstances.push_back(std::move(param));

		return playId;
	}

	int DXSoundManager::Play(const PlaySoundParameter& _soundParameter, const DXSimp::Vector3& _soundPos)
	{
		if (!_soundParameter.pSound) return -1;

		// �Đ�����
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
		// �S�Ẳ����~�߂�
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

		// ������Ȃ��Ȃ�
		if (itr == playSoundInstances.end()) return;

		// �Đ����~�߂�
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
			HASHI_DEBUG_LOG("���X�i�[���폜���܂���");
#endif

		pAudioLisner = nullptr;
	}

	DXSoundManager::DXSoundManager() : pAudioLisner(nullptr)
	{
	}

	DXSoundManager::~DXSoundManager()
	{
		playSoundInstances.clear();

		// �����ŏ������N�����Ȃ��Ƃ����Ȃ�
		pAudioEngine->Update();
	}

	void DXSoundManager::Init()
	{
		// ������
		DX::AUDIO_ENGINE_FLAGS eflags = DX::AudioEngine_Default;
#ifdef _DEBUG
		eflags |= DX::AudioEngine_Debug;
#endif
		pAudioEngine = std::make_unique<DX::AudioEngine>(eflags);
	}

	int DXSoundManager::GetEmptyIndex()
	{
		// �C���f�b�N�X�ԍ���T��
		int idx = 0;
		for (; idx < MAX_PLAY_CNT; idx++)
		{
			bool isDupplicate = false;	// �d�����Ă��邩�t���O
			for (auto& play : playSoundInstances)
			{
				if (idx != play.soundId) continue;

				// �d��������
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