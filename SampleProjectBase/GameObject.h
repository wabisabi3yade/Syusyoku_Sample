#pragma once

// メンバ変数
#include "Tag.h"
// コンポーネント生成
#include "ComponentFactory.h"
// シーン関数
#include "SF_Define.h"
// セーブ・ロード
#include "ISaveLoad.h"

#include "CollisionInfo.h"

// シーンで使用するオブジェクト全般の基底クラス
class GameObject : public HashiTaku::ISaveLoad, public HashiTaku::IImGuiUser
{
private:
	// このオブジェクトの名前
	std::string name;	

	/// @brief トランスフォーム
	std::unique_ptr<Transform> pTransform;

	/// @brief 
	std::unique_ptr<CollisionTypeJudge> pCollisionJudge;

	// コンポーネントリスト
	std::list<std::unique_ptr<Component>> pComponents;

	/// @brief アクティブ状態のコンポーネント
	std::list<Component*> pActiveComponents;

	/// @brief Awake処理を行うコンポーネント
	std::list<Component*> pAwakeComponents;

	/// @brief Start処理を行うコンポーネント
	std::list<Component*> pStartComponents;

	// タグ
	Tag tag;

	// レイヤー
	Layer layer;

	// アクティブ状態かどうか
	bool isActive;
public:
	GameObject();
	GameObject(const GameObject& _other);
	GameObject& operator=(const GameObject& _other);
	~GameObject();

	/// @brief 必ず最初に呼び出される処理
	void AwakeCall();
	/// @brief Awakeの次に呼び出される処理
	void StartCall();
	/// @brief 毎フレーム更新する処理
	void UpdateCall();	
	/// @brief Updateよりも後に更新される処理
	void LateUpdateCall();
	/// @brief 描画処理
	void DrawCall();

	/// @brief 衝突開始時の処理
	void OnCollisionEnter(const HashiTaku::Bullet::CollisionInfo& _otherColInfo);
	/// @brief 衝突中の処理
	void OnCollisionStay(const HashiTaku::Bullet::CollisionInfo& _otherColInfo);
	/// @brief 衝突終了時の処理
	void OnCollisionExit(const HashiTaku::Bullet::CollisionInfo& _otherColInfo);

	/// @brief 自身を削除
	void Destroy();

	/// @brief 移動座標が変更した時のコールバック
	void OnChangePosition();
	/// @brief 移動座標が変更した時のコールバック
	void OnChangeScale();
	/// @brief 移動座標が変更した時のコールバック
	void OnChangeRotation();

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

	/// @brief セーブする
	/// @param _sceneData セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _sceneData ロードするシーンデータ 
	void Load(const nlohmann::json& _sceneData) override;

	/// @brief オブジェクト関係をロードする(シーン内のオブジェクトが必要)
	/// @param _sceneData ロードするシーンデータ 
	void LateLode(const nlohmann::json& _sceneData);

	void SetName(const std::string& _name);
	void SetActive(bool _isActive);

	Transform& GetTransform();
	const std::string& GetName() const { return name; }
	bool GetIsActive() const { return isActive; }
	Tag GetTag() const { return tag; }
	Layer GetLayer() const  { return layer; }
private:
	virtual GameObject& Copy(const GameObject& _other);

	// アクティブ状態を切り替えた時に起こす処理
	void OnActiveTrue();
	void OnActiveFalse();

	void ImGuiSetParent();

	/// @brief 所持している配列にコンポーネントが存在するか
	/// @param _pCheckComponent 確認するコンポーネント
	/// @return 存在したか？
	bool IsExistComponent(const Component& _pCheckComponent);

	/// @brief	アクティブ状態のコンポーネント内にが存在するか
	/// @param _pCheckComponent 確認するコンポーネント
	/// @return 存在したか？
	bool IsExistActiveComponent(const Component& _pCheckComponent);

	/// @brief Awake配列に存在するか
	/// @param _pCheckComponent 確認するコンポーネント
	/// @return 存在したか？
	bool IsExistAwakeComponent(const Component& _pCheckComponent);

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

	// ImGuiの設定
	void ImGuiSetting() override;
};

template<HashiTaku::ComponentConcept T>
inline T* GameObject::AddComponent()
{
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
		if (T* getPtr = dynamic_cast<T*>(comp.get()))
		{
			return getPtr;
		}
	}

	return nullptr;
}

template<HashiTaku::ComponentConcept T>
inline bool GameObject::isDuplicateCompoent()
{
	// 指定した型名と同じコンポーネントがあるか確認
	for (auto& comp : pComponents)
	{
		if (typeid(T) == typeid(*comp))
			return true;
	}

	return false;
}
