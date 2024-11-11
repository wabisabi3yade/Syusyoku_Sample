#pragma once

// ボーンのトランスフォーム
struct BoneTransform
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 scale{ 1.0f, 1.0f, 1.0f };
	DirectX::SimpleMath::Quaternion rotation;
};

/// @brief モデルの1ボーンだけのクラス
class Bone
{
	/// @brief ボーン名(デバッグ用)
	std::string boneName;

	/// @brief コンビネーション行列
	DirectX::SimpleMath::Matrix combinationMatrix;

	/// @brief ボーンオフセット行列
	DirectX::SimpleMath::Matrix offsetMatrix;

	/// @brief ボーンの姿勢行列
	DirectX::SimpleMath::Matrix globalMatrix;

	/// @brief アニメーションのトランスフォーム
	BoneTransform animationTransform;

	/// @brief ボーンインデックス
	u_int boneIdx;

public:
	Bone() : boneName(""), boneIdx(0) {}
	Bone(const Bone& _other);
	~Bone() {}
	Bone& operator=(const Bone& _other);

	/// @brief コンビネーション行列を作成
	/// @param _parentMtx 親ノードまでのローカルトランスフォーム
	void CreateCombMtx(const DirectX::SimpleMath::Matrix& _parentMtx);

	/// @brief ローカル空間でのボーン座標
	/// @param _parentMtx 親ノードまでのローカルトランスフォーム
	/// @param _offsetMtx オフセット行列
	void CreateGlobalMtx(const DirectX::SimpleMath::Matrix& _parentMtx, const DirectX::SimpleMath::Matrix& _offsetMtx);

	// 名前セット
	void SetBoneName(const std::string& _boneName);

	// 行列セット
	void SetOffeetMtx(const DirectX::SimpleMath::Matrix& _offsetMatrix);

	/// @brief ボーンのアニメーショントランスフォームの参照を取得
	/// @return アニメーショントランスフォームの参照
	BoneTransform& GetRefelenceAnimTransform();

	// インデックスセット
	void SetIndex(u_int _idx);

	// 名前取得
	std::string GetBoneName() const;

	// 行列を取得
	const DirectX::SimpleMath::Matrix& GetCombMtx() const;
	DirectX::SimpleMath::Matrix GetAnimMtx() const;
	const DirectX::SimpleMath::Matrix& GetOffsetMtx() const;
	const DirectX::SimpleMath::Matrix& GetGlobalMtx() const;

	// トランスフォームを取得
	BoneTransform GetAnimationTransform() const;

	// インデックスを取得
	u_int GetIndex() const;

private:
	void Copy(const Bone& _other);
};
