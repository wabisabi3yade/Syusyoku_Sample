#pragma once
class GameObject;

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

// ゲームのサブシステムとなるコンポーネントの基底クラス
class Component
{
	friend class GameObject;

protected:
	// 活動フラグ
	bool isEnable;

	// 名前
	std::string name;	

	// このコンポーネントの所持オブジェクト
	GameObject* gameObject;	

public:
	Component() : isEnable(true), name(""), gameObject(nullptr) {}
	virtual ~Component(){};

	// コピー代入演算子
	Component& operator=(const Component & _other);

	// AddComponentされたときに行う
	virtual void Init() {};	

	// 毎フレーム更新処理
	virtual void Update() {};	

	// Updateの後に行う更新処理
	virtual void LateUpdate() {};

	// 描画処理
	virtual void Draw() {};

	// オブジェクトのトランスフォーム
	Transform& GetTransform() const;

	// ImGuiでパラメータを変える処理
	virtual void ImGuiSetting() {};	

	const GameObject& GetGameObject() { return *gameObject; }	// ゲームオブジェクト取得

private:
	void Copy(const Component& _other);
};

