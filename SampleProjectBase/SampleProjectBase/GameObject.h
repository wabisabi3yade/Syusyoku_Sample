#pragma once
#include <typeinfo>
#include "Tag.h"
#include "Component.h"

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
	for (auto comp : pComponents)
	{
		if (typeid(std::unique_ptr<T>) != typeid(comp)) continue;
		
		return comp.get(); // あるなら返す
	}

	return nullptr;	// 無かったらnullptr
}
