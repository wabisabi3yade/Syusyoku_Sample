#pragma once
#include "Component.h"

class ComponentRespawner_Base;

/// @brief シングルトンなコンポーネント
template<class T>
class SingletonComponent : public Component
{
	// コンポーネント自体の生成はコンポーネント生成クラスに任せる
	friend class ComponentRespawner_Base;

	/// @brief インスタンス
	static T* pInstance;

public:
	/// @brief インスタンスを取得
	/// @return インスタンス
	static inline T* GetInstance();

	/// @brief 削除処理
	void OnDestroy() override;
protected:
	SingletonComponent() {}
	virtual ~SingletonComponent() {}

	void Init() override;
private:
	SingletonComponent(const  SingletonComponent&) = delete;
	SingletonComponent& operator=(const  SingletonComponent&) = delete;
	SingletonComponent(SingletonComponent&&) = delete;
	SingletonComponent& operator=(SingletonComponent&&) = delete;
	

	/// @brief インスタンスを削除する
	void Destroy();
};

// 初期化
template <class T> T* SingletonComponent<T>::pInstance = nullptr;

template<class T>
inline T* SingletonComponent<T>::GetInstance()
{
	// 生成されていなければ実態は無い
	return pInstance;
}

template<class T>
inline void SingletonComponent<T>::OnDestroy()
{
	Destroy();
}

template<class T>
inline void SingletonComponent<T>::Init()
{
	// 既に生成されているなら
	if (pInstance)
	{
		HASHI_DEBUG_LOG(GetName() + "既にシングルトンインスタンスは生成されています");
		return;
	}

	// 自分自身を代入
	pInstance = static_cast<T*>(this);
}

template<class T>
inline void SingletonComponent<T>::Destroy()
{
	pInstance = nullptr;
}
