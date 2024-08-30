#pragma once

class AnimationNode_Base;

/// @brief アニメーション遷移の条件等を設定する矢印
class AnimTransitionArrow : public HashiTaku::IImGuiUser
{
	/// @brief 遷移する条件
	std::function<bool()> condition;

	/// @brief	遷移元アニメノード
	AnimationNode_Base* pFromNode;

	/// @brief	遷移先アニメノード
	AnimationNode_Base* pToNode;

	/// @brief 遷移先のアニメーションの指定割合
	float transTargetRatio;
public:
	//AnimTransitionArrow();

	/// @brief コンストラクタ
	/// @param _pFromNode 遷移前
	/// @param _pToNode 遷移先
	/// @param _transTargetRatio 遷移先アニメーション割合(0.0～1.0)
	/// @param _condition 遷移条件
	AnimTransitionArrow(AnimationNode_Base& _pFromNode, AnimationNode_Base& _pToNode, float _transTargetRatio = 0.0f, std::function<bool()> _condition = nullptr);

	~AnimTransitionArrow() {}

	/// @brief 遷移条件を達成しているか確認
	/// @return 達成しているか？
	bool CheckTransition();

	/// @brief 条件をセット
	/// @param _condition bool型戻り値の条件
	void SetCondition(std::function<bool()> _condition);

	// 遷移先のノードを取得する
	AnimationNode_Base& GetToNode();

	float GetTargetRatio() const;

private:
	void ImGuiSetting() override;
};

