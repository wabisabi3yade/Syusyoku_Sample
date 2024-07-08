#pragma once
#include <iostream>

template <class T>
class Singleton_Base
{
	static T* instance;

protected:
	Singleton_Base() {};
	virtual ~Singleton_Base() {};

public:
	static inline T* GetInstance();
	static inline void Delete();

private:
	Singleton_Base(const Singleton_Base&) = delete;
	Singleton_Base& operator=(const Singleton_Base&) = delete;
	Singleton_Base(Singleton_Base&&) = delete;
	Singleton_Base& operator=(Singleton_Base&&) = delete;

};
// ������
template <class T> T* Singleton_Base<T>::instance = nullptr;

template<class T>
inline T* Singleton_Base<T>::GetInstance()
{
	// �C���X�^���X���m�ۂ���Ă��Ȃ��Ȃ�
	if (!instance)
	{
		// �C���X�^���X���m�ۂ���
		instance = new T();
	}

	return instance;
}

template<class T>
inline void Singleton_Base<T>::Delete()
{
	CLASS_DELETE(instance);
}