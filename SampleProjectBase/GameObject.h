#pragma once

// メンバ変数
#include "Tag.h"

// コンポーネント生成
#include "ComponentFactory.h"

// シーン関数
#include "SF_Include.h"

// Json
#include "SaveJson.h"
#include "LoadJson.h"
#include "SaveJsonValue.h"

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

	// アクティブ変更時処理
	void ActiveProcess();
	void NotActiveProcess();

	// 各オブジェクトの処理
	virtual void Update() {};	
	virtual void LateUpdate(){};
	virtual void Draw() {};

	// Jsonからロード
	virtual void FromJson(const nlohmann::json& _jsonData) {};

	virtual GameObject& Copy(const GameObject& _other);
public:
	Transform transform;	

	GameObject();
	GameObject(const GameObject& _other);	
	GameObject& operator=(const GameObject& _other);
	virtual ~GameObject() {};

	// 更新処理
	void UpdateBase();	
	void LateUpdateBase();
	void DrawBase();

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

	void SetName(const std::string& _name);
	void SetActive(bool _isActive);

	const std::string& GetName() { return name; }const
	bool GetIsActive() const { return isActive; }
	const Tag& GetTag() { return tag; }
	const Layer& GetLayer() { return layer; }
};

template<typename T>
inline T* GameObject::AddComponent()
{
	// コンポーネントファクトリーから取得
	std::unique_ptr<Component> createComp = ComponentFactory::Create<T>();
	createComp->gameObject = this;

	// 初期処理
	createComp->Init();	

	// 戻り値を取得
	T* retPtr = static_cast<T*>(createComp.get());	

	// リストに追加
	pComponents.push_back(std::move(createComp));	

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
