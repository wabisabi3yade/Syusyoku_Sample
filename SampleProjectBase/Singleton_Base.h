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
// 初期化
template <class T> T* Singleton_Base<T>::instance = nullptr;

template<class T>
inline T* Singleton_Base<T>::GetInstance()
{
	// インスタンスが確保されていないなら
	if (!instance)
	{
		// インスタンスを確保する
		instance = new T();
	}

	return instance;
}

template<class T>
inline void Singleton_Base<T>::Delete()
{
	CLASS_DELETE(instance);
}