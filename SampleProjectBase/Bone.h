#pragma once

namespace HashiTaku
{
	/// @brief  ボーンのトランスフォーム
	struct BoneTransform
	{
		DXSimp::Vector3 position;
		DXSimp::Vector3 scale{ 1.0f, 1.0f, 1.0f };
		DXSimp::Quaternion rotation;
	};

	/// @brief モデルの1ボーンだけのクラス
	class Bone
	{
		/// @brief ボーン名(デバッグ用)
		std::string boneName;

		/// @brief コンビネーション行列
		DXSimp::Matrix combinationMatrix;

		/// @brief ボーンオフセット行列
		DXSimp::Matrix offsetMatrix;

		/// @brief ボーンの姿勢行列
		DXSimp::Matrix globalMatrix;

		/// @brief アニメーションのトランスフォーム
		BoneTransform animationTransform;

		/// @brief 親ボーンからの初期姿勢トランスフォーム
		BoneTransform localNodeTransform;

		/// @brief ボーンインデックス
		u_int boneIdx;
	public:
		Bone() : boneName(""), boneIdx(0) {}
		Bone(const Bone& _other);
		~Bone() {}
		Bone& operator=(const Bone& _other);

		/// @brief コンビネーション行列を作成
		/// @param _parentMtx 親ノードまでのローカルトランスフォーム
		void CreateCombMtx(const DXSimp::Matrix& _parentMtx);

		/// @brief ローカル空間でのボーン座標
		/// @param _parentMtx 親ノードまでのローカルトランスフォーム
		/// @param _offsetMtx オフセット行列
		void CreateGlobalMtx(const DXSimp::Matrix& _parentMtx, const DXSimp::Matrix& _offsetMtx);

		// 名前セット
		void SetBoneName(const std::string& _boneName);

		// 行列セット
		void SetOffeetMtx(const DXSimp::Matrix& _offsetMatrix);

		/// @brief ローカルノード行列をセット
		/// @param _nodeMatrix ノード行列
		void SetLocalNodeMtx(const DXSimp::Matrix& _nodeMatrix);

		/// @brief ボーンのアニメーショントランスフォームの参照を取得
		/// @return アニメーショントランスフォームの参照
		BoneTransform& GetRefelenceAnimTransform();

		// インデックスセット
		void SetIndex(u_int _idx);

		// 名前取得
		std::string GetBoneName() const;

		// 行列を取得
		const DXSimp::Matrix& GetCombMtx() const;
		DXSimp::Matrix GetAnimMtx() const;
		const DXSimp::Matrix& GetOffsetMtx() const;
		const DXSimp::Matrix& GetGlobalMtx() const;

		// トランスフォームを取得
		const BoneTransform& GetAnimationTransform() const;

		/// @brief 初期姿勢ローカルトランスフォームを取得
		/// @return 初期姿勢ローカルトランスフォーム
		const BoneTransform& GetLocalNodeTransform() const;

		// インデックスを取得
		u_int GetIndex() const;

	private:
		void Copy(const Bone& _other);
	};
}