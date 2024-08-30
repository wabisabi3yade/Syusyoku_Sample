#pragma once
#include "GameObject.h"
#include "Light.h"

class SceneFunction::ObjectFunc;

using Objects = std::unordered_map<std::string, std::unique_ptr<GameObject>>;

// シーンで使用するオブジェクト配列クラス
class SceneObjects
{
	friend class SceneFunction::ObjectFunc;

	// シーンで使用するオブジェクト配列（オブジェクトの名前がキー値）
	Objects objList;

	// UI用のリスト(描画を上のリストより後にするため)
	Objects uiList;

	// 配列にオブジェクトを入れる(ObjectFuncからアクセス)
	GameObject* SetObject(std::unique_ptr<GameObject> _objPtr);

public:
	SceneObjects();
	~SceneObjects();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();	

	void ImGuiSetting();

	//　配列からゲームオブジェクトを削除する
	void DeleteObj(GameObject& _deleteObj);

	// オブジェクトの名前からオブジェクトを返す
	GameObject* GetSceneObject(const std::string& _objectName);

	// オブジェクトの数を返す
	u_int GetObjectsNum()const { return static_cast<u_int>(objList.size()); }

	// シーンオブジェクトをセーブ・ロード
	nlohmann::json SaveObject();
	void LoadObject(const nlohmann::json& _objectsData);
private:

	/// @brief オブジェクトの名前があるか確認
	/// @param _gameObject オブジェクト
	void CheckEmptyName(GameObject& _gameObject);

	/// @brief オブジェクトの名前が他に重複しているか確認
	/// @param _gameObject 
	void CheckDuplicationName(GameObject& _gameObject, Objects& _objects);

	/// @brief UIかどうか
	/// @param _gameObject ゲームオブジェクト 
	/// @return UIフラグ
	bool IsUI(GameObject& _gameObject);
};