#pragma once
#include "Model.h"

// 画面に写されるオブジェクトクラス
class GameObject
{
public:
	Transform transform;

	GameObject();
	virtual ~GameObject();
};

