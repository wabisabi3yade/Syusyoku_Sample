#pragma once

namespace HashiTaku
{
	/// @brief Observer�p�^�[��
	class IObserver
	{
	public:
		IObserver() {}
		~IObserver() = default;

		/// @brief �ʒm�������Ƃ��̏���
		virtual void Update() = 0;
	};

	/// @brief �ʒm����N���X
	class Subject
	{
		/// @brief �I�u�U�[�o�[
		std::list<IObserver*> observers;

	public:
		Subject() {}
		~Subject() = default;

		/// @brief �I�u�U�[�o�[�ǉ�
		/// @param _observer �ǉ�����I�u�U�[�o�[
		void AddObserver(IObserver& _observer);

		/// @brief �I�u�U�[�o�[�폜
		/// @param _observer �폜����I�u�U�[�o�[
		void RemoveObserver(IObserver& _observer);

		/// @brief �I�u�U�[�o�[�ɒʒm����
		void NotifyAll();
	};
}


