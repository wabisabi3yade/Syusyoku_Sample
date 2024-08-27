#pragma once

// メンバ変数
#include "Tag.h"

// コンポーネント生成
#include "ComponentFactory.h"

// シーン関数
#include "SF_Define.h"

// セーブ・ロード
#include "ISaveLoad.h"

// シーンで使用するオブジェクト全般の基底クラス
class GameObject : public ISaveLoad
{
private:
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

	/// @brief アクティブ状態のコンポーネント
	std::list<Component*> pActiveComponents;

	/// @brief Start処理を行うコンポーネント
	std::list<Component*> pStartComponents;
public:
	/// @brief トランスフォーム
	Transform transform;	

	bool isSave = true;

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

	/// @brief コンポーネントをセットするときの処理
	/// @param _pSetComponent セットするコンポーネント
	void SetComponent(std::unique_ptr<Component> _pSetComponent);

	// コンポーネントをアタッチ
	template<HashiTaku::ComponentConcept T> T* AddComponent();

	// コンポーネントを削除する
	void DeleteComponent(Component& _deleteComonent);

	// コンポーネントを取得
	template<HashiTaku::ComponentConcept T> T* GetComponent();

	// アクティブ配列から外す・追加する
	void RemoveActiveComponent(Component& _removeComonent);
	void AddActiveComponent(Component& _addComonentComonent);

	// ImGuiの設定
	virtual void ImGuiSet();	

	/// @brief セーブする
	/// @param _sceneData セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _sceneData ロードするシーンデータ 
	void Load(const nlohmann::json& _sceneData) override;

	void SetName(const std::string& _name);
	void SetActive(bool _isActive);

	const std::string& GetName() const { return name; }
	bool GetIsActive() const { return isActive; }
	Tag GetTag() const { return tag; }
	Layer GetLayer() const  { return layer; }

private:
	virtual GameObject& Copy(const GameObject& _other);
	void ImGuiSetParent();

	/// @brief 所持している配列にコンポーネントが存在するか
	/// @param _pCheckComponent 確認するコンポーネント
	/// @return 存在したか？
	bool IsExistComponent(const Component& _pCheckComponent);

	/// @brief	アクティブ状態のコンポーネント内にが存在するか
	/// @param _pCheckComponent 確認するコンポーネント
	/// @return 存在したか？
	bool IsExistActiveComponent(const Component& _pCheckComponent);

	/// @brief Start配列に存在するか
	/// @param _pCheckComponent 確認するコンポーネント
	/// @return 存在したか？
	bool IsExistStartComponent(const Component& _pCheckComponent);

	/// @brief コンポーネントを重複しているか
	/// @tparam  コンポーネント
	/// @return 重複しているか？
	template<HashiTaku::ComponentConcept T> bool isDuplicateCompoent();

	/// @brief コンポーネントのロード処理
	/// @param _componentData このオブジェクト全てのコンポーネントデータ
	void LoadComponent(const nlohmann::json& _componentsData);
};

template<HashiTaku::ComponentConcept T>
inline T* GameObject::AddComponent()
{
	if (isDuplicateCompoent<T>())
	{
		HASHI_DEBUG_LOG("重複しているので作成しません");
		return nullptr;
	}

	// コンポーネントファクトリーから取得
	std::unique_ptr<T> createComp = ComponentFactory::GetInstance()->Create<T>();
	T& comp = *createComp;
	
	// コンポーネント追加処理
	SetComponent(std::move(createComp));

	return &comp;
}

template<HashiTaku::ComponentConcept T>
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

template<HashiTaku::ComponentConcept T>
inline bool GameObject::isDuplicateCompoent()
{
	// 指定した型名と同じコンポーネントがあるか確認
	for (auto& comp : pComponents)
	{
		if (typeid(T) == typeid(*comp.get()))
			return true;
	}

	return false;
}
