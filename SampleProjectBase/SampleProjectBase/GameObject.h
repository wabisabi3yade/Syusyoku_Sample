#pragma once
#include <typeinfo>
#include "Tag.h"
#include "Component.h"
#include "MaterialClass.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

// シーンで使用するオブジェクト全般の基底クラス
class GameObject
{
protected:
	bool isActive;	// アクティブ状態かどうか
	std::string name;	// このオブジェクトの名前

	Tag tag;	// ゲームオブジェクトのタグ
	Layer layer;	// ゲームオブジェクトのレイヤー

	DirectX::SimpleMath::Vector2 uvScroll;	// UVスクロール値

	std::list<std::unique_ptr<Component>> pComponents;	// コンポーネントリスト

	void ActiveProcess();	// アクティブに変更したときの処理
	void NotActiveProcess();	// 非アクティブに変更したときの処理
public:
	Transform transform;	// Transformパラメータ

	GameObject() : isActive(true), name(""), uvScroll(DirectX::SimpleMath::Vector2::Zero) {};
	virtual ~GameObject() {};

	virtual void Update();	// 更新処理
	virtual void LateUpdate();	// Updateを行ったあとの更新処理
	virtual void Draw();	// 描画処理

	template<typename T> T* AddComponent();	// コンポーネントをつける
	template<typename T> T* GetComponent();	// コンポーネントを取得

	virtual void ImGuiSet();	// ImGuiの設定

	void SetName(const std::string& _name) { name = _name; }	// 名前
	const std::string& GetName() { return name; }

	// アクティブ状態を変更する
	void SetActive(bool _isActive);

	// Active状態か取得
	bool GetIsActive() { return isActive; }

	// タグ・レイヤーを取得
	const Tag& GetTag() { return tag; }
	const Layer& GetLayer() { return layer; }
};

template<typename T>
inline T* GameObject::AddComponent()
{
	// コンポーネントを作成
	auto t = this;
	std::unique_ptr<T> addComp = std::make_unique<T>(t);

	Component* comp = dynamic_cast<Component*>(addComp.get());
	if (comp == nullptr)	// コンポーネントクラスを継承してるかチェック
	{
		ImGuiDebugLog::Add("AddComponent：Componentを継承していないので処理できません");
		return nullptr;
	}
	comp->Init();	// 初期処理
	T* retPtr = addComp.get();	// 返すポインタを取得しておく
	
	pComponents.push_back(std::move(addComp));	// リストに追加
	
	return retPtr;
}

template<typename T>
inline T* GameObject::GetComponent()
{
	// 指定した型名と同じコンポーネントがあるか確認
	for (auto& comp : pComponents)
	{
		if (typeid(T) != typeid(*comp.get())) continue;
		T* retPtr = static_cast<T*>(comp.get());	// 変換
		return retPtr; // あるなら返す
	}

	return nullptr;	// 無かったらnullptr
}
