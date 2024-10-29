#pragma once

namespace HashiTaku
{
	/// @brief Observerパターン
	template<typename T>
	class IObserver
	{
		std::string observerName;

	public:
		IObserver(const std::string& _observerName) : observerName(_observerName) {}
		virtual ~IObserver() {}

		/// @brief 通知がきたときの処理
		virtual void ObserverUpdate(const T& _value) = 0;

		std::string GetObserverName() const;
	};

	/// @brief サブジェクト基底クラス
	/// @tparam T オブザーバーへ通知時渡す変数の型
	template<typename T>
	class Subject
	{
		/// @brief オブザーバー
		std::list<IObserver<T>*> pObservers;

	public:
		Subject() {}
		~Subject() {}

		/// @brief オブザーバー追加
		/// @param _observer 追加するオブザーバー
		void AddObserver(IObserver<T>& _observer);

		/// @brief オブザーバー削除
		/// @param _observer 削除するオブザーバー
		void RemoveObserver(IObserver<T>& _observer);

		/// @brief オブザーバーに通知する
		void NotifyAll(const T& _value);
	};

	template<typename T>
	inline std::string IObserver<T>::GetObserverName() const
	{
		return observerName;
	}

	template<typename T>
	inline void Subject<T>::AddObserver(IObserver<T>& _observer)
	{
		auto itr = std::find(pObservers.begin(), pObservers.end(), &_observer);

		if (itr != pObservers.end()) return;

		pObservers.push_back(&_observer);
	}

	template<typename T>
	inline void Subject<T>::RemoveObserver(IObserver<T>& _observer)
	{
		pObservers.remove(&_observer);
	}

	template<typename T>
	inline void Subject<T>::NotifyAll(const T& _value)
	{
		for (auto& o : pObservers)
			o->ObserverUpdate(_value);
	}
}


