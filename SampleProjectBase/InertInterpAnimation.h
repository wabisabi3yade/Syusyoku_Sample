#pragma once
#include "Bone.h"

class AnimationData;
class SingleAnimationNode;

/// @brief  慣性補間で行う
class InertInterpAnimation
{
	/// @brief フレームごとのボーンのキャッシュ
	struct BoneCache
	{
		std::vector<BoneTransform> transform;	// ボーンのトランスフォーム
		float deltaTime_s{0.0f};	// 前フレームからの時間
		bool isEnable{ false };	// 使用しているか
	};

	/// @brief 遷移情報
	struct TransitionBase
	{
		float x0;   // 初期差分
		float v0;   // 初期速度
		float t1;   // 遷移終了の時間
		float a0;   // 初期加速度
		float A;
		float B;
		float C;
	};

	struct VectorTransition : public TransitionBase
	{
		DirectX::SimpleMath::Vector3 normalizeVector;
	};

	struct QuatTransition : public TransitionBase
	{
		DirectX::SimpleMath::Vector3 axis;  // 軸ベクトル
	};

	/// @brief 座標の遷移情報
	std::vector<VectorTransition> positionTransition;

	/// @brief スケールの遷移情報
	std::vector<VectorTransition> scaleTransition;

	/// @brief 回転の遷移情報
	std::vector<QuatTransition> rotationTransition;

	/// @brief 1フレーム前のボーンのトランスフォーム
	BoneCache lastBoneCache;

	/// @brief 2フレーム前のボーンのトランスフォーム
	BoneCache secondLastBoneCache;

	/// @brief 遷移したときのトランスフォーム
	std::vector<BoneTransform> changeTimeTransform;

public:
	InertInterpAnimation() {}
	~InertInterpAnimation() {}

	/// @brief 遷移時の計算をする
	/// @param _nextAnimation 遷移先のアニメーションの姿勢
	/// @param _blendTime ブレンド時間
	/// @return 計算成功したか？
	bool Calculate(const std::vector<BoneTransform>& _nextAnimation, float _blendTime);

	/// @brief ボーンのキャッシュを更新する
	/// @param boneTransforms 更新するボーンのトランスフォーム
	void UpdateBoneCache(std::vector<BoneTransform>& boneTransforms);

	// 各パラメータのブレンド値を計算する
	DirectX::SimpleMath::Vector3 CalcBlendPos(u_int _boneIdx, float _blendingTime);
	DirectX::SimpleMath::Vector3 CalcBlendScale(u_int _boneIdx, float _blendingTime);
	DirectX::SimpleMath::Quaternion CalcBlendRot(u_int _boneIdx, float _blendingTime);
private:

	/// @brief 座標の初期計算
	/// @param _boneIdx ボーンのID
	/// @param _blendTime 遷移時間
	void BoneInitTransition(u_int _boneIdx, const std::vector<BoneTransform>& _animData, float _blendTime);

	/// @brief Vector3型の慣性補間計算
	/// @param _transition 遷移情報
	/// @param _requestPose 遷移先のパラメータ
	/// @param _lastPose 1フレーム前のパラメータ
	/// @param _secondLastPose 2フレーム前のパラメータ
	/// @param _secondToLastDeltaTime 2フレーム前から1フレーム前までのdeltaTime
	/// @param _blendTime 遷移時間
	void InitTransition(VectorTransition& _transition, const DirectX::SimpleMath::Vector3& _requestPose, const DirectX::SimpleMath::Vector3& _lastPose, const DirectX::SimpleMath::Vector3& _secondLastPose, float _blendTime);

	/// @brief Quaternion型の慣性補間計算
	/// @param _transition 遷移情報
	/// @param _requestPose 遷移先のパラメータ
	/// @param _lastPose 1フレーム前のパラメータ
	/// @param _secondLastPose 2フレーム前のパラメータ
	/// @param _blendTime 遷移時間
	void InitTransition(QuatTransition& _transition, const DirectX::SimpleMath::Quaternion& _requestPose, const DirectX::SimpleMath::Quaternion& _lastPose, const DirectX::SimpleMath::Quaternion& _secondLastPose, float _blendTime);

	/// @brief x0とv0が決まった状態で、残りの値を計算する
	/// @param _transition 遷移情報
	/// @param _blendTime 遷移時間
	void CalcTransition(TransitionBase& _transition, float _blendTime);

	/// @brief ブレンド値を計算する（スカラー）
	/// @param _transition 遷移情報
	/// @param _blendingTime ブレンドの現在時間
	/// @return ブレンド値
	float CalcBlend(const TransitionBase& _transition, float _blendingTime);
};