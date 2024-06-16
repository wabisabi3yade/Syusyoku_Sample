#pragma once
#include "Tag.h"

// シーンで使用するオブジェクト全般の基底クラス
class GameObject
{
protected:
	bool isActive;	// アクティブ状態かどうか
	std::string name;	// このオブジェクトの名前

	Tag tag;	// ゲームオブジェクトのタグ
	Layer layer;	// ゲームオブジェクトのレイヤー

	DirectX::SimpleMath::Vector2 uvScroll;	// UVスクロール値
public:
	Transform transform;	// Transformパラメータ

	GameObject() : isActive(true), name(""), uvScroll(DirectX::SimpleMath::Vector2::Zero) {};
	virtual ~GameObject() {};

	virtual void Update(){};	// 更新処理
	virtual void LateUpdate() {};	// Updateを行ったあとの更新処理
	virtual void Draw() {};	// 描画処理

	virtual void ImGuiSet();	// ImGuiの設定

	void SetName(const std::string& _name) { name = _name; }	// 名前
	const std::string& GetName() { return name; }

	// タグ・レイヤーを取得
	const Tag& GetTag() { return tag; }
	const Layer& GetLayer() { return layer; }
};

