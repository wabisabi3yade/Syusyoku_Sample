#pragma once

namespace HashiTaku
{
	/// @brief Observerパターン
	class IObserver
	{
	public:
		IObserver() {}
		~IObserver() = default;

		/// @brief 通知がきたときの処理
		virtual void Update() = 0;
	};

	/// @brief 通知するクラス
	class Subject
	{
		/// @brief オブザーバー
		std::list<IObserver*> observers;

	public:
		Subject() {}
		~Subject() = default;

		/// @brief オブザーバー追加
		/// @param _observer 追加するオブザーバー
		void AddObserver(IObserver& _observer);

		/// @brief オブザーバー削除
		/// @param _observer 削除するオブザーバー
		void RemoveObserver(IObserver& _observer);

		/// @brief オブザーバーに通知する
		void NotifyAll();
	};
}


