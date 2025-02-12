#include "pch.h"
#include "AnimationData.h"
#include "SkeletalMesh.h"
#include "CatmulSplineInterp.h"

namespace HashiTaku
{
	using namespace DXSimp;

	AnimationData::AnimationData() :
		animationTime_s(0.0f), timePerKey_s(0.0f), allFrameCnt(0), rootBoneId(-1),
		isRightHand(false)
	{
	}

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

		// 配列内のルートモーションにあたるチャンネルを探す
		AnimationChannel* pFind = pAnimChannels[0].get();
		for (auto& c : pAnimChannels)
		{
			if (rootBoneId == c->GetBodeIdx())
			{
				pFind = c.get();
				break;
			}
		}

		// 秒速の移動速度を求める
		DXSimp::Vector3 startPos = GetRootMotionPos(0.0f);
		Vector3 endPos = GetRootMotionPos(1.0f);
		Vector3 moveDistance = endPos - startPos;
		rootMovePosPerSec = Vec3::Abs(moveDistance / animationTime_s);
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

	void AnimationData::SetAllFrameCnt(u_int _allFrameCnt)
	{
		allFrameCnt = _allFrameCnt;
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

	u_int AnimationData::GetAllAnimationFrame() const
	{
		return allFrameCnt;
	}

	u_int AnimationData::GetRatioToFrame(float _ratio)
	{
		return static_cast<u_int>(allFrameCnt * _ratio);
	}

	bool AnimationData::GetScaleByRatio(u_int _boneId, float _playingRatio, DXSimp::Vector3& _outScale) const
	{
		const AnimationChannel* channel = FindChannel(_boneId);

		// 対応したボーンがないなら
		if (channel == nullptr) return false;

		if (channel->GetScaleKeyCnt() == 1)	// 1つだと補間しない
		{
			_outScale = channel->GetScaleKey(0).parameter;
			return true;
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
		_outScale = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

		return true;
	}

	bool AnimationData::GetQuaternionByRatio(u_int _boneId, float _playingRatio, DXSimp::Quaternion& _outRot) const
	{
		const AnimationChannel* channel = FindChannel(_boneId);

		// 対応したボーンがないなら
		if (channel == nullptr) return false;

		if (channel->GetQuatKeyCnt() == 1)	// 1つだと補間しない
		{
			_outRot = channel->GetQuatKey(0).parameter;
			return true;
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

		// 球面線形補間
		_outRot = Quaternion::Slerp(prevKey.parameter, nextKey.parameter, ratio);

		return true;
	}

	bool AnimationData::GetPositionByRatio(u_int _boneId, float _playingRatio, DXSimp::Vector3& _outPos) const
	{
		const AnimationChannel* channel = FindChannel(_boneId);

		// 対応したボーンがないなら
		if (channel == nullptr) return false;

		if (channel->GetPosKeyCnt() == 1)	// 1つだと補間しない
		{
			_outPos = channel->GetPosKey(0).parameter;
			return true;
		}

		u_int prevKeyNum = channel->FindPrevPosKey(_playingRatio);
		u_int nextKeyNum = channel->GetNextPosKey(prevKeyNum);

		const AnimKey_V3& prevKey = channel->GetPosKey(prevKeyNum);
		const AnimKey_V3& nextKey = channel->GetPosKey(nextKeyNum);

		float deltaKeyNum = nextKey.startKeyNum - prevKey.startKeyNum;

		// 割合からキー数を取得
		float playingKeyNum = channel->GetPosKeyByRatio(_playingRatio);

		// 割合
		float ratio = (playingKeyNum - prevKey.startKeyNum) / deltaKeyNum;

		//// 線形補間
		_outPos = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

		return true;
	}

	bool AnimationData::GetTransformByRatio(u_int _boneId, float _playingRatio, BoneTransform& _outTransform) const
	{
		bool hasAnimation = GetPositionByRatio(_boneId, _playingRatio, _outTransform.position);
		if (!hasAnimation) return false;	// 対応したアニメーションがないなら

		GetScaleByRatio(_boneId, _playingRatio, _outTransform.scale);
		GetQuaternionByRatio(_boneId, _playingRatio, _outTransform.rotation);

		return true;
	}

	DXSimp::Vector3 AnimationData::GetScaleByKey(u_int _boneId, u_int _playingKey) const
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

	DXSimp::Quaternion AnimationData::GetQuaternioneByKey(u_int _boneId, u_int _playingKey) const
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

	DXSimp::Vector3 AnimationData::GetPositioneByKey(u_int _boneId, u_int _playingKey) const
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

	const DXSimp::Vector3& AnimationData::GetRootMotionPosSpeedPerSec() const
	{
		return rootMovePosPerSec;
	}

	DXSimp::Vector3 AnimationData::GetRootMotionPos(float _ratio) const
	{
		const AnimationChannel* pRootChannel = FindChannel(rootBoneId);

		if (pRootChannel->GetPosKeyCnt() == 1)	// 1つだと補間しない
		{
			return pRootChannel->GetPosKey(0).parameter;
		}

		u_int prevKeyNum = pRootChannel->FindPrevPosKey(_ratio);
		u_int nextKeyNum = pRootChannel->GetNextPosKey(prevKeyNum);

		const AnimKey_V3& prevKey = pRootChannel->GetPosKey(prevKeyNum);
		const AnimKey_V3& nextKey = pRootChannel->GetPosKey(nextKeyNum);

		float deltaKeyNum = nextKey.startKeyNum - prevKey.startKeyNum;

		// 割合からキー数を取得
		float playingKeyNum = pRootChannel->GetPosKeyByRatio(_ratio);

		// 割合
		float ratio = (playingKeyNum - prevKey.startKeyNum) / deltaKeyNum;

		//// 線形補間
		Vector3 calcPos = Vector3::Lerp(prevKey.parameter, nextKey.parameter, ratio);

		return calcPos;
	}

	DXSimp::Quaternion AnimationData::GetRootMotionRot(float _ratio) const
	{
		const AnimationChannel* pRootChannel = FindChannel(rootBoneId);

		if (pRootChannel->GetQuatKeyCnt() == 1)	// 1つだと補間しない
		{
			return pRootChannel->GetQuatKey(0).parameter;
		}

		u_int prevKeyNum = pRootChannel->FindPrevQuatKey(_ratio);
		u_int nextKeyNum = pRootChannel->GetNextQuatKey(prevKeyNum);

		const AnimKey_Q& prevKey = pRootChannel->GetQuatKey(prevKeyNum);
		const AnimKey_Q& nextKey = pRootChannel->GetQuatKey(nextKeyNum);

		float deltaKeyNum = nextKey.startKeyNum - prevKey.startKeyNum;

		// 割合からキー数を取得
		float playingKeyNum = pRootChannel->GetQuatKeyByRatio(_ratio);

		// 割合
		float ratio = (playingKeyNum - prevKey.startKeyNum) / deltaKeyNum;

		// 球面線形補間
		Quaternion calcQuat = Quaternion::Slerp(prevKey.parameter, nextKey.parameter, ratio);

		return calcQuat;
	}

	float AnimationData::GetAnimationTime() const
	{
		return animationTime_s;
	}

	json AnimationData::Save()
	{
		auto data = AssetPath_Base::Save();

		data["boneListName"] = boneListName;
		data["rightHand"] = isRightHand;

		return data;
	}
}