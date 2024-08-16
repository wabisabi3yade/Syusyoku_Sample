#pragma once
#include "Component.h"
#include "IAnimationObserver.h"

// モデル
class SkeletalMesh;
class Bone;
class TreeNode;

class AnimationController;

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
public:
	CP_Animation();
	~CP_Animation() {}

	void Init() override;

	void LateUpdate() override;

	void ImGuiSetting() override;

	/// @brief スケルタルメッシュをセット
	/// @param _skeletalMesh 
	void SetSkeletalMesh(SkeletalMesh& _skeletalMesh);

	// アニメーションコントローラーをセット
	void SetAnimationController(AnimationController& _controller);

	// スケルタルメッシュを取得
	SkeletalMesh& GetSkeletalMesh();
private:
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
};

