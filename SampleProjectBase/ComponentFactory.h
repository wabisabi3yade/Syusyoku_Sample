#pragma once

// コンポーネントヘッダー
#include "ComponentDefine.h"

/// @brief コンポーネントのファクトリーメソッド
class ComponentFactory
{
public:

	/// @brief コンポーネント作成
	/// @tparam T コンポーネントの型
	/// @return コンポーネントのユニークポインタ
	template<class T> static std::unique_ptr<T> Create();
};

/// @brief コンポーネント作成
/// @tparam T コンポーネントの型
/// @return コンポーネントのユニークポインタ
template<class T>
inline std::unique_ptr<T> ComponentFactory::Create()
{
	std::unique_ptr<T> createComp = std::make_unique<T>();

	// コンポーネントを継承していないなら
	if (dynamic_cast<Component*>(createComp.get()) == nullptr)
	{
		std::string compName = std::string(typeid(T).name());

		HASHI_DEBUG_LOG(compName + "はComponentではありません");
		return nullptr;
	}

	return std::move(createComp);
}
