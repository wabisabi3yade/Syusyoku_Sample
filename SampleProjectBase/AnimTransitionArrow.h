#pragma once

#include "InterpolateKind.h"

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

	/// @brief 遷移時間
	float transitionTime;

	/// @brief 遷移をすすめるときに利用するイージング
	HashiTaku::EaseKind easeKind;

	/// @brief アニメーション間で使用する補間方法
	HashiTaku::AnimInterpolateKind interpolateKind;
public:
	/// @brief コンストラクタ
	/// @param _pFromNode 遷移前
	/// @param _pToNode 遷移先
	/// @param _transTargetRatio 遷移先アニメーション割合(0.0～1.0)
	/// @param _transitionTime 遷移時間
	/// @param _condition 遷移条件
	/// @param _easeKind イージングの種類
	AnimTransitionArrow(AnimationNode_Base& _pFromNode, AnimationNode_Base& _pToNode, float _transTargetRatio = 0.0f, float _transitionTime = 0.2f, std::function<bool()> _condition = nullptr, HashiTaku::EaseKind _easeKind = HashiTaku::EaseKind::Linear);

	~AnimTransitionArrow() {}

	/// @brief 遷移条件を達成しているか確認
	/// @return 達成しているか？
	bool CheckTransition();

	/// @brief 条件をセット
	/// @param _condition bool型戻り値の条件
	void SetCondition(std::function<bool()> _condition);

	/// イージングの種類をセット
	void SetEaseKind(HashiTaku::EaseKind _easeKind);

	// 遷移の補間種類をセット
	void SeInterpolateKind(HashiTaku::AnimInterpolateKind _interpolateKind);

	// 遷移先のノードを取得する
	AnimationNode_Base& GetToNode();

	// 遷移先のアニメーション割合を取得
	float GetTargetRatio() const;

	// 遷移時間を取得
	float GetTransitionTime() const;

	// イージングの種類を取得
	HashiTaku::EaseKind GetEaseKind() const;

	// 補間の種類を取得
	HashiTaku::AnimInterpolateKind GetInterpolateKind() const;
private:
	void ImGuiSetting() override;
};

