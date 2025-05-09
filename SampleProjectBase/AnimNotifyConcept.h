#pragma once
#include "AnimationNotify_Base.h"

namespace HashiTaku
{
	// コンセプトで取得できるリソースの型を指定
	template<typename T>
	concept NotifyConcept = std::is_base_of_v<AnimationNotify_Base, T>;
}