#include "pch.h"
#include "CP_HitStopManager.h"

namespace HashiTaku
{
	void CP_HitStopManager::HitStopBegin(u_int _stopFrame)
	{
		// �q�b�g�X�g�b�v���͌Ăяo���Ȃ�
		if (isHitStopping) return;

		// �p�����[�^����
		isHitStopping = true;
		stopFrame = _stopFrame;
		curStopFrame = 0;

		// �q�b�g�X�g�b�p�[�S�̂ɒʒm
		for (auto& pStopper : hitStoppers)
		{
			pStopper->OnHitStopBegin();
		}
	}

	void CP_HitStopManager::Update()
	{
		HitStopUpdate();
	}

	void CP_HitStopManager::HitStopUpdate()
	{
		if (!isHitStopping) return;

		// �i�߂�
		curStopFrame++;

		// ��߂��q�b�g�X�g�b�v���ɂȂ�����
		if (curStopFrame >= stopFrame)
			OnHitStopEnd();
	}

	void CP_HitStopManager::OnHitStopEnd()
	{
		isHitStopping = false;

		// �q�b�g�X�g�b�p�[�S�̂ɒʒm
		for (auto& pStopper : hitStoppers)
		{
			pStopper->OnHitStopEnd();
		}
	}

	void CP_HitStopManager::AddHitStopper(HitStopper_Base& _addHitStopper)
	{
		auto itr = std::find(hitStoppers.begin(), hitStoppers.end(), &_addHitStopper);
		if (itr != hitStoppers.end())
		{
			HASHI_DEBUG_LOG("���ɒǉ�����Ă��܂�");
			return;
		}
		hitStoppers.push_back(&_addHitStopper);

	}

	void CP_HitStopManager::RemoveHitStopper(HitStopper_Base& _removeHitStopper)
	{
		hitStoppers.remove(&_removeHitStopper);
	}
}