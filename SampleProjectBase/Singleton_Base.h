#pragma once
#include <iostream>

template <class T>
class Singleton_Base
{
	/// @brief インスタンス
	static T* pInstance;
public:
	/// @brief インスタンスを取得する
	/// @return インスタンス
	static inline T* GetInstance();

	/// @brief インスタンスを削除する
	static inline void Delete();

protected:
	Singleton_Base() {};
	virtual ~Singleton_Base() {};

private:
	// コピーできないようにする
	Singleton_Base(const Singleton_Base&) = delete;
	Singleton_Base& operator=(const Singleton_Base&) = delete;
	Singleton_Base(Singleton_Base&&) = delete;
	Singleton_Base& operator=(Singleton_Base&&) = delete;

};
// 初期化
template <class T> T* Singleton_Base<T>::pInstance = nullptr;

template<class T>
inline T* Singleton_Base<T>::GetInstance()
{
	// インスタンスが確保されていないなら
	if (!pInstance)
	{
		// インスタンスを確保する
		pInstance = new T();
	}

	return pInstance;
}

template<class T>
inline void Singleton_Base<T>::Delete()
{
	CLASS_DELETE(pInstance);
}