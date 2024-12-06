#include "pch.h"
#include "DXSoundManager.h"

constexpr u_int MAX_PLAY_CNT(100);	// �����Đ�����ő��

void HashiTaku::DXSoundManager::Update()
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

void HashiTaku::DXSoundManager::CheckEndSound()
{
	// �I�����Ă��邩�m�F
	auto playItr = playSoundParams.begin();
	for (; playItr != playSoundParams.end(); playItr++)
	{
		auto state = (*playItr).pSoundInstance->GetState();

		// �I�����Ă����Ȃ�
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
		HASHI_DEBUG_LOG("�Đ������ő吔�𒴂��Ă��܂�");
		return -1;
	}

	DX::SoundEffect* soundFx = _playSound.GetSoundData();

	// �T�E���h�f�[�^����C���X�^���X���쐬
	auto effect = soundFx->CreateInstance(
		DX::SoundEffectInstance_Use3D
		| DX::SoundEffectInstance_ReverbUseFilters
	);

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
	PlaySoundParam param;
	param.soundId = playId;
	param.pSoundInstance = std::move(effect);
	playSoundParams.push_back(std::move(param));

	return playId;
}

void HashiTaku::DXSoundManager::StopAll()
{
	// �S�Ẳ����~�߂�
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

	// ������Ȃ��Ȃ�
	if (itr == playSoundParams.end()) return;

	// �Đ����~�߂�
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
		HASHI_DEBUG_LOG("���X�i�[���폜���܂���");
#endif

	pAudioLisner = nullptr;
}

HashiTaku::DXSoundManager::DXSoundManager() : pAudioLisner(nullptr)
{
}

void HashiTaku::DXSoundManager::Init()
{
	// ������
	DX::AUDIO_ENGINE_FLAGS eflags = DX::AudioEngine_Default;
#ifdef _DEBUG
	eflags |= DX::AudioEngine_Debug;
#endif
	pAudioEngine = std::make_unique<DX::AudioEngine>(eflags);
}

int HashiTaku::DXSoundManager::GetEmptyIndex()
{
	// �C���f�b�N�X�ԍ���T��
	int idx = 0;
	for (; idx < MAX_PLAY_CNT; idx++)
	{
		bool isDupplicate = false;	// �d�����Ă��邩�t���O
		for (auto& play : playSoundParams)
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

DirectX::AudioEngine& HashiTaku::DXSoundManager::GetEngine()
{
	return *pAudioEngine;
}
