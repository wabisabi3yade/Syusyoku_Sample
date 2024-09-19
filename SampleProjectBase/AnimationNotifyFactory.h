#pragma once
#include "AnimNotifyRespawner.h"

class AnimationNotifyFactory : public Singleton_Base<AnimationNotifyFactory>
{
	friend class Singleton_Base<AnimationNotifyFactory>;

	/// @brief 全てのアニメーション通知クラスリスト
	std::unordered_map<std::string, std::unique_ptr<AnimNotifyRespawner_Base>> animNotifyList;

#ifdef EDIT
	// 選択中の名前
	std::string selectName;	
#endif

public:

	/// @brief 通知クラスを生成し、返す
	/// @param _notifyName 通知イベントクラス名
	/// @return 生成したインスタンス
	std::unique_ptr<AnimationNotify_Base> Create(const std::string& _notifyName);

	/// @brief コンボボックスで作成する
	/// @brief Factoryで
	/// @return 作成するボタンをおされたら返す
	bool ImGuiCombo(std::unique_ptr<AnimationNotify_Base>& _pCreateNotify);
private:
	AnimationNotifyFactory();
	~AnimationNotifyFactory() {}

	void Init();
	template<HashiTaku::AnimNotifyConcept T> void ResisterNotify();

	std::vector<const std::string*> GetNotifyNames() const;
};

template<HashiTaku::AnimNotifyConcept T>
inline void AnimationNotifyFactory::ResisterNotify()
{
	std::string notifyName = TYPENAME_ROUGH(T);
	std::unique_ptr<AnimNotifyRespawner_Base> pRespawner = std::make_unique<AnimNotifyRespawner<T>>();

	animNotifyList[notifyName] = std::move(pRespawner);
}
