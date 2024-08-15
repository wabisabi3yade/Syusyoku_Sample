#pragma once
#include "GameObject.h"
#include "Light.h"

using Objects = std::unordered_map<std::string, std::unique_ptr<GameObject>>;

// シーンで使用するオブジェクト配列クラス
class SceneObjects
{
	// シーンで使用するオブジェクト配列（オブジェクトの名前がキー値）
	Objects objList;

	// UI用のリスト(描画を上のリストより後にするため)
	Objects uiList;

public:
	SceneObjects();
	~SceneObjects();

	// 更新処理
	void Update();

	// Updateのあとの更新処理
	void LateUpdate();	

	// 描画処理
	void Draw();	

	// 配列にオブジェクトを入れる
	GameObject* SetObject(std::unique_ptr<GameObject> _objPtr);

	//　配列からゲームオブジェクトを削除する
	void DeleteObj(GameObject& _deleteObj);

	// オブジェクトの名前からオブジェクトを返す
	template<class T> T* GetSceneObject(const std::string& _objectName);

	// オブジェクトの数を返す
	u_int GetObjectsNum()const { return static_cast<u_int>(objList.size()); }

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

template<class T>
inline T* SceneObjects::GetSceneObject(const std::string& _objectName)
{
	// 先に3D空間状のオブジェクトから探す　→　無かったらUIの方を探す
	
	// 名前から探す
	auto itr = objList.find(_objectName);
	if (itr == objList.end())
	{
		// オブジェクトに無かったらUiで探す
		itr = uiList.find(_objectName);

		// それでもなかったら
		if (itr == uiList.end())
		{
			std::string message = "リスト内に名前のオブジェクトがありませんでした" + _objectName;
			HASHI_DEBUG_LOG(message);
			return nullptr;
		}
	}

	// 指定した型に変換する(GameObjectを派生しているクラスのみ)
	T* retPtr = dynamic_cast<T*>(itr->second.get());
	if (retPtr == nullptr)	// GameObjectを派生していないクラスだったら
	{
		std::string message = _objectName + "を指定した型に変換できませんでした";
		HASHI_DEBUG_LOG(message);
		return nullptr;
	}

	return retPtr;
}
