#pragma once

/// @brief モデルの1ボーンだけのクラス
class Bone
{
	/// @brief ボーン名(デバッグ用)
	std::string boneName;

	/// @brief コンビネーション行列
	DirectX::SimpleMath::Matrix combinationMatrix;

	/// @brief アニメーション行列
	DirectX::SimpleMath::Matrix animationMatrix;

	/// @brief ボーンオフセット行列
	DirectX::SimpleMath::Matrix offsetMatrix;

	/// @brief ボーンインデックス
	u_int boneIdx;

public:
	Bone() : boneName(""), boneIdx(0) {}
	~Bone() {}

	void CreateCombMtx(const DirectX::SimpleMath::Matrix& _parentMtx);

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
	const DirectX::SimpleMath::Matrix& GetCombMtx() const;
	const DirectX::SimpleMath::Matrix& GetAnimMtx() const;
	const DirectX::SimpleMath::Matrix& GetOffsetMtx() const;

	// インデックスを取得
	u_int GetIndex() const;
};
