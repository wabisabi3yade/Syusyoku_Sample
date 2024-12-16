#pragma once
#include "IObserever.h"

namespace HashiTaku
{
	class AnimTransitionArrow;
	class BlendAnimationNode;

	namespace AnimParam
	{
		// 変更イベントの種類
		enum class AnimParamObseveEvent
		{
			Remove,	// 削除
			Rename	// 名前変更
		};

		/// @brief パラメータの変更内容オブザーバー渡す構造体
		struct NotificationData
		{
			/// @brief イベントの種類
			AnimParamObseveEvent eventType = AnimParamObseveEvent::Remove;

			/// @brief 削除する名前
			const std::string* pDeleteName{ nullptr };

			/// @brief 変更語名前
			const std::string* pAfterName{ nullptr };
		};

		/// @brief インターフェース
		class IAnimParamRelater
		{
		public:
			IAnimParamRelater() {}
			virtual ~IAnimParamRelater() {}

			/// @brief 変更内容を受けとり、処理を行う
			/// @param _data 通知データ
			virtual void AcceptAnimParamData(const NotificationData& _notifyData) = 0;
		};

		/// @brief アニメーションパラメータに変更があった場合の矢印のオブザーバーパターン
		class AnimParamObserver : public IObserver<NotificationData>
		{
			/// @brief オブザーバーを所持する矢印
			IAnimParamRelater* pInterface;
		public:
			AnimParamObserver(IAnimParamRelater& _relater, const std::string& _observerName);
			~AnimParamObserver() {}

			/// @brief 通知が来たときの処理
			/// @param _notifyData 通知データ
			void ObserverUpdate(const NotificationData& _notifyData) override;
		};
	}
}



