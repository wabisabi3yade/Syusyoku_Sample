#include "pch.h"
#include "AnimSingleNodePlayer.h"
#include "SingleAnimationNode.h"

namespace HashiTaku
{
	AnimSingleNodePlayer::AnimSingleNodePlayer(const AnimationNode_Base& _singleNode,
		BoneList& _boneList, Transform& _transform) :
		AnimNodePlayer_Base(_singleNode, _boneList, _transform),
		playingFrame(0)
	{
	}

	u_int AnimSingleNodePlayer::GetAllFrame() const
	{
		const SingleAnimationNode& singleNode =
			static_cast<const SingleAnimationNode&>(*pPlayAnimNode);

		u_int allKeyFrame = singleNode.GetAllKeyFrame();

		return allKeyFrame;
	}

	void AnimSingleNodePlayer::Update(std::vector<BoneTransform>& _outTransforms)
	{
		float playingRatio = GetCurPlayRatio();

		// �Đ��L�[�ɔ��f
		playingFrame = static_cast<u_int>(playingRatio * GetAllFrame());

		// �{�[���̃g�����X�t�H�[�����擾
		const SingleAnimationNode& singleNode = static_cast<const SingleAnimationNode&>(*pPlayAnimNode);
		_outTransforms.resize(pAssetBoneList->GetBoneCnt());
		for (unsigned int b_i = 0; b_i < pAssetBoneList->GetBoneCnt(); b_i++)
		{
			// �Đ����Ԃ���e�p�����[�^���擾
			singleNode.GetAnimTransform(_outTransforms[b_i], b_i, playingRatio);
		}
	}

	void AnimSingleNodePlayer::CalcRootMotionPosSpeed()
	{
		// �R���g���[���[��
		const SingleAnimationNode& singleNode = static_cast<const SingleAnimationNode&>(*pPlayAnimNode);

		float nodePlaySpeed = std::max(GetNodePlaySpeed(), Mathf::epsilon);

		rootMotionPosSpeedPerSec =
			singleNode.GetRootMotionPosSpeed() *
			pObjectTransform->GetScale() *
			pAssetBoneList->GetLoadScale() *
			(allPlaySpeed / nodePlaySpeed);
	}

	DirectX::SimpleMath::Vector3 AnimSingleNodePlayer::GetRootMotionPos(float _ratio, bool _isWorldScaling) const
	{
		using namespace DirectX::SimpleMath;

		const SingleAnimationNode& singleNode = static_cast<const SingleAnimationNode&>(*pPlayAnimNode);

		DirectX::SimpleMath::Vector3 rootMotionPos = singleNode.GetAnimationData().GetRootMotionPos(_ratio);

		// ���[�h���̉�]�ʂƁA�X�P�[�����|����
		if (_isWorldScaling)
			ApplyLoadTransform(rootMotionPos);


		return rootMotionPos;
	}

	float AnimSingleNodePlayer::GetModelScale() const
	{
		return pAssetBoneList->GetLoadScale();
	}

	void AnimSingleNodePlayer::ImGuiDebug()
	{
		AnimNodePlayer_Base::ImGuiDebug();

		// �Đ��L�[��ҏW
		int imKey = static_cast<int>(playingFrame);
		if (ImGui::SliderInt("Key", &imKey, 0, GetAllFrame()))
		{
			SetPlayingKey(imKey);
		}
	}

	void AnimSingleNodePlayer::SetPlayingKey(u_int _playKey)
	{
		u_int allKey = GetAllFrame();
		playingFrame = _playKey;

		// �L�[������Đ����������߂�
		float playingRatio = static_cast<float>(playingFrame) / allKey;
		SetCurPlayRatio(playingRatio);
	}

	u_int AnimSingleNodePlayer::GetPlayKey() const
	{
		return playingFrame;
	}
}