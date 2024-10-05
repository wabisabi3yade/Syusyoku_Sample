#include "pch.h"
#include "AnimSingleNodePlayer.h"
#include "SingleAnimationNode.h"

AnimSingleNodePlayer::AnimSingleNodePlayer(const AnimationNode_Base& _singleNode, BoneList& _boneList, Transform& _transform)
	: AnimNodePlayer_Base(_singleNode, _boneList, _transform)
{
}

void AnimSingleNodePlayer::Update()
{
	float playingRatio = GetCurPlayRatio();

	//ボーン数ループ
	for (unsigned int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		Bone& bone = pBoneList->GetBone(b_i);
		BoneTransform transform;

		// 再生時間から各パラメータを取得
		pPlayAnimNode->GetAnimTransform(transform, b_i, playingRatio);

		bone.SetAnimTransform(transform);
	}
}

DirectX::SimpleMath::Vector3 AnimSingleNodePlayer::GetRootMotionPos(float _ratio)
{
	using namespace DirectX::SimpleMath;

	const SingleAnimationNode& singleNode = static_cast<const SingleAnimationNode&>(*pPlayAnimNode);
	DirectX::SimpleMath::Vector3 rootMotionPos = singleNode.GetAnimationData().GetRootMotionPos(_ratio);

	// ロード時の回転量を求める
	Matrix rotateMtx = Matrix::CreateFromQuaternion(pBoneList->GetLoadRotation());
	rootMotionPos = Vector3::Transform(rootMotionPos, rotateMtx);

	static float s = 0.001f;
	ImGui::Begin("aa");
	ImGui::DragFloat("a", &s, 0.0001f);
	ImGui::End();
	rootMotionPos *= s;
	/*rootMotionPos *= pBoneList->GetLoadScale();*/
	return rootMotionPos;
}

DirectX::SimpleMath::Quaternion AnimSingleNodePlayer::GetRootMotionRot(float _ratio)
{
	const SingleAnimationNode& singleNode = static_cast<const SingleAnimationNode&>(*pPlayAnimNode);
	return singleNode.GetAnimationData().GetRootMotionRot(_ratio);
}

float AnimSingleNodePlayer::GetModelScale() const
{ 
	return pBoneList->GetLoadScale();
}
