#pragma once

namespace HashiTaku
{
	// ボーン
	class Bone;

	/// @brief モデルのツリーノード
	class TreeNode
	{
		/// @brief ノード名
		std::string nodeName;

		/// @brief 親ノード
		TreeNode* pParentNode;

		/// @brief 子ノード
		std::list<std::unique_ptr<TreeNode>>  pChildNodes;

		/// @brief 親ノードからのローカル座標系の変換行列
		DXSimp::Matrix transformMtx;

		/// @brief 対応したボーン
		Bone* pLinkBone;
	public:
		TreeNode() : nodeName(""), pParentNode(nullptr), pLinkBone(nullptr) {}
		~TreeNode() {}

		/// @brief 親ノードをセット
		/// @param _parentNode 親ノード
		void SetParent(TreeNode& _parentNode);

		/// @brief 子供ノードを追加
		/// @param _chiledNode 子ノードのユニークポインタ
		void AddChild(std::unique_ptr<TreeNode> _chiledNode);

		/// @brief 子供ノードを除外
		/// @param _chiledNode 子ノード
		void RemoveChiled(TreeNode& _chiledNode);

		/// @brief 対応したボーンがあるか確認
		/// @return ボーンがあるか？
		bool HasBone() const;

		/// @brief ボーンのIDから対応したツリーノードを子ノードから探す
		/// @param _boneIdx 探したいツリーノードのボーンID
		/// @return ツリーノード(見つからなければnullptr)
		const TreeNode* FindTreeNode(int _boneIdx) const;

		// ノード名をセット
		void SetNodeName(const std::string& _nodeName);

		// ローカル座標系への変換行列セット
		void SetTransformMtx(const DXSimp::Matrix& _transformMtx);

		// 対応したボーンをセット
		void SetBone(Bone& _bone);

		/// @brief 子ノードを取得
		/// @param _arrayIdx 配列インデックス
		/// @return 子ノード
		const TreeNode* GetChild(u_int _arrayIdx) const;

		// 子ノードの数取得
		u_int GetChildNum() const;

		// 名前を取得する
		const std::string& GetName() const;

		/// @brief ボーンIDを取得
		/// @return ボーンID
		u_int GetBoneIdx() const;

		// ローカル座標までの移動行列を取得
		const DXSimp::Matrix& GetTransformMtx() const;
	};
}