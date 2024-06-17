#pragma once
class GameObject;

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

	virtual void ImGuiParamater() {};
};

