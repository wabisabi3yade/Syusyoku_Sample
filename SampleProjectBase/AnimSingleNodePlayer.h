#pragma once
#include "AnimNodePlayer_Base.h"

class SingleAnimationNode;

/// @brief 単一アニメーションのノード再生する
class AnimSingleNodePlayer : public AnimNodePlayer_Base
{
public:
	AnimSingleNodePlayer(const AnimationNode_Base& _singleNode, BoneList& _boneList, Transform& _transform);
	~AnimSingleNodePlayer() {}

private:
	void Update() override;

	/// @brief ルートモーションを取得する（内部で必要な計算を行う）
	/// @param 現在の再生割合
	/// @return 現在の割合のルートモーション座標
	DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio) override;

	/// @brief ルートモーションを取得する（内部で必要な計算を行う）
	/// @param 現在の再生割合
	/// @return 現在の割合のルートモーション回転量
	DirectX::SimpleMath::Quaternion GetRootMotionRot(float _ratio) override;

protected:
	/// @brief モデルの大きさが原寸の大きさからどれだけの倍率なのか計算する
	/// @return 原寸からのモデルスケール倍率
	float GetModelScale() const;
};

