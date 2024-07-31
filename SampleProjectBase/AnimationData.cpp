#include "pch.h"
#include "AnimationData.h"

using namespace DirectX::SimpleMath;

void AnimationData::AddAnimationChannel(std::unique_ptr<AnimationChannel> _pAnimNode)
{
	pAnimChannels.push_back(std::move(_pAnimNode));
}

std::string AnimationData::GetBoneName(u_int _nodeId)
{
	assert(_nodeId < static_cast<u_int>(pAnimChannels.size()) && "ノードIDが配列外を指しています");

	return pAnimChannels[_nodeId]->GetName();
}

u_int AnimationData::GetChannelCount() const
{
	return static_cast<u_int>(pAnimChannels.size());
}

DirectX::SimpleMath::Vector3 AnimationData::GetScale(u_int _nodeId, float _playingTime) const
{
	AnimationChannel& channel =  *pAnimChannels[_nodeId];

	if (channel.GetScaleKeyCnt() == 1)	// 1つだと補間しない
	{
		return channel.GetScaleKey(0).parameter;
	}

	u_int prevKeyNum = channel.FindPrevScaleKey(_playingTime);
	u_int nextKeyNum = prevKeyNum + 1;

	if (nextKeyNum >= channel.GetScaleKeyCnt())
		nextKeyNum = 0;

	const AnimKey_V3& prevKey = channel.GetScaleKey(prevKeyNum);
	const AnimKey_V3& nextKey = channel.GetScaleKey(nextKeyNum);

	float deltaTime = nextKey.startTime - prevKey.startTime;

	// 割合
	float ratio = (_playingTime - prevKey.startTime) / deltaTime;

	// 線形補間
	Vector3 lerpedScale = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

	return lerpedScale;
}

DirectX::SimpleMath::Quaternion AnimationData::GetQuaternion(u_int _nodeId, float _playingTime) const
{
	AnimationChannel& channel = *pAnimChannels[_nodeId];

	if (channel.GetQuatKeyCnt() == 1)	// 1つだと補間しない
	{
		return channel.GetQuatKey(0).parameter;
	}

	u_int prevKeyNum = channel.FindPrevQuatKey(_playingTime);
	u_int nextKeyNum = prevKeyNum + 1;

	if (nextKeyNum >= channel.GetQuatKeyCnt())
		nextKeyNum = 0;

	const AnimKey_Q& prevKey = channel.GetQuatKey(prevKeyNum);
	const AnimKey_Q& nextKey = channel.GetQuatKey(nextKeyNum);

	float deltaTime = nextKey.startTime - prevKey.startTime;

	// 割合
	float ratio = (_playingTime - prevKey.startTime) / deltaTime;

	// 球面線形補間
	Quaternion slerpedQuat = Quaternion::Slerp(prevKey.parameter, nextKey.parameter, ratio);

	return slerpedQuat;
}

DirectX::SimpleMath::Vector3 AnimationData::GetPosition(u_int _nodeId, float _playingTime) const
{
	AnimationChannel& channel = *pAnimChannels[_nodeId];

	if (channel.GetPosKeyCnt() == 1)	// 1つだと補間しない
	{
		return channel.GetPosKey(0).parameter;
	}

	u_int prevKeyNum = channel.FindPrevPosKey(_playingTime);
	u_int nextKeyNum = prevKeyNum + 1;

	if (nextKeyNum >= channel.GetPosKeyCnt())
		nextKeyNum = 0;

	const AnimKey_V3& prevKey = channel.GetPosKey(prevKeyNum);
	const AnimKey_V3& nextKey = channel.GetPosKey(nextKeyNum);

	float deltaTime = nextKey.startTime - prevKey.startTime;

	// 割合
	float ratio = (_playingTime - prevKey.startTime) / deltaTime;

	// 線形補間
	Vector3 lerpedPos = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

	return lerpedPos;
}

float AnimationData::GetAnimationTime() const
{
	return animationTime;
}
