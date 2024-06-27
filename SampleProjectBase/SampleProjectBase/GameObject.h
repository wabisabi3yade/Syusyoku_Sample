#pragma once
#include "Tag.h"
#include "Component.h"
#include "MaterialClass.h"
// Json
#include "SaveJson.h"
#include "LoadJson.h"
#include "SaveJsonValue.h"
// ImGui
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

class SceneObjects;
// シーンで使用するオブジェクト全般の基底クラス
class GameObject
{
protected:
	bool isActive;	// アクティブ状態かどうか
	std::string name;	// このオブジェクトの名前
	Tag tag;	// ゲームオブジェクトのタグ
	Layer layer;	// ゲームオブジェクトのレイヤー
	std::list<std::unique_ptr<Component>> pComponents;	// コンポーネントリスト
	//std::unique_ptr<SaveJsonValue> saveValues;	// セーブをする変数クラス

	void ActiveProcess();	// アクティブに変更したときの処理
	void NotActiveProcess();	// 非アクティブに変更したときの処理

	virtual void Update(){};	// 更新処理

	virtual void FromJson(const nlohmann::json& _jsonData) {};
public:
	Transform transform;	// Transformパラメータ

	GameObject();
	virtual ~GameObject() {};

	void UpdateBase();	// どのオブジェクトも行う処理はここ
	virtual void LateUpdate();	// Updateを行ったあとの更新処理
	virtual void Draw();	// 描画処理

	void Destroy();	// このオブジェクトを削除する

	template<typename T> T* AddComponent();	// コンポーネントをつける
	template<typename T> T* GetComponent();	// コンポーネントを取得

	virtual void ImGuiSet();	// ImGuiの設定

	//void ToJsonBase(); // Jsonファイルに書き込む
	//void FromJsonBase(const nlohmann::json& _jsonData);	// Jsonファイルからロードする
	
	void SetName(const std::string& _name) { name = _name; }	// 名前
	void SetActive(bool _isActive);	// アクティブ状態を変更する

	const std::string& GetName() { return name; }
	bool GetIsActive() { return isActive; }	// Active状態か取得
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
