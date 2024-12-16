#include "pch.h"
#include "CP_HitStopManager.h"

namespace HashiTaku
{
	void CP_HitStopManager::HitStopBegin(u_int _stopFrame)
	{
		// ヒットストップ中は呼び出さない
		if (isHitStopping) return;

		// パラメータを代入
		isHitStopping = true;
		stopFrame = _stopFrame;
		curStopFrame = 0;

		// ヒットストッパー全体に通知
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

		// 進める
		curStopFrame++;

		// 定めたヒットストップ数になったら
		if (curStopFrame >= stopFrame)
			OnHitStopEnd();
	}

	void CP_HitStopManager::OnHitStopEnd()
	{
		isHitStopping = false;

		// ヒットストッパー全体に通知
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
			HASHI_DEBUG_LOG("既に追加されています");
			return;
		}
		hitStoppers.push_back(&_addHitStopper);

	}

	void CP_HitStopManager::RemoveHitStopper(HitStopper_Base& _removeHitStopper)
	{
		hitStoppers.remove(&_removeHitStopper);
	}
}