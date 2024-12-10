#pragma once
#include "AnimNodePlayer_Base.h"

/// @brief レイヤーノードの再生クラス
class LayerdNodePlayer : public AnimNodePlayer_Base
{

	
public:
	LayerdNodePlayer(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform);
	~LayerdNodePlayer() {}

	/// @brief アニメーションの更新処理
	/// @param _outTransforms アニメーションの姿勢 
	void Update(std::vector<BoneTransform>& _outTransforms) override;

	/// @brief ルートモーションの座標移動速度を計算する
	void CalcRootMotionPosSpeed() override;

private:
	void ImGuiDebug() override;
};

