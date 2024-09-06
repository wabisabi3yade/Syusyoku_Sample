#include "pch.h"
#include "AnimationData.h"

#include "SkeletalMesh.h"

#include "CatmulSplineInterp.h"

using namespace DirectX::SimpleMath;

void AnimationData::AddAnimationChannel(std::unique_ptr<AnimationChannel> _pAnimNode)
{
	pAnimChannels.push_back(std::move(_pAnimNode));
}

const AnimationChannel* AnimationData::FindChannel(u_int _boneIdx) const
{
	auto itr = std::find_if(pAnimChannels.begin(), pAnimChannels.end(),
		[&](const std::unique_ptr<AnimationChannel>& channel)
		{
			return channel->GetBodeIdx() == _boneIdx;
		});

	if (itr == pAnimChannels.end())
	{
		return nullptr;
	}

	return (*itr).get();
}

void AnimationData::SetBoneListName(const std::string& _boneListName)
{
	boneListName = _boneListName;
}

void AnimationData::SetAnimationTime(float _animTime)
{
	assert(_animTime >= 0.0f);
	animationTime_s = std::max(_animTime, 0.0f);
}

void AnimationData::SetTimePerKey(float _timePerKey)
{
	assert(_timePerKey >= 0.0f);
	timePerKey_s = std::max(_timePerKey, 0.0f);
}

void AnimationData::SetIsRightHand(bool _isRightHand)
{
	isRightHand = _isRightHand;
}

std::string AnimationData::GetBoneName(u_int _nodeId) const
{
	assert(_nodeId < static_cast<u_int>(pAnimChannels.size()) && "ノードIDが配列外を指しています");

	return pAnimChannels[_nodeId]->GetName();
}

u_int AnimationData::GetBoneIdx(u_int _nodeId) const
{
	assert(_nodeId < static_cast<u_int>(pAnimChannels.size()) && "ノードIDが配列外を指しています");

	return pAnimChannels[_nodeId]->GetBodeIdx();
}

u_int AnimationData::GetChannelCount() const
{
	return static_cast<u_int>(pAnimChannels.size());
}

DirectX::SimpleMath::Vector3 AnimationData::GetScaleByRatio(u_int _boneId, float _playingRatio) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	// 対応したボーンがないなら
	if (channel == nullptr) return Vector3::One;

	if (channel->GetScaleKeyCnt() == 1)	// 1つだと補間しない
	{
		return channel->GetScaleKey(0).parameter;
	}

	u_int prevKeyNum = channel->FindPrevScaleKey(_playingRatio);
	u_int nextKeyNum = prevKeyNum + 1;

	if (nextKeyNum >= channel->GetScaleKeyCnt())
		nextKeyNum = 0;

	const AnimKey_V3& prevKey = channel->GetScaleKey(prevKeyNum);
	const AnimKey_V3& nextKey = channel->GetScaleKey(nextKeyNum);

	float deltaKeyNum = nextKey.startKeyNum - prevKey.startKeyNum;

	// 割合からキー数を取得
	float playingKeyNum = channel->GetScaleKeyByRatio(_playingRatio);

	// 割合
	float ratio = (playingKeyNum - prevKey.startKeyNum) / deltaKeyNum;

	// 線形補間
	Vector3 lerpedScale = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

	return lerpedScale;
}

DirectX::SimpleMath::Quaternion AnimationData::GetQuaternionByRatio(u_int _boneId, float _playingRatio) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	// 対応したボーンがないなら
	if (channel == nullptr) return Quaternion::Identity;

	if (channel->GetQuatKeyCnt() == 1)	// 1つだと補間しない
	{
		return channel->GetQuatKey(0).parameter;
	}

	u_int prevKeyNum = channel->FindPrevQuatKey(_playingRatio);
	u_int nextKeyNum = channel->GetNextQuatKey(prevKeyNum);

	const AnimKey_Q& prevKey = channel->GetQuatKey(prevKeyNum);
	const AnimKey_Q& nextKey = channel->GetQuatKey(nextKeyNum);

	float deltaKeyNum = nextKey.startKeyNum - prevKey.startKeyNum;

	// 割合からキー数を取得
	float playingKeyNum = channel->GetQuatKeyByRatio(_playingRatio);

	// 割合
	float ratio = (playingKeyNum - prevKey.startKeyNum) / deltaKeyNum;

	/*u_int prevprevKeyNum = channel->GetNextQuatKey(prevKeyNum, -1);
	u_int nextnextKeyNum = channel->GetNextQuatKey(prevKeyNum, 2);

	const AnimKey_Q& prevprevKey = channel->GetQuatKey(prevprevKeyNum);
	const AnimKey_Q& nextnextKey = channel->GetQuatKey(nextnextKeyNum);

	Quaternion calcQuat = CatmulSplineInterp::CalcQuaternion(prevKey.parameter, nextKey.parameter, prevprevKey.parameter, nextnextKey.parameter, ratio);*/

	// 球面線形補間
	Quaternion calcQuat = Quaternion::Slerp(prevKey.parameter, nextKey.parameter, ratio);

	return calcQuat;
}

DirectX::SimpleMath::Vector3 AnimationData::GetPositionByRatio(u_int _boneId, float _playingRatio) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	// 対応したボーンがないなら
	if (channel == nullptr) return Vector3::Zero;

	if (channel->GetPosKeyCnt() == 1)	// 1つだと補間しない
	{
		return channel->GetPosKey(0).parameter;
	}

	u_int prevKeyNum = channel->FindPrevPosKey(_playingRatio);
	u_int nextKeyNum = channel->GetNextPosKey(prevKeyNum);

	const AnimKey_V3& prevKey = channel->GetPosKey(prevKeyNum);
	const AnimKey_V3& nextKey = channel->GetPosKey(nextKeyNum);

	float deltaKeyNum = nextKey.startKeyNum - prevKey.startKeyNum;

	// 割合からキー数を取得
	float playingKeyNum = channel->GetPosKeyByRatio(_playingRatio);

	// 割合
	float ratio = (playingKeyNum - prevKey.startKeyNum ) / deltaKeyNum;

	/*u_int prevprevKeyNum = channel->GetNextPosKey(prevKeyNum, -1);
	u_int nextnextKeyNum = channel->GetNextPosKey(prevKeyNum, 2);

	const AnimKey_V3& prevprevKey = channel->GetPosKey(prevprevKeyNum);
	const AnimKey_V3& nextnextKey = channel->GetPosKey(nextnextKeyNum);

	Vector3 calcPos = CatmulSplineInterp::CalcVector3(prevKey.parameter, nextKey.parameter, prevprevKey.parameter, nextnextKey.parameter, ratio);*/

	//// 線形補間
	Vector3 calcPos = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

	return calcPos;
}

BoneTransform AnimationData::GetTransformByRatio(u_int _boneId, float _playingRatio) const
{
	BoneTransform retTransform;

	retTransform.position = GetPositionByRatio(_boneId, _playingRatio);
	retTransform.scale = GetScaleByRatio(_boneId, _playingRatio);
	retTransform.rotation = GetQuaternionByRatio(_boneId, _playingRatio);

	return retTransform;
}

DirectX::SimpleMath::Vector3 AnimationData::GetScaleByKey(u_int _boneId, u_int _playingKey) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	if (!channel) return Vector3::One;
	if (_playingKey > channel->GetScaleKeyCnt() - 1)
	{
		HASHI_DEBUG_LOG("指定したキーが最大数を超えています");
		_playingKey = channel->GetScaleKeyCnt();
	}

	return channel->GetScaleKey(_playingKey).parameter;
}

DirectX::SimpleMath::Quaternion AnimationData::GetQuaternioneByKey(u_int _boneId, u_int _playingKey) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	if (!channel) return Quaternion::Identity;
	if (_playingKey > channel->GetQuatKeyCnt() - 1)
	{
		HASHI_DEBUG_LOG("指定したキーが最大数を超えています");
		_playingKey = channel->GetQuatKeyCnt();
	}

	return channel->GetQuatKey(_playingKey).parameter;
}

DirectX::SimpleMath::Vector3 AnimationData::GetPositioneByKey(u_int _boneId, u_int _playingKey) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	if (!channel) return Vector3::Zero;
	if (_playingKey > channel->GetPosKeyCnt() - 1)
	{
		HASHI_DEBUG_LOG("指定したキーが最大数を超えています");
		_playingKey = channel->GetPosKeyCnt();
	}

	return channel->GetPosKey(_playingKey).parameter;
}

BoneTransform AnimationData::GetTransformByKey(u_int _boneId, u_int _playingKey) const
{
	BoneTransform boneTransform;

	boneTransform.position = GetPositioneByKey(_boneId, _playingKey);
	boneTransform.scale = GetScaleByKey(_boneId, _playingKey);
	boneTransform.rotation = GetQuaternioneByKey(_boneId, _playingKey);

	return boneTransform;
}

float AnimationData::GetAnimationTime() const
{
	return animationTime_s;
}

nlohmann::json AnimationData::Save()
{
	auto data = AssetPath_Base::Save();

	data["boneListName"] = boneListName;
	data["rightHand"] = isRightHand;

	return data;
}
