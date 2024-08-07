#pragma once
#include "Component.h"

#include "AnimationController.h"

// モデル
class SkeletalMesh;
class Bone;
class TreeNode;

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

	/// @brief アニメーションコントローラー
	AnimationController* pAnimController;

	/// @brief 現在再生しているアニメーションの割合（0.0f～1.0f）
	float playingRatio;

	/// @brief 再生速度
	float playSpeed;

	/// @brief 再生中か？
	bool isPlaying;
public:
	CP_Animation();
	~CP_Animation() {}

	void Init() override;

	void LateUpdate() override;

	void ImGuiSetting() override;

	/// @brief アニメーションを追加する
	/// @param _animName 追加するアニメーションの名前
	void AddAnimations(const std::string& _animName);

	/// @brief アニメーションを外す
	/// @param _animName 外したいアニメーションの名前
	void RemoveAnimations(const std::string& _animName);

	/// @brief スケルタルメッシュをセット
	/// @param _skeletalMesh 
	void SetSkeletalMesh(SkeletalMesh& _skeletalMesh);

	// アニメーションコントローラーをセット
	void SetAnimationController(AnimationController& _controller);
private:
	
	/// @brief 再生時間を進める
	void ProgressPlayTime();

	/// @brief 再生できる状態か？
	/// @return 再生できるか
	bool IsCanPlay();

	/// @brief ループ再生できるか？
	/// @return 再生できるフラグ
	bool IsCanLoop();

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

	// 再生中のアニメーションのノードを取得
	const AnimationNode_Base* GetCurrentNode();
};

