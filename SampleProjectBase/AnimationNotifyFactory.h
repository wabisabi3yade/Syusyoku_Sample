#pragma once
#include "AnimNotifyRespawner.h"
#include "AnimNotifyConcept.h"

class AnimationParameters;

class AnimationNotifyFactory
{
	/// @brief 全てのアニメーション通知クラスリスト
	std::unordered_map<AnimationNotify_Base::NotifyType, std::unique_ptr<AnimNotifyRespawner_Base>> animNotifyList;

	/// @brief 生成時に必要になるアニメーションパラメータアドレス
	AnimationParameters* pAnimationParameters;
public:
	AnimationNotifyFactory(AnimationParameters& _animationParameters);
	~AnimationNotifyFactory() {}

	/// @brief 通知クラスを生成し、返す
	/// @param _notifyName 通知イベントクラス
	/// @return 生成したインスタンス
	std::unique_ptr<AnimationNotify_Base> Create(AnimationNotify_Base::NotifyType _notifyType);

	/// @brief コンボボックスで作成する
	/// @brief Factoryで作成した通知
	/// @return 作成するボタンをおされたら返す
	bool ImGuiCombo(std::unique_ptr<AnimationNotify_Base>& _pCreateNotify);
private:
	void Init();

	/// @brief イベント通知を追加して、リスポーナーを作成する
	/// @tparam T 作成する通知イベント
	/// @param _createNotifyType 種類
	template<HashiTaku::AnimNotifyConcept T> void ResisterNotify(AnimationNotify_Base::NotifyType _createNotifyType);
};

template<HashiTaku::AnimNotifyConcept T>
inline void AnimationNotifyFactory::ResisterNotify(AnimationNotify_Base::NotifyType _createNotifyType)
{
	std::unique_ptr<AnimNotifyRespawner_Base> pRespawner = std::make_unique<AnimNotifyRespawner<T>>();

	animNotifyList[_createNotifyType] = std::move(pRespawner);
}
