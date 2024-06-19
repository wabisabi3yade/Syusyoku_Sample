#pragma once
class GameObject;

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// ゲームのサブシステムとなるコンポーネントの基底クラス
class Component
{
protected:
	GameObject* gameObject;	// コンポーネント所持しているオブジェクト

public:
	Component(GameObject* _gameObject) : gameObject(_gameObject) {};
	virtual ~Component(){};

	virtual void Init() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void Draw() {};

	virtual void SetParameter() {};
};

