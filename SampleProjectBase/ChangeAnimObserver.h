#pragma once
#include "IObserever.h"

namespace HashiTaku
{
	/// @brief アニメーション変更に関する情報
	struct ChangeAnimationInfo
	{
		/// @brief 遷移元アニメーションノード名
		const std::string* pFromAnimNodeName;

		/// @brief 遷移先アニメーションノード名
		const std::string* pToAnimNodeName;
	};

	/// @brief アニメーション変更を通知されるオブザーバー
	class ChangeAnimObserver : public IObserver<ChangeAnimationInfo>
	{

	public:
		ChangeAnimObserver(const std::string& _observerName);
		virtual ~ChangeAnimObserver() {}

		/// @brief 通知がきたときの処理
		virtual void ObserverUpdate(const ChangeAnimationInfo& _value) = 0;
	};

	/// @brief 変更通知するサブジェクト
	using ChangeAnimSubject = Subject<ChangeAnimationInfo>;
}



