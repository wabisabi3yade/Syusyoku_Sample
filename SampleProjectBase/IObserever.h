#pragma once

namespace HashiTaku
{
	/// @brief Observer�p�^�[��
	template<typename T>
	class IObserver
	{
		std::string observerName;

	public:
		IObserver(const std::string& _observerName) : observerName(_observerName) {}
		virtual ~IObserver() {}

		/// @brief �ʒm�������Ƃ��̏���
		virtual void ObserverUpdate(const T& _value) = 0;

		std::string GetObserverName() const;
	};

	/// @brief �T�u�W�F�N�g���N���X
	/// @tparam T �I�u�U�[�o�[�֒ʒm���n���ϐ��̌^
	template<typename T>
	class Subject
	{
		/// @brief �I�u�U�[�o�[
		std::list<IObserver<T>*> pObservers;

	public:
		Subject() {}
		~Subject() {}

		/// @brief �I�u�U�[�o�[�ǉ�
		/// @param _observer �ǉ�����I�u�U�[�o�[
		void AddObserver(IObserver<T>& _observer);

		/// @brief �I�u�U�[�o�[�폜
		/// @param _observer �폜����I�u�U�[�o�[
		void RemoveObserver(IObserver<T>& _observer);

		/// @brief �I�u�U�[�o�[�ɒʒm����
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


