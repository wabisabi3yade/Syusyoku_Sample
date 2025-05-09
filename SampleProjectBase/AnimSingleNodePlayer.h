#pragma once
#include "AnimNodePlayer_Base.h"

namespace HashiTaku
{
	class SingleAnimationNode;

	/// @brief 単一アニメーションのノード再生する
	class AnimSingleNodePlayer : public AnimNodePlayer_Base
	{
		/// @brief 再生のキー数
		u_int playingFrame;

	public:
		/// @brief コンストラクタ
		/// @param _singleNode 単一のアニメーションノード
		/// @param _boneList ボーンリスト
		/// @param _transform トランスフォーム
		AnimSingleNodePlayer(const AnimationNode_Base& _singleNode, BoneList& _boneList, Transform& _transform);
		~AnimSingleNodePlayer() {}

		/// @brief 再生キー数をセット
		/// @param _playKey 再生キー数
		void SetPlayingKey(u_int _playKey);

		/// @brief 再生キー数を取得する
		/// @return 再生キー
		u_int GetPlayKey() const;

		/// @brief 全体のキー数
		/// @return キー数
		u_int GetAllFrame() const;
	private:
		void Update(std::vector<BoneTransform>& _outTransforms) override;

		/// @brief ルートモーションの座標移動速度を計算する
		void CalcRootMotionPosSpeed() override;

		/// @brief ルートモーションを取得する（内部で必要な計算を行う）
		/// @param 現在の再生割合
		/// @param _isWorldScaling ワールド軸に対応するスケーリングするか？
		/// @return 現在の割合のルートモーション座標
		DXSimp::Vector3 GetRootMotionPos(float _ratio, bool _isWorldScaling = true) const override;

	protected:
		/// @brief モデルの大きさが原寸の大きさからどれだけの倍率なのか計算する
		/// @return 原寸からのモデルスケール倍率
		float GetModelScale() const;

		void ImGuiDebug() override;

	private:

	};
}