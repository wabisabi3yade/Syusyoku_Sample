#pragma once
#include "TransCondition_Base.h"
#include "InterpolateKind.h"
#include "AnimationParameters.h"
#include "AnimParamRemoveObserver.h"

class AnimationNode_Base;
namespace HashiTaku
{
	namespace AnimParam
	{
		struct NotificationData;
	}
}

/// @brief アニメーション遷移の条件等を設定する矢印
class AnimTransitionArrow : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad, public HashiTaku::AnimParam::IAnimParamRelater
{
	/// @brief 遷移条件を格納するリスト
	std::list<std::unique_ptr<TransCondition_Base>> conditionList;

	/// @brief パラメータ削除時の通知受け取り
	std::unique_ptr<HashiTaku::AnimParam::AnimParamObserver> pRemoveParamObserver;

	/// @brief	遷移元アニメノード
	AnimationNode_Base* pFromNode;

	/// @brief	遷移先アニメノード
	AnimationNode_Base* pToNode;

	/// @brief コントローラー内変数
	AnimationParameters* pAnimParameters;

	/// @brief 遷移をすすめるときに利用するイージング
	HashiTaku::EaseKind transitiionEase;

	/// @brief アニメーション間で使用する補間方法
	HashiTaku::AnimInterpolateKind interpolateKind;

	/// @brief 遷移開始割合
	float exitRatio;

	/// @brief 遷移先のアニメーションの指定割合
	float transTargetRatio;

	/// @brief 遷移時間
	float transitionTime;

	/// @brief 遷移開始時間でしか開始しないようにする
	bool isHasExitRatio;
#ifdef EDIT
	// 選択中の名前
	std::string selectParamName;
#endif
public:
	/// @brief コンストラクタ
	/// @param _fromNode 遷移前
	/// @param _toNode 遷移先
	/// @param _animParameters コントローラー内変数
	AnimTransitionArrow(AnimationNode_Base& _fromNode, AnimationNode_Base& _toNode, AnimationParameters& _animParameters);
	virtual ~AnimTransitionArrow() {}

	/// @brief 矢印に遷移条件を作成
	/// @param _val 参照するパラメータ値
	/// @param _name パラメータ名
	void AddCondition(const HashiTaku::AnimParam::conditionValType& _val, const std::string& _name);

	/// @brief パラメータ名から遷移条件を削除する
	/// @param _condParamName 遷移条件が参照しているパラメータ名
	void RemoveCondition(const std::string& _condParamName);

	/// @brief 条件が参照している名前を変える
	/// @param _prevName 前の名前
	/// @param _afterName 変更後の名前
	void ReNameCondition(const std::string& _prevName, const std::string& _afterName);

	// 遷移終了時の遷移先のアニメーション割合をセット
	void SetTransTargetRatio(float _transTargetRatio);

	// 遷移にかかる時間をセット
	void SetTransitonTime(float _transTime);

	// イージングの種類をセット
	void SetEaseKind(HashiTaku::EaseKind _easeKind);

	// 遷移の補間種類をセット
	void SeInterpolateKind(HashiTaku::AnimInterpolateKind _interpolateKind);

	// 遷移先のノードを取得する
	AnimationNode_Base& GetToNode() const;

	/// @brief 遷移先の開始割合を取得
	/// @return 遷移先の開始割合
	float GetTargetRatio() const;

	// 遷移時間を取得
	float GetTransitionTime() const;

	/// @brief 遷移元の終了割合を取得
	/// @return 遷移元の終了割合
	float GetExitRatio() const;

	/// @brief 遷移終了時間を持っているか取得
	/// @return 遷移終了時間もっているか？
	bool GetIsHasExit() const;


	/// @brief 条件リストを取得
	/// @return 条件リスト
	const std::list<std::unique_ptr<TransCondition_Base>>& GetConditionList() const;

	// イージングの種類を取得
	HashiTaku::EaseKind GetEaseKind() const;

	// 補間の種類を取得
	HashiTaku::AnimInterpolateKind GetInterpolateKind() const;

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;

	void AcceptAnimParamData(const HashiTaku::AnimParam::NotificationData& _notifyData);
private:
	// ImGuiで補間の種類を編集
	void ImGuiSetInterpolate();

	// 遷移条件を編集
	void ImGuiSetCondistion();

protected:
	void ImGuiSetting() override;
};

