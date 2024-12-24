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

namespace HashiTaku
{
	// シーンで使用するオブジェクト全般の基底クラス
	class GameObject : public ISaveLoad, public IImGuiUser
	{
		friend class SceneObjects;

		/// @brief このオブジェクトの名前
		std::string name;

		/// @brief トランスフォーム
		std::unique_ptr<Transform> pTransform;

		/// @brief 
		std::unique_ptr<CollisionTypeJudge> pCollisionJudge;

		// コンポーネントリスト
		std::list<std::unique_ptr<Component>> components;

		/// @brief アクティブ状態のコンポーネント
		std::list<Component*> pActiveComponents;

		/// @brief Awake処理を行うコンポーネント
		std::list<Component*> pAwakeComponents;

		/// @brief Start処理を行うコンポーネント
		std::list<Component*> pStartComponents;

		/// @brief タグ
		Tag tag;

		/// @brief レイヤー
		Layer layer;

		/// @brief オブジェクト自身の経過時間速度
		float deltaTimeSpeed;

		/// @brief オブジェクト自身の経過時間
		float deltaTime;

		/// @brief アクティブ状態かどうか
		bool isActive;

		/// @brief 削除するか？
		bool isDestroy;
	public:
		GameObject();
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
		void OnCollisionEnter(const CollisionInfo& _otherColInfo);
		/// @brief 衝突中の処理
		void OnCollisionStay(const CollisionInfo& _otherColInfo);
		/// @brief 衝突終了時の処理
		void OnCollisionExit(const CollisionInfo& _otherColInfo);


		/// @brief 移動座標が変更した時のコールバック
		void OnChangePosition();
		/// @brief 移動座標が変更した時のコールバック
		void OnChangeScale();
		/// @brief 移動座標が変更した時のコールバック
		void OnChangeRotation();

		/// @brief コンポーネントをセットするときの処理
		/// @param _pSetComponent セットするコンポーネント
		void SetComponent(std::unique_ptr<Component> _pSetComponent);

		/// @brief 削除するようにする
		void SetDestroy();

		/// @brief コンポーネントをアタッチ
		template<ComponentConcept T> T* AddComponent();

		/// @brief コンポーネントを削除する
		void DeleteComponent(Component& _deleteComonent);

		/// @brief/ コンポーネントを取得
		template<class T> T* GetComponent();

		/// @brief アクティブ配列から外す・追加する
		void RemoveActiveComponent(Component& _removeComonent);

		/// @brief 活動状態リストに追加
		void AddActiveComponent(Component& _addComonentComonent);

		/// @brief セーブする
		/// @param _sceneData セーブシーンデータ
		json Save() override;

		/// @brief ロードする
		/// @param _sceneData ロードするシーンデータ 
		void Load(const json& _sceneData) override;

		/// @brief オブジェクト関係をロードする(シーン内のオブジェクトが必要)
		/// @param _sceneData ロードするシーンデータ 
		void LateLode(const json& _sceneData);

		/// @brief オブジェクト名をセット
		/// @param _name オブジェクト名
		void SetName(const std::string& _name);

		/// @brief 活動状態をセット
		/// @param _isActive セットしたい活動状態
		void SetActive(bool _isActive);

		/// @brief タグをセット
		/// @param _setType セットしたいタグの種類
		void SetTag(Tag::Type _setType);

		/// @brief レイヤーをセット
		/// @param _setType セットしたいレイヤーの種類
		void SetLayer(Layer::Type _setType);

		/// @brief 経過時間を進める速度をセット
		/// @param _deltaSpeed 経過時間を進める速度(1.0が等速)
		void SetDeltaTimeSpeed(float _deltaSpeed);

		/// @brief トランスフォームを取得
		/// @return トランスフォームの参照
		Transform& GetTransform();

		/// @brief トランスフォームを取得
		/// @return トランスフォームの参照
		const Transform& GetConstTransform() const;

		/// @brief オブジェクト名を取得
		/// @return オブジェクト名
		const std::string& GetName() const;

		/// @brief オブジェクトの経過時間を取得する
		/// @return オブジェクトの経過時間
		float DeltaTime() const;

		/// @brief Δt速度を取得
		/// @return Δt
		float GetDeltaSpeed() const;

		/// @brief 活動状態を取得
		/// @return 活動状態
		bool GetIsActive() const;

		/// @brief タグの種類を取得
		/// @return タグの種類
		Tag::Type GetTag() const;

		/// @brief レイヤーの種類を取得
		/// @return レイヤーの種類
		Layer::Type GetLayer() const;
	private:
		/// @brief 経過時間を更新する
		void UpdateDeltaTime();

		/// @brief アクティブ状態を切り替えた時に起こす処理
		void OnActiveTrue();

		/// @brief 非アクティブ状態を切り替えた時に起こす処理
		void OnActiveFalse();

		/// @brief 自身を削除
		void Destroy();

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
		template<class T> bool isDuplicateCompoent();

		/// @brief コンポーネントを並べ変える
		void SortCompPriority();

		/// @brief コンポーネントの優先度が降順に並べていくソート関数
		static bool SortCompPriorityFunc(const Component* _c1, const Component* _c2);

		/// @brief コンポーネント生成ロード処理
		/// @param _componentsData コンポーネントデータ
		void LoadCreateComponnet(const json& _componentsData);

		/// @brief コンポーネントのロード処理
		/// @param _componentData このオブジェクト全てのコンポーネントデータ
		void LoadComponentParameter(const json& _componentsData);

		// ImGuiの設定
		void ImGuiDebug() override;

		// ImGuiで親状態の設定
		void ImGuiSetParent();
	};

	template<ComponentConcept T>
	inline T* GameObject::AddComponent()
	{
		// コンポーネントファクトリーから取得
		std::unique_ptr<T> createComp = ComponentFactory::GetInstance()->Create<T>();
		T* comp = createComp.get();

		// コンポーネント追加処理
		SetComponent(std::move(createComp));

		return comp;
	}

	template<class T>
	inline T* GameObject::GetComponent()
	{
		// 指定した型名と同じコンポーネントがあるか確認
		for (auto& comp : components)
		{
			if (T* getPtr = dynamic_cast<T*>(comp.get()))
			{
				return getPtr;
			}
		}

		return nullptr;
	}

	template<class T>
	inline bool GameObject::isDuplicateCompoent()
	{
		// 指定した型名と同じコンポーネントがあるか確認
		for (auto& comp : components)
		{
			if (typeid(T) == typeid(*comp))
				return true;
		}

		return false;
	}
}