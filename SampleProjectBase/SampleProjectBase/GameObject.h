#pragma once
#include "Tag.h"
#include "Component.h"
#include "MaterialClass.h"

// シーン関数
#include "SF_Include.h"

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
	// アクティブ状態かどうか
	bool isActive;	

	// このオブジェクトの名前
	std::string name;	

	// タグ
	Tag tag;

	// レイヤー
	Layer layer;	

	// コンポーネントリスト
	std::list<std::unique_ptr<Component>> pComponents;

	// セーブをする変数リスト
	//std::unique_ptr<SaveJsonValue> saveValues;	

	// アクティブ変更時の処理
	void ActiveProcess();

	// 非アクティブ変更時の処理
	void NotActiveProcess();

	// 更新処理
	virtual void Update() {};	

	// Jsonからロード
	virtual void FromJson(const nlohmann::json& _jsonData) {};

	virtual GameObject& Copy(const GameObject& _other);
public:
	Transform transform;	

	GameObject();
	GameObject(const GameObject& _other);	
	GameObject& operator=(const GameObject& _other);
	virtual ~GameObject() {};

	// どのオブジェクトも行う処理はここ
	void UpdateBase();	

	// Updateを行ったあとの更新処理
	virtual void LateUpdate();

	void DrawBase();
	// 描画処理
	virtual void Draw(){};

	// 自身を削除
	void Destroy();

	// コンポーネントをアタッチ
	template<typename T> T* AddComponent();	

	// コンポーネントを取得
	template<typename T> T* GetComponent();	

	// ImGuiの設定
	virtual void ImGuiSet();	

	// Jsonファイルに書き込む
	//void ToJsonBase(); 
	
	// Jsonファイルからロードする
	//void FromJsonBase(const nlohmann::json& _jsonData);	

	void SetName(const std::string& _name) { name = _name; }
	void SetActive(bool _isActive);

	const std::string& GetName() { return name; }
	bool GetIsActive() { return isActive; }
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
		T* retPtr = static_cast<T*>(comp.get());	
		return retPtr; 
	}

	return nullptr;
}
