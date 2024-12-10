#pragma once
#include "AnimationNode_Base.h"

/// @brief 部位ごとに分けてアニメーションするノード
class LayerdAnimationNode : public AnimationNode_Base
{
	// 今回上半身、下半身の再生だけをするので2個だけ対応します、必要になったら拡張

	/// @brief 基礎となるアニメーション
	AnimationData* pBaseAnimation;

	/// @brief ブレンドするアニメーション
	AnimationData* pBlendAnimation;

	/// @brief どこからブレンドアニメーションを再生するか指定するボーンID
	int beginBlendBoneId;
public:
	/// @brief コンストラクタ
	/// @param _nodeName ノード名
	LayerdAnimationNode(const std::string& _nodeName);
	~LayerdAnimationNode() {}


};

