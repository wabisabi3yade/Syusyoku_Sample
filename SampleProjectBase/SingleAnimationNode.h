#pragma once
#include "AnimationNode_Base.h"

namespace HashiTaku
{
	/// @brief アニメーション情報がひとつだけのアニメーションノード
	class SingleAnimationNode : public AnimationNode_Base
	{
		/// @brief アニメーションデータ
		AnimationData* pAnimationData;
	public:
		SingleAnimationNode(const std::string& _nodeName) : AnimationNode_Base(_nodeName, NodeType::Single), pAnimationData(nullptr) {}

		~SingleAnimationNode() {}

		/// @brief アニメーション
		/// @param _animData アニメーションデータ
		void SetAnimationData(const std::string& _animName);

		// アニメーションデータを取得する
		const AnimationData& GetAnimationData() const;

		/// @brief アニメーションのトランスフォーム取得
		/// @param _outTransform 格納するボーントランスフォーム
		/// @param _boneId ボーンのID
		/// @param _requestRatio 取得する指定のアニメーション割合
		void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const;

		/// @brief 割合からキー数を求める
		/// @param _ratio 割合
		/// @return キー数
		u_int GetRatioToKey(float _ratio) const;

		/// @brief 全体キー数を求める
		/// @return 全体のキー数
		u_int GetAllKeyFrame() const override;

		/// @brief ルートモーションの移動速度を取得する
		/// @return ルートモーションの移動速度
		const DirectX::SimpleMath::Vector3& GetRootMotionPosSpeed() const;

		json Save() override;
		void Load(const json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}