#pragma once

/// @brief アニメーション時の頂点への影響度
struct Weight
{
	/// @brief ボーン名(デバッグ用)
	std::string boneName{ "" };

	/// @brief メッシュ名(デバッグ用)
	std::string meshName{ "" };

	/// @brief 頂点への影響度(0～1)
	float weight{ 0.0f };

	/// @brief 頂点インデックス
	int	vertexIndex{ 0 };
};

/// @brief モデルの1ボーンだけのクラス
class Bone
{
	/// @brief ボーン名(デバッグ用)
	std::string boneName;

	/// @brief メッシュ名(デバッグ用)
	std::string meshName;

	/// @brief アーマチュア名(デバッグ用)
	std::string armatureName;

	/// @brief コンビネーション行列
	DirectX::SimpleMath::Matrix combinationMatrix;

	/// @brief アニメーション行列
	DirectX::SimpleMath::Matrix animationMatrix;

	/// @brief ボーンオフセット行列
	DirectX::SimpleMath::Matrix offsetMatrix;

	/// @brief ボーンインデックス
	u_int boneIdx;

	/// @brief ウェイト値
	std::vector<Weight> weights;

public:
	Bone() : boneName(""), meshName(""), armatureName(""), boneIdx(0) {}
	~Bone() {}

	/// @brief ウェイトを配列に追加する
	/// @param _weight ウェイト
	void AddWeight(const Weight& _weight);

	// 名前セット
	void SetBoneName(const std::string& _boneName);
	void SetMeshName(const std::string& _meshName);
	void SetArmatureName(const std::string& _armatureName);

	// 行列セット
	void SetAnimationMtx(const DirectX::SimpleMath::Matrix& _animationMatrix);
	void SetOffeetMtx(const DirectX::SimpleMath::Matrix& _offsetMatrix);

	// インデックスセット
	void SetIndex(u_int _idx);

	// 名前取得
	std::string GetBoneName() const;
	std::string GetMeshName() const;
	std::string GetArmatureName() const;

	// 行列を取得
	DirectX::SimpleMath::Matrix& GetCombMtx();
	DirectX::SimpleMath::Matrix& GetAnimMtx();
	DirectX::SimpleMath::Matrix& GetOffsetMtx();

	void CreateCombMtx(const DirectX::SimpleMath::Matrix& _parentMtx);

	// インデックスを取得
	u_int GetIndex();
};
