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
		std::vector<int> baseAnimBoneIds;

		/// @brief ブレンド側のアニメーションを反映するボーンのId
		std::vector<int> blendAnimBoneIds;

		/// @brief ベースとなるアニメーション(ルートボーンはこっち)
		AnimationData* pBaseAnimation;

		/// @brief ブレンドするアニメーション
		AnimationData* pBlendAnimation;

		/// @brief ボーンリスト
		const BoneList* pBoneList;

		/// @brief ブレンド側のアニメーションをどこから再生するか
		u_int beginBlendPlayFrame;

		/// @brief ブレンド開始ボーンからのブレンドするボーンの深さ(2なら開始ボーンから深さ2までのボーンを0.5ずつブレンドする)
		u_int blendDepth;

		/// @brief どこからブレンドアニメーションを再生するか指定するボーンID
		int beginBlendBoneId;

		/// @brief メッシュのローカル空間内で回転量をブレンド
		bool isMeshSpaceRotationBlend;
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
		DXSimp::Vector3 GetRootMotionPos(float _ratio) const;

		/// @brief ルートモーション秒速を取得
		/// @return ルートモーション秒速
		const DXSimp::Vector3& GetRootMotionPerSpeed() const;

		/// @brief メッシュ空間内でブレンドを行うか取得
		/// @return メッシュ空間内でブレンドを行うか
		bool GetMotionSpaceRotationBlend() const;

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief 機能できているか確認する
		/// @return 機能できているか？
		bool CanWarking() const;

		/// @brief メッシュ空間でブレンドをしたトランスフォーム取得(計算コスト高)
		/// @param outTransforms 格納するボーントランスフォームリスト
		/// @param _currentNode 現在のノード
		/// @param _parentBaseRotation ベース側の親ボーンまでのメッシュ空間回転量
		/// @param _parentBlendRotation ブレンド側の親ボーンまでのメッシュ空間回転量
		/// @param _parentCalculateRotation 計算されている現在の親ボーンまでの
		/// @param _baseRatio ベースのアニメーション側の割合
		/// @param _blendRatio ブレンド側のアニメーション割合
		/// @param _blendDepth ブレンド開始からの深さ
		void GetMeshSpaceBlendTransform(std::vector<BoneTransform>& _outTransforms, 
			const TreeNode* _currentNode, 
			const DXSimp::Quaternion& _parentBaseRotation,
			const DXSimp::Quaternion& _parentBlendRotation,
			const DXSimp::Quaternion& _parentCalculateRotation,
			float _baseRatio, 
			float _blendRatio,
			u_int _blendDepth) const;

		/// @brief ボーンのローカル空間でブレンドをしたトランスフォーム取得(計算コスト低)
		/// @param outTransforms 格納するボーントランスフォームリスト
		/// @param_baseRatio ベースのアニメーション側の割合
		/// @param _blendRatio ブレンド側のアニメーション割合
		void GetLocalSpaceBlendTransform(std::vector<BoneTransform>& _outTransforms, float _baseRatio, float _blendRatio) const;


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