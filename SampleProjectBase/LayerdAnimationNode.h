#pragma once
#include "AnimationNode_Base.h"

namespace HashiTaku
{
	class TreeNode;

	// 今回上半身、下半身の再生だけをするので2個だけ対応します、必要になったら拡張

	/// @brief 部位ごとに分けてアニメーションするノード
	class LayerdAnimationNode : public AnimationNode_Base
	{
		/// @brief ブレンド側のスピードカーブで
		AnimationCurve blendSpeedCurve;

		/// @brief ベース側のアニメーションを反映するボーンのId
		std::vector<int> baseBoneIds;

		/// @brief ブレンド側のアニメーションを反映するボーンのId
		std::vector<int> blendBoneIds;

		/// @brief ベースとなるアニメーション(ルートボーンはこっち)
		AnimationData* pBaseAnimation;

		/// @brief ブレンドするアニメーション
		AnimationData* pBlendAnimation;

		/// @brief ボーンリスト
		const BoneList* pBoneList;

		/// @brief ブレンド側のアニメーションをどこから再生するか
		u_int beginBlendPlayFrame;

		/// @brief どこからブレンドアニメーションを再生するか指定するボーンID
		int beginBlendBoneId;
	public:
		/// @brief コンストラクタ
		/// @param _boneList ボーンリスト
		/// @param _nodeName ノード名
		LayerdAnimationNode(const BoneList& _boneList, const std::string& _nodeName);
		~LayerdAnimationNode() {}

		/// @brief アニメーションのトランスフォーム取得
		/// @param outTransforms 格納するボーントランスフォームリスト
		/// @param_baseRatio ベースのアニメーション側の割合
		/// @param _blendRatio ブレンド側のアニメーション割合
		void GetAnimTransform(std::vector<BoneTransform>& _outTransforms, float _baseRatio, float _blendRatio) const;

		/// @brief ベース側のアニメーション全体キー数を求める
		/// @return 全体のキー数
		u_int GetAllKeyFrame() const override;

		/// @brief ブレンド側のアニメーション全体キー数を求める
		/// @return 全体のキー数
		u_int GetBlendAllKeyFrame() const;

		/// @brief ブレンド側のアニメーションカーブ速度を求める
		/// @param _playRatio 再生割合
		/// @return 再生割合
		float GetBlendCurveSpeed(float _playRatio) const;

		/// @brief ブレンドアニメーションの時間を取得する
		/// @return ブレンドアニメーションの時間
		float GetBlendAnimationTime() const;

		/// @brief ブレンドアニメーションの開始時再生割合を取得する
		/// @return 開始時再生割合
		float GetBeginBlendPlayRatio() const;

		/// @brief ブレンド開始するボーンIDを取得
		/// @return ブレンド開始するボーンID
		int GetBeginBlendBoneId() const;

		/// @brief 指定した割合のルートモーションの移動座標を取得
		/// @return ルートモーションの移動座標
		DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio) const;

		/// @brief ルートモーション秒速を取得
		/// @return ルートモーション秒速
		const DirectX::SimpleMath::Vector3& GetRootMotionPerSpeed() const;

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief 機能できているか確認する
		/// @return 機能できているか？
		bool CanWarking() const;

		/// @brief ブレンド開始ボーンをセットする
		/// @param _beginBoneId ブレンド開始ボーンのId
		void SetBlendBeginBone(int _beginBoneId);

		/// @brief ツリーノードを辿って行ってボーンのグループを分けていく
		/// @param _treeNode ツリーノード（最初はツリーノード）
		void SeparateBoneGroup(const TreeNode& _treeNode);

		/// @brief ブレンドグループを作成する
		/// @param _blendBoneNode ブレンド開始ノード
		void CreateBlendBoneGroup(const TreeNode& _blendBoneNode);

		void ImGuiDebug() override;
	};
}