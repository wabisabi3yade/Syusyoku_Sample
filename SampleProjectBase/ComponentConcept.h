#pragma once

#include "Component.h"

namespace HashiTaku
{
	// コンセプトで取得できるリソースの型を指定
	template<typename T>
	concept ComponentConcept = std::is_base_of_v<Component, T>;
}
