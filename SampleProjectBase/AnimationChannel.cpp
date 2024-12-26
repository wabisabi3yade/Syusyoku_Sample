#include "pch.h"
#include "AnimationChannel.h"

namespace HashiTaku
{
	void AnimationChannel::AddPosKey(float _startKeyNum, const DXSimp::Vector3& _position)
	{
		AnimKey_V3 addPosKey;
		addPosKey.parameter = _position;
		addPosKey.startKeyNum = _startKeyNum;
		positionKeys.push_back(addPosKey);
	}

	void AnimationChannel::AddScaleKey(float _startKeyNum, const DXSimp::Vector3& _scale)
	{
		AnimKey_V3 addScaleKey;
		addScaleKey.parameter = _scale;
		addScaleKey.startKeyNum = _startKeyNum;
		scaleKeys.push_back(addScaleKey);
	}

	void AnimationChannel::AddQuatKey(float _startKeyNum, const DXSimp::Quaternion& _quaternion)
	{
		AnimKey_Q addQuatKey;
		addQuatKey.parameter = _quaternion;
		addQuatKey.startKeyNum = _startKeyNum;
		quaternionKeys.push_back(addQuatKey);
	}

	void AnimationChannel::ResetKeys()
	{
		AnimKey_V3 v3 = positionKeys[0];
		positionKeys.clear();
		positionKeys.push_back(v3);
	}

	u_int AnimationChannel::FindPrevPosKey(float _playingRatio) const
	{
		u_int prevKeyNum = GetPosKeyCnt() - 1;

		float posKeyNum = GetPosKeyByRatio(_playingRatio);

		// 再生時間から前のキーを探す
		for (u_int k_i = 0; k_i < GetPosKeyCnt() - 1; k_i++)
		{
			float keyNum = positionKeys[k_i + 1].startKeyNum;
			if (posKeyNum < keyNum)
			{
				prevKeyNum = k_i;
				break;
			}
		}

		return prevKeyNum;
	}

	u_int AnimationChannel::FindPrevScaleKey(float _playingRatio)const
	{
		u_int prevKeyNum = GetScaleKeyCnt() - 1;

		float scaleKeyNum = GetScaleKeyByRatio(_playingRatio);

		// 再生時間から前のキーを探す
		for (u_int k_i = 0; k_i < GetScaleKeyCnt() - 1; k_i++)
		{
			float keyNum = scaleKeys[k_i + 1].startKeyNum;
			if (scaleKeyNum < keyNum)
			{
				prevKeyNum = k_i;
				break;
			}
		}

		return prevKeyNum;
	}

	u_int AnimationChannel::FindPrevQuatKey(float _playingRatio) const
	{
		u_int prevKeyNum = GetQuatKeyCnt() - 1;

		float quatKeyNum = GetQuatKeyByRatio(_playingRatio);

		// 再生時間から前のキーを探す
		for (u_int k_i = 0; k_i < GetQuatKeyCnt() - 1; k_i++)
		{
			float keyNum = quaternionKeys[k_i + 1].startKeyNum;
			if (quatKeyNum < keyNum)
			{
				prevKeyNum = k_i;
				break;
			}
		}

		return prevKeyNum;
	}

	void AnimationChannel::SetBoneIdx(u_int _boneIdx)
	{
		boneIdx = _boneIdx;
	}

	void AnimationChannel::SetName(const std::string& _name)
	{
		channelName = _name;
	}

	const AnimKey_V3& AnimationChannel::GetPosKey(u_int _keyNum) const
	{
		return positionKeys[_keyNum];
	}

	const AnimKey_V3& AnimationChannel::GetScaleKey(u_int _keyNum) const
	{
		return scaleKeys[_keyNum];
	}

	const AnimKey_Q& AnimationChannel::GetQuatKey(u_int _keyNum) const
	{
		return quaternionKeys[_keyNum];
	}

	u_int AnimationChannel::GetPosKeyCnt() const
	{
		return static_cast<u_int>(positionKeys.size());
	}

	u_int AnimationChannel::GetScaleKeyCnt() const
	{
		return static_cast<u_int>(scaleKeys.size());
	}

	u_int AnimationChannel::GetQuatKeyCnt() const
	{
		return static_cast<u_int>(quaternionKeys.size());
	}

	u_int AnimationChannel::GetBodeIdx() const
	{
		return boneIdx;
	}

	std::string AnimationChannel::GetName()
	{
		return channelName;
	}

	u_int AnimationChannel::GetNextPosKey(u_int _currentKeyNum, int _progressNum) const
	{
		int calcKeyNum = _currentKeyNum + _progressNum;
		int intKeyCnt = static_cast<int>(GetPosKeyCnt());

		if (calcKeyNum > intKeyCnt - 1)	// 最大数を超えているなら
		{
			calcKeyNum -= intKeyCnt;
		}
		else if (calcKeyNum < 0)	// 0未満なら
		{
			calcKeyNum += intKeyCnt;
		}

		return calcKeyNum;
	}

	u_int AnimationChannel::GetNextScaleKey(u_int _currentKeyNum, int _progressNum) const
	{
		int calcKeyNum = _currentKeyNum + _progressNum;
		int intKeyCnt = static_cast<int>(GetScaleKeyCnt());

		if (calcKeyNum > intKeyCnt - 1)	// 最大数を超えているなら
		{
			calcKeyNum -= intKeyCnt;
		}
		else if (calcKeyNum < 0)	// 0未満なら
		{
			calcKeyNum += intKeyCnt;
		}

		return calcKeyNum;
	}

	u_int AnimationChannel::GetNextQuatKey(u_int _currentKeyNum, int _progressNum) const
	{
		int calcKeyNum = _currentKeyNum + _progressNum;
		int intKeyCnt = static_cast<int>(GetQuatKeyCnt());

		if (calcKeyNum > intKeyCnt - 1)	// 最大数を超えているなら
		{
			calcKeyNum -= intKeyCnt;
		}
		else if (calcKeyNum < 0)	// 0未満なら
		{
			calcKeyNum += intKeyCnt;
		}

		return calcKeyNum;
	}

	float AnimationChannel::GetPosKeyByRatio(float _playingRatio) const
	{
		// 最大のキー数を取得
		u_int lastKeyNum = static_cast<u_int>(positionKeys.size()) - 1;
		float maxKeyNum = positionKeys[lastKeyNum].startKeyNum;

		return maxKeyNum * _playingRatio;
	}

	float AnimationChannel::GetScaleKeyByRatio(float _playingRatio) const
	{
		// 最大のキー数を取得
		u_int lastKeyNum = static_cast<u_int>(scaleKeys.size()) - 1;
		float maxKeyNum = scaleKeys[lastKeyNum].startKeyNum;

		return maxKeyNum * _playingRatio;
	}

	float AnimationChannel::GetQuatKeyByRatio(float _playingRatio) const
	{
		// 最大のキー数を取得
		u_int lastKeyNum = static_cast<u_int>(quaternionKeys.size()) - 1;
		float maxKeyNum = quaternionKeys[lastKeyNum].startKeyNum;

		return maxKeyNum * _playingRatio;
	}
}