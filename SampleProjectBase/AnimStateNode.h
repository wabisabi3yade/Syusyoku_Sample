#pragma once

#include "AnimationData.h"

// ボーンのトランスフォーム
struct BoneTransform
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Quaternion rotation;
};

/// @brief フレームごとのボーンのキャッシュ
struct BoneCache
{
	std::vector<BoneTransform> transform;	// ボーンのトランスフォーム
	float deltaTime_s;	// 前フレームからの時間
	bool isEnable;	// 使用しているか
};

// アニメーション遷移で使用する1ノードクラス
class AnimStateNode
{
	/// @brief アニメーションデータ
	AnimationData* pAnimationData;

	/// @brief 1フレーム前のボーンのトランスフォーム
	BoneCache lastBoneCache;

	/// @brief 2フレーム前のボーンのトランスフォーム
	BoneCache secondLastBoneCache;
public:
	AnimStateNode() : pAnimationData(nullptr) {}
	~AnimStateNode() {}

	/// @brief アニメーション
	/// @param _animData アニメーションデータ
	void SetAnimationData(AnimationData& _animData);

	/// @brief フレームごとのボーンのキャッシュを更新する
	/// @param boneTransforms ボーンのトランスフォーム
	/// @param _deltaTime 前回フレームからの時間(s)
	void UpdateCache(std::vector<BoneTransform>& boneTransforms, float _deltaTime);

	// アニメーションデータを取得する
	const AnimationData& GetAnimationData();

	/// @brief アニメーション遷移で補間したボーンのトランスフォームを返す
	/// @param _nextAnimation 次のアニメーションノード
	/// @param _blendTime 遷移する時間
	/// @return ボーンのトランスフォーム
	std::vector<BoneTransform> CalcTranslation(AnimStateNode& _nextAnimation, float _blendTime);

	/// @brief 次のアニメーションまでを慣性補間で遷移する
	/// @param _nextAnimation 次のアニメーション
	/// @param _blendTime 遷移する時間
	/// @return ボーンのトランスフォーム
	std::vector<BoneTransform> InertInterpolation(AnimStateNode& _nextAnimation, float _blendTime);

	BoneTransform CalcInertTransform();
};

