#pragma once

// 親子関係をあらわすクラス
#include "TreeNode.h"

/// @brief アニメーション時の頂点への影響度
struct Weight
{
	/// @brief ボーン名(デバッグ用)
	std::string boneName{ "" };

	/// @brief 頂点への影響度(0～1)
	float weight{ 0.0f };

	/// @brief 頂点インデックス
	int	vertexIndex{ 0 };
};

/// @brief モデルの1ボーンだけのクラス
class Bone
{
	/// @brief ウェイト値
	std::vector<Weight> weights;

	/// @brief ボーン名(デバッグ用)
	std::string boneName;

	/// @brief コンビネーション行列
	DirectX::SimpleMath::Matrix combinationMatrix;

	/// @brief アニメーション行列
	DirectX::SimpleMath::Matrix animationMatrix;

	/// @brief ボーンオフセット行列
	DirectX::SimpleMath::Matrix offsetMatrix;
	
	// ボーンの親子関係ノード
	std::unique_ptr<TreeNode> pTreeNode;

	/// @brief ボーンインデックス
	u_int boneIdx;

public:
	Bone() : boneName(""), boneIdx(0) {}
	~Bone() {}

	void CreateCombMtx(const DirectX::SimpleMath::Matrix& _parentMtx);

	/// @brief ウェイトを配列に追加する
	/// @param _weight ウェイト
	void AddWeight(const Weight& _weight);

	// 名前セット
	void SetBoneName(const std::string& _boneName);

	// 行列セット
	void SetAnimationMtx(const DirectX::SimpleMath::Matrix& _animationMatrix);
	void SetOffeetMtx(const DirectX::SimpleMath::Matrix& _offsetMatrix);

	// インデックスセット
	void SetIndex(u_int _idx);

	// 名前取得
	std::string GetBoneName() const;

	// 行列を取得
	DirectX::SimpleMath::Matrix& GetCombMtx();
	DirectX::SimpleMath::Matrix& GetAnimMtx();
	DirectX::SimpleMath::Matrix& GetOffsetMtx();

	const TreeNode* GetTreeNode();

	// インデックスを取得
	u_int GetIndex();
};
