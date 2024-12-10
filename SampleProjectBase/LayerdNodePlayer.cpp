#include "pch.h"
#include "LayerdNodePlayer.h"

LayerdNodePlayer::LayerdNodePlayer(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform) :
	AnimNodePlayer_Base(_playNode, _boneList, _transform)
{
}

void LayerdNodePlayer::Update(std::vector<BoneTransform>& _outTransforms)
{
	_outTransforms.resize(pAssetBoneList->GetBoneCnt());

	//ボーン数ループ
	for (unsigned int b_i = 0; b_i < pAssetBoneList->GetBoneCnt(); b_i++)
	{
		// 再生時間から各パラメータを取得
		pPlayAnimNode->GetAnimTransform(_outTransforms[b_i], b_i, GetCurPlayRatio());
	}
}

void LayerdNodePlayer::CalcRootMotionPosSpeed()
{

}

void LayerdNodePlayer::ImGuiDebug()
{
	AnimNodePlayer_Base::ImGuiDebug();
}
