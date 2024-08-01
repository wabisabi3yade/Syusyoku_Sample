#include "pch.h"
#include "AnimationChannel.h"

using namespace DirectX::SimpleMath;

void AnimationChannel::AddPosKey(float _startKeyNum, const DirectX::SimpleMath::Vector3& _position)
{
	AnimKey_V3 addPosKey;
	addPosKey.parameter = _position;
	addPosKey.startKeyNum = _startKeyNum;
	positionKeys.push_back(addPosKey);
}

void AnimationChannel::AddScaleKey(float _startKeyNum, const DirectX::SimpleMath::Vector3& _scale)
{
	AnimKey_V3 addScaleKey;
	addScaleKey.parameter = _scale;
	addScaleKey.startKeyNum = _startKeyNum;
	scaleKeys.push_back(addScaleKey);
}

void AnimationChannel::AddQuatKey(float _startKeyNum, const DirectX::SimpleMath::Quaternion& _quaternion)
{
	AnimKey_Q addQuatKey;
	addQuatKey.parameter = _quaternion;
	addQuatKey.startKeyNum = _startKeyNum;
	quaternionKeys.push_back(addQuatKey);
}

u_int AnimationChannel::FindPrevPosKey(float _playingTime, float _timePerKey) const
{
	u_int prevKeyNum = GetPosKeyCnt() - 1;

	// 再生時間から前のキーを探す
	for (u_int k_i = 0; k_i < GetPosKeyCnt() - 1; k_i++)
	{
		float keyTime = positionKeys[k_i + 1].startKeyNum * _timePerKey;
		if (_playingTime < keyTime)
		{
			prevKeyNum = k_i;
			break;
		}
	}

	return prevKeyNum;
}

u_int AnimationChannel::FindPrevScaleKey(float _playingTime, float _timePerKey)const
{
	u_int prevKeyNum = GetScaleKeyCnt() - 1;

	// 再生時間から前のキーを探す
	for (u_int k_i = 0; k_i < GetScaleKeyCnt() - 1; k_i++)
	{
		float keyTime = scaleKeys[k_i + 1].startKeyNum * _timePerKey;
		if (_playingTime < keyTime)
		{
			prevKeyNum = k_i;
			break;
		}
	}

	return prevKeyNum;
}

u_int AnimationChannel::FindPrevQuatKey(float _playingTime, float _timePerKey) const
{
	u_int prevKeyNum = GetQuatKeyCnt() - 1;

	// 再生時間から前のキーを探す
	for (u_int k_i = 0; k_i < GetQuatKeyCnt() - 1; k_i++)
	{
		float keyTime = quaternionKeys[k_i + 1].startKeyNum * _timePerKey;
		if (_playingTime < keyTime)
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

const AnimKey_V3& AnimationChannel::GetPosKey(u_int _keyNum)
{
	return positionKeys[_keyNum];
}

const AnimKey_V3& AnimationChannel::GetScaleKey(u_int _keyNum)
{
	return scaleKeys[_keyNum];
}

const AnimKey_Q& AnimationChannel::GetQuatKey(u_int _keyNum)
{
	return quaternionKeys[_keyNum];
}

//DirectX::SimpleMath::Vector3 AnimationChannel::GetPosition(float _playingTime) const
//{
//	if (GetPosKeyCnt() == 1)	// キーが1つしかないなら
//	{
//		return positionKeys[0].parameter;
//	}
//
//	// 前のキーを探す
//	u_int prevKeyNum = FindPrevPosKey(_playingTime);
//
//	// 補間を考慮した座標を取得
//	Vector3 lerpedPos = CalcInterpolatedPosition(prevKeyNum, _playingTime);
//
//	return lerpedPos;
//}
//
//DirectX::SimpleMath::Vector3 AnimationChannel::GetScaling(float _playingTime) const
//{
//	if (GetScaleKeyCnt() == 1)
//	{
//		return scaleKeys[0].parameter;
//	}
//
//	// 前のキーを探す
//	u_int prevKeyNum = FindPrevScaleKey(_playingTime);
//
//	// 補間を考慮した座標を取得
//	Vector3 lerpedScale = CalcInterpolatedScale(prevKeyNum, _playingTime);
//
//	return lerpedScale;
//}
//
//DirectX::SimpleMath::Quaternion AnimationChannel::GetQuaternion(float _playingTime) const
//{
//	if (GetQuatKeyCnt() == 1)
//	{
//		return quaternionKeys[0].parameter;
//	}
//
//	// 前のキーを探す
//	u_int prevKeyNum = FindPrevQuatKey(_playingTime);
//
//	// 補間を考慮した座標を取得
//	Quaternion lerpedQuat = CalcInterpolatedQuaternion(prevKeyNum, _playingTime);
//
//	return lerpedQuat;
//}

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

//DirectX::SimpleMath::Vector3 AnimationChannel::CalcInterpolatedPosition(u_int _prevKeyNum, float _playingTime) const
//{
//	// 次のキーの数
//	u_int nextKeyNum = _prevKeyNum + 1;
//
//	float nextTime = 0.0f;
//
//	if (nextKeyNum >= GetPosKeyCnt())	// キー数を超えているなら
//	{
//		nextKeyNum = 0;
//		//nextTime = アニメーションの全体時間を書く
//	}
//
//	AnimKey_V3 prevKey = positionKeys[_prevKeyNum];
//	AnimKey_V3 nextKey = positionKeys[nextKeyNum];
//
//	float deltaTime =  nextTime - prevKey.startTime;
//	float ratio = (_playingTime - prevKey.startTime) / deltaTime;
//
//	assert(ratio >= 0.0f && ratio <= 1.0f);
//
//	// 線形補間した値を返す
//	return Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);
//}
//
//DirectX::SimpleMath::Vector3 AnimationChannel::CalcInterpolatedScale(u_int _prevKeyNum, float _playingTime) const
//{
//	// 次のキーの数
//	u_int nextKeyNum = _prevKeyNum + 1;
//
//	float nextTime = 0.0f;
//
//	if (nextKeyNum >= GetScaleKeyCnt())	// キー数を超えているなら
//	{
//		nextKeyNum = 0;
//		//nextTime = アニメーションの全体時間を書く
//	}
//
//	AnimKey_V3 prevKey = scaleKeys[_prevKeyNum];
//	AnimKey_V3 nextKey = scaleKeys[nextKeyNum];
//
//	float deltaTime = nextTime - prevKey.startTime;
//	float ratio = (_playingTime - prevKey.startTime) / deltaTime;
//
//	assert(ratio >= 0.0f && ratio <= 1.0f);
//
//	// 線形補間した値を返す
//	return Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);
//}
//
//DirectX::SimpleMath::Quaternion AnimationChannel::CalcInterpolatedQuaternion(u_int _prevKeyNum, float _playingTime) const
//{
//	// 次のキーの数
//	u_int nextKeyNum = _prevKeyNum + 1;
//
//	float nextTime = 0.0f;
//
//	if (nextKeyNum >= GetQuatKeyCnt())	// キー数を超えているなら
//	{
//		nextKeyNum = 0;
//		//nextTime = アニメーションの全体時間を書く
//	}
//
//	AnimKey_Q prevKey = quaternionKeys[_prevKeyNum];
//	AnimKey_Q nextKey = quaternionKeys[nextKeyNum];
//
//	float deltaTime = nextTime - prevKey.startTime;
//	float ratio = (_playingTime - prevKey.startTime) / deltaTime;
//
//	assert(ratio >= 0.0f && ratio <= 1.0f);
//
//	// 線形補間した値を返す
//	return Quaternion::Slerp(prevKey.parameter, nextKey.parameter, ratio);
//}
