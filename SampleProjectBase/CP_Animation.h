#pragma once
#include "Component.h"

// モデル
class SkeletalMesh;
class Bone;

// アニメーションデータ
class AnimationData;

// assimp
struct aiNode;

/// @brief アニメーションコンポーネント
class CP_Animation : public Component
{
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

	/// @brief ボーンコンビネーション行列を更新
	void UpdateBoneCombMtx();

	/// @brief 子ノードのコンビネーション行列を更新する（再帰関数）
	/// @param _aiNode 更新するノード
	/// @param _parentMtx ワールド変換するための親までの行列
	void UpdateNodeHierarchy(const aiNode& _aiNode, const DirectX::SimpleMath::Matrix& _parentMtx);

	/// @brief ボーンのコンビネーション行列を更新
	void UpdateAnimationMtx();

	// 消す
	Bone* GetBoneByName(const std::string& _boneName);

	/// @brief 所持しているアニメーションを探す
	/// @param _animName 
	/// @return 
	AnimationData* FindAnimaton(const std::string& _animName);

	/// @brief アニメーションの情報にボーンのIDをリンクさせる
	/// @param _connectAnim リンクするアニメーション
	void ConnectBoneId(AnimationData& _connectAnim);
};

