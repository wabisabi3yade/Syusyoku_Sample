#pragma once
#include "GameObject.h"
#include "Light.h"

namespace HashiTaku
{
	class SceneFunction::ObjectFunc;

	using SceneObjectList = std::unordered_map<std::string, std::unique_ptr<GameObject>>;

	// シーンで使用するオブジェクト配列クラス
	class SceneObjects : public IImGuiUser
	{
		friend class SceneFunction::ObjectFunc;

		// シーンで使用するオブジェクト配列（オブジェクトの名前がキー値）
		SceneObjectList objList;
		//std::list<GameObject*> activeObjList;

		// UI用のリスト(描画を上のリストより後にするため)
		SceneObjectList uiList;
		//std::list<GameObject*> activeUIList;

		/// @brief z座標の値でソートした描画UIリスト
		std::list<GameObject*> drawUiList;

		/// @brief オブジェクト配列とUI配列を行き来する為に一時的に格納するリスト
		std::list<GameObject*> tmpMoveList;

		// 配列にオブジェクトを入れる(ObjectFuncからアクセス)
		GameObject* SetObject(std::unique_ptr<GameObject> _objPtr);

	public:
		SceneObjects();
		~SceneObjects();

		/// @brief オブジェクトAwake処理
		void Awake();

		/// @brief オブジェクトStart処理
		void Start();

		/// @brief オブジェクトUpdate処理
		void Update();

		/// @brief オブジェクトLateUpdate処理
		void LateUpdate();

		/// @brief オブジェクトDraw処理
		void ObjectDraw();

		/// @brief UIDraw処理
		void UIDraw();

		//　配列からゲームオブジェクトを削除する
		void DeleteGameObject(GameObject& _deleteObj);

		/// @brief UIとオブジェクトのリスト間を移動する
		/// @param _targetObj 対象オブジェクト
		void MoveTmpList(GameObject& _targetObj);

		/// @brief z値順にUIリストをソートする
		void SortUiList();

		// オブジェクトの名前からオブジェクトを返す
		GameObject* GetSceneObject(const std::string& _objectName);

		// オブジェクトの数を返す
		u_int GetObjectsNum()const { return static_cast<u_int>(objList.size()); }

		// シーンオブジェクトをセーブ・ロード
		json SaveObjectList();
		void LoadObjectList(const json& _objectsData);
	private:
		/// @brief オブジェクトの名前があるか確認
		/// @param _gameObject オブジェクト
		void CheckEmptyName(GameObject& _gameObject);

		/// @brief オブジェクトの名前が他に重複しているか確認
		/// @param _gameObject 
		void CheckDuplicationName(GameObject& _gameObject, SceneObjectList& _objects);

		/// @brief オブジェクトを取り出す
		/// @param _targetObject 対象のオブジェクト
		/// @param _objectName オブジェクト名
		/// @param _outObject オブジェクトのインスタンス
		/// @param _objectList 取り出す対象オブジェクトリスト
		/// @return 成功したかどうか？
		bool TakeOutObject(const GameObject& _targetObject, std::string& _objectName, std::unique_ptr<GameObject>& _outObject, SceneObjectList& _objectList);

		/// @brief UIかどうか
		/// @param _gameObject ゲームオブジェクト 
		/// @return UIフラグ
		bool IsUI(GameObject& _gameObject);

		/// @brief UI描画の前の準備
		void UIDrawSetup();

		/// @brief UI描画終了処理
		void UIDrawEnd();

		/// @brief リスト間で移動するオブジェクトがあるなら移動する
		void MoveList();

		/// @brief UIListに移動させる
		/// @param _gameObject 対象のゲームオブジェクト
		void MoveToUIList(GameObject& _gameObject);

		/// @brief ObjListに移動させる
		/// @param _gameObject 対象のUIオブジェクト
		void MoveToObjList(GameObject& _gameObject);

		/// @brief UI描画リストに追加する
		/// @param _uiObj 追加するUI
		void SetDrawUIList(GameObject& _uiObj);

		/// @brief UI描画リストから削除する
		/// @param _uiObj 削除するUI
		void RemoveDrawUIList(GameObject& _uiObj);

		/// @brief プレハブファイルのパス名を取得
		/// @return プレハブファイルのパス名
		std::string PrefabFileParh();

		/// @brief プレハブを生成
		/// @param _prefabName プレハブ名
		void CreatePrefab(const std::string& _prefabName);

		void ImGuiDebug() override;

		/// @brief オブジェクトをセーブ
		/// @param _go ゲームオブジェクト
		/// @return セーブデータ
		json SaveObject(GameObject& _go);

		/// @brief オブジェクトをプレハブ化する
		/// @param _toPrefabObject プレハブ化するオブジェクト
		void ObjectToPrefab(GameObject& _toPrefabObject);

		/// @brief オブジェクトをロードする
		/// @param _gameObjectData ゲームオブジェクトデータ
		/// @return 成功したか？
		bool LoadObject(const json& _gameObjectData);

		/// @brief オブジェクトを遅れたロードする
		/// @param _gameObjectData ゲームオブジェクトデータ
		/// @return 成功したか？
		bool LateLoadObject(const json& _gameObjectData);

		/// @brief ゲームオブジェクトのImGUi表示
		/// @param _gameObject 表示するゲームオブジェクト
		/// @return deleteしたか
		bool ImGuiSettingObject(GameObject& _gameObject);

		/// @brief UIオブジェクトのZ座標
		static bool SortUIPosZFunction(const GameObject* _a1, const GameObject* _a2);
	};
}