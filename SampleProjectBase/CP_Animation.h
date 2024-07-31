#pragma once
#include "Component.h"

// モデル
class SkeletalMesh;
class Bone;
class TreeNode;

// アニメーションデータ
class AnimationData;

constexpr u_int MAX_BONEMTX(400);	// シェーダーの渡すボーン行列の最大数

/// @brief アニメーションコンポーネント
class CP_Animation : public Component
{
	// シェーダーに渡すボーン行列構造体
	struct BoneCombMtricies
	{
		DirectX::SimpleMath::Matrix matrix[MAX_BONEMTX];
	};

	BoneCombMtricies boneComb;

	/// @brief スケルタルメッシュ
	SkeletalMesh* pSkeletalMesh;

	/// @brief 現在のアニメーション
	AnimationData* pCurrentAnimation;

	/// @brief このコンポーネントにあるアニメーション
	std::list<AnimationData*> pHaveAnimations;

	/// @brief 現在再生しているアニメーションの時間(単位：s)
	float playingTime_s;

	/// @brief 再生速度
	float playSpeed;

	/// @brief 再生中か？
	bool isPlaying;
public:
	CP_Animation() : pSkeletalMesh(nullptr), pCurrentAnimation(nullptr), playingTime_s(0.0f),
		playSpeed(1.0f) , isPlaying(false) {}
	~CP_Animation() {}

	void Init() override;

	void LateUpdate() override;

	void ImGuiSetting() override;

	/// @brief アニメーションを再生する
	/// @param _animName 再生したいアニメーションの名前
	void PlayAnimation(const std::string& _animName);

	/// @brief アニメーションを追加する
	/// @param _addAnim 追加するアニメーション
	void AddAnimations(AnimationData& _addAnim);

	/// @brief アニメーションを追加する
	/// @param _animName 追加するアニメーションの名前
	void AddAnimations(const std::string& _animName);

	/// @brief アニメーションを外す
	/// @param _animName 外したいアニメーションの名前
	void RemoveAnimations(const std::string& _animName);

	/// @brief スケルタルメッシュをセット
	/// @param _skeletalMesh 
	void SetSkeletalMesh(SkeletalMesh& _skeletalMesh);
private:

	/// @brief 再生時間を進める
	void ProgressPlayTime();

	/// @brief 再生できる状態か？
	/// @return 再生できるか
	bool IsCanPlay();

	/// @brief ボーンコンビネーション行列を更新
	void UpdateBoneCombMtx();

	/// @brief 子ノードのコンビネーション行列を更新する（再帰関数）
	/// @param _treeNode 更新するノード
	/// @param _parentMtx ワールド変換するための親までの行列
	void UpdateNodeHierarchy(TreeNode& _treeNode, const DirectX::SimpleMath::Matrix& _parentMtx);

	/// @brief ボーンのコンビネーション行列を更新
	void UpdateAnimationMtx();

	/// @brief シェーダーのバッファを更新する
	void UpdateBoneBuffer();

	/// @brief 所持しているアニメーションを探す
	/// @param _animName 
	/// @return 
	AnimationData* FindAnimaton(const std::string& _animName);
};

