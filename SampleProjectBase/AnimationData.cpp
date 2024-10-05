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

void AnimationData::CalcRootMotion(u_int _rootBoneId)
{
	rootBoneId = _rootBoneId;

	// �z����̃��[�g���[�V�����ɂ�����`�����l����T��
	AnimationChannel* pFind = pAnimChannels[0].get();
	for (auto& c : pAnimChannels)
	{
		if (rootBoneId == c->GetBodeIdx())
		{
			pFind = c.get();
			break;
		}
	}

	// ���[�g���[�V�����`�����l���ɃR�s�[
	pRootMotionChannels = std::make_unique<AnimationChannel>(*pFind);
	// ���X�̃��[�g���[�V�����`�����l���̈ړ���0�ɂ���
	/*pFind->ResetKeys();*/

	// �b���̈ړ����x�����߂�
	Vector3 startPos = GetRootMotionPos(0.0f);
	Vector3 endPos = GetRootMotionPos(1.0f);
	Vector3 moveDistance = endPos - startPos;
	rootMovePosPerSec = moveDistance / animationTime_s;
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
	assert(_nodeId < static_cast<u_int>(pAnimChannels.size()) && "�m�[�hID���z��O���w���Ă��܂�");

	return pAnimChannels[_nodeId]->GetName();
}

u_int AnimationData::GetBoneIdx(u_int _nodeId) const
{
	assert(_nodeId < static_cast<u_int>(pAnimChannels.size()) && "�m�[�hID���z��O���w���Ă��܂�");

	return pAnimChannels[_nodeId]->GetBodeIdx();
}

u_int AnimationData::GetChannelCount() const
{
	return static_cast<u_int>(pAnimChannels.size());
}

DirectX::SimpleMath::Vector3 AnimationData::GetScaleByRatio(u_int _boneId, float _playingRatio) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	// �Ή������{�[�����Ȃ��Ȃ�
	if (channel == nullptr) return Vector3::One;

	if (channel->GetScaleKeyCnt() == 1)	// 1���ƕ�Ԃ��Ȃ�
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

	// ��������L�[�����擾
	float playingKeyNum = channel->GetScaleKeyByRatio(_playingRatio);

	// ����
	float ratio = (playingKeyNum - prevKey.startKeyNum) / deltaKeyNum;

	// ���`���
	Vector3 lerpedScale = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

	return lerpedScale;
}

DirectX::SimpleMath::Quaternion AnimationData::GetQuaternionByRatio(u_int _boneId, float _playingRatio) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	// �Ή������{�[�����Ȃ��Ȃ�
	if (channel == nullptr) return Quaternion::Identity;

	if (channel->GetQuatKeyCnt() == 1)	// 1���ƕ�Ԃ��Ȃ�
	{
		return channel->GetQuatKey(0).parameter;
	}

	u_int prevKeyNum = channel->FindPrevQuatKey(_playingRatio);
	u_int nextKeyNum = channel->GetNextQuatKey(prevKeyNum);

	const AnimKey_Q& prevKey = channel->GetQuatKey(prevKeyNum);
	const AnimKey_Q& nextKey = channel->GetQuatKey(nextKeyNum);

	float deltaKeyNum = nextKey.startKeyNum - prevKey.startKeyNum;

	// ��������L�[�����擾
	float playingKeyNum = channel->GetQuatKeyByRatio(_playingRatio);

	// ����
	float ratio = (playingKeyNum - prevKey.startKeyNum) / deltaKeyNum;

	/*u_int prevprevKeyNum = channel->GetNextQuatKey(prevKeyNum, -1);
	u_int nextnextKeyNum = channel->GetNextQuatKey(prevKeyNum, 2);

	const AnimKey_Q& prevprevKey = channel->GetQuatKey(prevprevKeyNum);
	const AnimKey_Q& nextnextKey = channel->GetQuatKey(nextnextKeyNum);

	Quaternion calcQuat = CatmulSplineInterp::CalcQuaternion(prevKey.parameter, nextKey.parameter, prevprevKey.parameter, nextnextKey.parameter, ratio);*/

	// ���ʐ��`���
	Quaternion calcQuat = Quaternion::Slerp(prevKey.parameter, nextKey.parameter, ratio);

	return calcQuat;
}

DirectX::SimpleMath::Vector3 AnimationData::GetPositionByRatio(u_int _boneId, float _playingRatio) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	// �Ή������{�[�����Ȃ��Ȃ�
	if (channel == nullptr) return Vector3::Zero;

	if (channel->GetPosKeyCnt() == 1)	// 1���ƕ�Ԃ��Ȃ�
	{
		return channel->GetPosKey(0).parameter;
	}

	u_int prevKeyNum = channel->FindPrevPosKey(_playingRatio);
	u_int nextKeyNum = channel->GetNextPosKey(prevKeyNum);

	const AnimKey_V3& prevKey = channel->GetPosKey(prevKeyNum);
	const AnimKey_V3& nextKey = channel->GetPosKey(nextKeyNum);

	float deltaKeyNum = nextKey.startKeyNum - prevKey.startKeyNum;

	// ��������L�[�����擾
	float playingKeyNum = channel->GetPosKeyByRatio(_playingRatio);

	// ����
	float ratio = (playingKeyNum - prevKey.startKeyNum) / deltaKeyNum;

	/*u_int prevprevKeyNum = channel->GetNextPosKey(prevKeyNum, -1);
	u_int nextnextKeyNum = channel->GetNextPosKey(prevKeyNum, 2);

	const AnimKey_V3& prevprevKey = channel->GetPosKey(prevprevKeyNum);
	const AnimKey_V3& nextnextKey = channel->GetPosKey(nextnextKeyNum);

	Vector3 calcPos = CatmulSplineInterp::CalcVector3(prevKey.parameter, nextKey.parameter, prevprevKey.parameter, nextnextKey.parameter, ratio);*/

	//// ���`���
	Vector3 calcPos = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

	return calcPos;
}

void AnimationData::GetTransformByRatio(u_int _boneId, float _playingRatio, BoneTransform& _outTransform) const
{
	_outTransform.position = GetPositionByRatio(_boneId, _playingRatio);
	_outTransform.scale = GetScaleByRatio(_boneId, _playingRatio);
	_outTransform.rotation = GetQuaternionByRatio(_boneId, _playingRatio);
}

DirectX::SimpleMath::Vector3 AnimationData::GetScaleByKey(u_int _boneId, u_int _playingKey) const
{
	const AnimationChannel* channel = FindChannel(_boneId);

	if (!channel) return Vector3::One;
	if (_playingKey > channel->GetScaleKeyCnt() - 1)
	{
		HASHI_DEBUG_LOG("�w�肵���L�[���ő吔�𒴂��Ă��܂�");
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
		HASHI_DEBUG_LOG("�w�肵���L�[���ő吔�𒴂��Ă��܂�");
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
		HASHI_DEBUG_LOG("�w�肵���L�[���ő吔�𒴂��Ă��܂�");
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

const DirectX::SimpleMath::Vector3& AnimationData::GetRootMotionPosSpeedPerSec() const
{
	return rootMovePosPerSec;
}

DirectX::SimpleMath::Vector3 AnimationData::GetRootMotionPos(float _ratio) const
{
	if (pRootMotionChannels->GetPosKeyCnt() == 1)	// 1���ƕ�Ԃ��Ȃ�
	{
		return pRootMotionChannels->GetPosKey(0).parameter;
	}

	u_int prevKeyNum = pRootMotionChannels->FindPrevPosKey(_ratio);
	u_int nextKeyNum = pRootMotionChannels->GetNextPosKey(prevKeyNum);

	const AnimKey_V3& prevKey = pRootMotionChannels->GetPosKey(prevKeyNum);
	const AnimKey_V3& nextKey = pRootMotionChannels->GetPosKey(nextKeyNum);

	float deltaKeyNum = nextKey.startKeyNum - prevKey.startKeyNum;

	// ��������L�[�����擾
	float playingKeyNum = pRootMotionChannels->GetPosKeyByRatio(_ratio);

	// ����
	float ratio = (playingKeyNum - prevKey.startKeyNum) / deltaKeyNum;

	//// ���`���
	Vector3 calcPos = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

	return calcPos;
}

DirectX::SimpleMath::Quaternion AnimationData::GetRootMotionRot(float _ratio) const
{
	if (pRootMotionChannels->GetQuatKeyCnt() == 1)	// 1���ƕ�Ԃ��Ȃ�
	{
		return pRootMotionChannels->GetQuatKey(0).parameter;
	}

	u_int prevKeyNum = pRootMotionChannels->FindPrevQuatKey(_ratio);
	u_int nextKeyNum = pRootMotionChannels->GetNextQuatKey(prevKeyNum);

	const AnimKey_Q& prevKey = pRootMotionChannels->GetQuatKey(prevKeyNum);
	const AnimKey_Q& nextKey = pRootMotionChannels->GetQuatKey(nextKeyNum);

	float deltaKeyNum = nextKey.startKeyNum - prevKey.startKeyNum;

	// ��������L�[�����擾
	float playingKeyNum = pRootMotionChannels->GetQuatKeyByRatio(_ratio);

	// ����
	float ratio = (playingKeyNum - prevKey.startKeyNum) / deltaKeyNum;

	// ���ʐ��`���
	Quaternion calcQuat = Quaternion::Slerp(prevKey.parameter, nextKey.parameter, ratio);

	return calcQuat;
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
