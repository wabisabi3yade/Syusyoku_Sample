#pragma once
#include "AnimNodePlayer_Base.h"
#include "BlendAnimationNode.h"

class AnimationParameters;

/// @brief ブレンドノードを再生
class AnimBlendNodePlayer : public AnimNodePlayer_Base
{
	/// @brief 再生時に必要になる軸ごとのパラメータ
	struct AxisPlayParameter
	{
		/// @brief ブレンド値の目標数値ポインタ
		const float* pTargetBlendValue{ nullptr };

		/// @brief 1フレーム前のブレンド値の目標値
		float lastTargetBlendValue{ 0.0f };

		/// @brief 現在のブレンド値
		float curBlendValue{ 0.0f };

		/// @brief 変更した瞬間のブレンド値
		float changeBlendValue { 0.0f };

		/// @brief 現在のブレンド移動時間
		float curBlendMoveTime{ 0.0f };
	};

	/// @brief ブレンドで対応している軸数
	static constexpr int MAX_AXIS_CNT = 2;

	std::vector<AxisPlayParameter> axisPlayParameters;

	/// @brief アニメーションパラメータリスト
	const AnimationParameters* pAnimParameters;

	/// @brief 再生中のブレンドノード
	const BlendAnimationNode* pBlendNode;
public:
	AnimBlendNodePlayer(const AnimationParameters& _animParameters, const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform);
	~AnimBlendNodePlayer() {}

private:
	/// @brief 初期化処理
	void Init();

	/// @brief ブレンドアニメーションの更新処理
	void Update() override;

	/// @brief 更新できるか確認
	/// @return 更新できるか？
	bool IsCanUpdate();

	/// @brief ブレンド値を移動する
	void MoveCurrentBlend();

	/// @brief アニメーション更新処理
	void AnimationUpdate();

	/// @brief 1つのみのアニメーションブレンド処理
	/// @param _blendingData ブレンドデータ
	void SingleAnimationUpdate(const std::vector<BlendAnimationNode::BlendingData>& _blendingData);

	/// @brief 1つのみのアニメーションブレンド処理
	/// @param _blendingData ブレンドデータ
	void TwoAnimationUpdate(const std::vector<BlendAnimationNode::BlendingData>& _blendingData);
	
	/// @brief 1つのみのアニメーションブレンド処理
	/// @param _blendingData ブレンドデータ
	void SquareAnimationUpdate(const std::vector<BlendAnimationNode::BlendingData>& _blendingData);

	/// @brief 現在の軸数を取得
	/// @return 軸数
	u_int GetBlendAxisCnt() const;

	/// @brief ルートモーションを取得する（内部で必要な計算を行う）
	/// @param 現在の再生割合
	/// @return 現在の割合のルートモーション座標
	DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio) override;

	/// @brief ルートモーションを取得する（内部で必要な計算を行う）
	/// @param 現在の再生割合
	/// @return 現在の割合のルートモーション回転量
	DirectX::SimpleMath::Quaternion GetRootMotionRot(float _ratio) override;
};

