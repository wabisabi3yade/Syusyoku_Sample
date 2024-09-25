#pragma once
#include "Component.h"
#include "AnimationNotify_Base.h"

#ifdef EDIT
#include "AnimationNotifyFactory.h"
#endif // EDIT

class AnimationNode_Base;
class CP_Animation;
class AnimationController;

/// @brief アニメーションの通知イベントコンポーネント
class CP_AnimationNotify : public Component
{
	/// @brief 通知イベントと対象となるアニメーションノード
	struct NotifyGroup
	{
		const AnimationNode_Base* pAnimNode;
		std::list<std::unique_ptr<AnimationNotify_Base>> pNotifys;
	};

	/// @brief 通知イベントグループリスト
	std::list<std::unique_ptr<NotifyGroup>> notifyGroups;

	/// @brief 現在の通知グループ
	NotifyGroup* pCurrentGroup;

	/// @brief アニメーションコンポーネント
	AnimationController* pAnimController;	

	/// @brief 現在のアニメーション割合
	float curRatio;

	/// @brief 最後に呼び出されたときの割合
	float lastRatio;

	/// @brief ループしたか？
	bool isLoop;

#ifdef EDIT
	std::unique_ptr<AnimationNotifyFactory> pFactory;
#endif // EDIT

public:
	CP_AnimationNotify();
	~CP_AnimationNotify() {}

	void ImGuiSetting() override;

private:
	void Init() override;
	void Start() override;
	void LateUpdate() override;

	/// @brief 更新できるか確認
	/// @return 更新できるか？
	bool IsCanUpdate();

	/// @brief 割合を更新する
	void UpdateRatio();

	/// @brief イベント通知を更新する
	void NotifyUpdate();
	
	/// @brief 現在のアニメーションとイベント通知を同期させる
	void LinkCurGroup();
	
	/// @brief コントローラーとノードを更新する
	void UpdateCurGroup();

	/// @brief 次回の為の前回割合を更新する
	void UpdateLastRatio();

	/// @brief Animtionに新規ノードが追加されたときにこっちにも対応する
	void UpdateNotifyGroups();

	/// @brief イベント通知を作成する
	/// @param _addGroup 追加するグループ
	/// @param _typeName 作成するイベント通知の型名
	void CreateNotify(NotifyGroup& _addGroup, const std::string& _typeName);

	/// @brief アニメーションコントローラー取得
	AnimationController* GetAnimController();

	// グループごとの編集
	void ImGuiGroupSetting();
};