#pragma once
#include "AnimationNotify_Base.h"

namespace HashiTaku
{
	/// @brief インスタンスを生成する基底クラス
	class AnimNotifyRespawner_Base
	{
	public:
		AnimNotifyRespawner_Base() {}
		virtual ~AnimNotifyRespawner_Base() {}

		virtual std::unique_ptr<AnimationNotify_Base> Create() = 0;
	};

	/// @brief インスタンスを生成するクラス
	/// @tparam T 生成したいクラス
	template <AnimNotifyConcept T>
	class AnimNotifyRespawner : public AnimNotifyRespawner_Base
	{
	public:
		AnimNotifyRespawner() {}
		~AnimNotifyRespawner() {}

		/// @brief 指定したクラスのインスタンスを生成する
		/// @returnインスタンスのユニークポインタ
		std::unique_ptr<AnimationNotify_Base> Create() override;
	};

	template<AnimNotifyConcept T>
	inline std::unique_ptr<AnimationNotify_Base> AnimNotifyRespawner<T>::Create()
	{
		return std::make_unique<T>();
	}
}