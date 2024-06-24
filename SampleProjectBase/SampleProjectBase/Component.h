#pragma once
class GameObject;

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// ゲームのサブシステムとなるコンポーネントの基底クラス
class Component
{
	bool isStartYet;	// まだStart関数が処理されていないフラグ
protected:
	GameObject* gameObject;	// コンポーネント所持しているオブジェクト

	Transform& GetTransform();	// トランスフォーム
public:
	Component(GameObject* _gameObject) : gameObject(_gameObject), isStartYet(false) {};
	virtual ~Component(){};

	virtual void Init() {};	// AddComponentされたときに行う
	virtual void Start() {};	// 最初の更新フレームで行う処理
	virtual void Update() {};	// 毎フレーム更新処理
	virtual void LateUpdate() {};	// Updateの後に行う更新処理
	virtual void Draw() {};	// 描画処理

	virtual void SetParameter() {};	// ImGuiでパラメータを変える処理
	void StartComplete() { isStartYet = true; }	// Start処理が終わった処理

	GameObject& GetGameObject() { return *gameObject; }	// ゲームオブジェクト取得
	bool GetIsStartYet() { return isStartYet; }
};

