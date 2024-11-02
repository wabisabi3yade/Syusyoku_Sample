#pragma once
#include <iostream>

template <class T>
class Singleton_Base
{
	/// @brief �C���X�^���X
	static T* pInstance;
public:
	/// @brief �C���X�^���X���擾����
	/// @return �C���X�^���X
	static inline T* GetInstance();

	/// @brief �C���X�^���X���폜����
	static inline void Delete();

protected:
	Singleton_Base() {};
	virtual ~Singleton_Base() {};

private:
	// �R�s�[�ł��Ȃ��悤�ɂ���
	Singleton_Base(const Singleton_Base&) = delete;
	Singleton_Base& operator=(const Singleton_Base&) = delete;
	Singleton_Base(Singleton_Base&&) = delete;
	Singleton_Base& operator=(Singleton_Base&&) = delete;

};
// ������
template <class T> T* Singleton_Base<T>::pInstance = nullptr;

template<class T>
inline T* Singleton_Base<T>::GetInstance()
{
	// �C���X�^���X���m�ۂ���Ă��Ȃ��Ȃ�
	if (!pInstance)
	{
		// �C���X�^���X���m�ۂ���
		pInstance = new T();
	}

	return pInstance;
}

template<class T>
inline void Singleton_Base<T>::Delete()
{
	CLASS_DELETE(pInstance);
}