#pragma once
#include "AnimNodePlayer_Base.h"

namespace HashiTaku
{
	/// @brief レイヤーノードの再生クラス
	class AnimLayerdNodePlayer : public AnimNodePlayer_Base
	{
		/// @brief ブレンド側の現在の再生割合
		float curBlendPlayRatio;

	public:
		AnimLayerdNodePlayer(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform);
		~AnimLayerdNodePlayer() {}

		/// @brief 再生割合を進める
		void ProgressPlayRatio(float _controllerSpeed) override;

		/// @brief アニメーションの更新処理
		/// @param _outTransforms アニメーションの姿勢 
		void Update(std::vector<BoneTransform>& _outTransforms) override;

		/// @brief ルートモーションの座標移動速度を計算する
		void CalcRootMotionPosSpeed() override;

		/// @brief ルートモーションの移動量取得
		/// @param _ratio 割合
		/// @param _isWorldScaling ワールドスケーリング適用するか？ 
		/// @return ルートモーション移動量
		DXSimp::Vector3 GetRootMotionPos(float _ratio, bool _isWorldScaling = true) const override;

	private:
		void ImGuiDebug() override;
	};
}