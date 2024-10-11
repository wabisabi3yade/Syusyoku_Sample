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
	void Update(std::vector<BoneTransform>& _outTransforms) override;

	/// @brief ルートモーションの座標移動速度を計算する
	/// @param _controllerSpeed コントローラー速度
	void CalcRootMotionPosSpeed(float _controllerSpeed) override;

	/// @brief ルートモーションを取得する（内部で必要な計算を行う）
	/// @param 現在の再生割合
	/// @param _isLoadScaling ロード時のスケールを反映するか
	/// @return 現在の割合のルートモーション座標
	DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio, bool _isLoadScaling = true) const override;

	/// @brief ルートモーションを取得する（内部で必要な計算を行う）
	/// @param 現在の再生割合
	/// @return 現在の割合のルートモーション回転量
	DirectX::SimpleMath::Quaternion GetRootMotionRot(float _ratio, bool _isLoadScaling = true) const override;

protected:
	/// @brief モデルの大きさが原寸の大きさからどれだけの倍率なのか計算する
	/// @return 原寸からのモデルスケール倍率
	float GetModelScale() const;
};

