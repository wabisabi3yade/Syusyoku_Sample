#pragma once
#include "GameObject.h"

class Camera;
// シーンで使用するオブジェクト配列クラス
class SceneObjects
{
	// シーンで使用するオブジェクト配列（オブジェクトの名前がキー値）
	std::unordered_map<std::string, std::unique_ptr<GameObject>> list;
	// UI用のリスト(描画を上のリストより後にするため)
	std::unordered_map<std::string, std::unique_ptr<GameObject>> uiList;
public:
	SceneObjects();
	~SceneObjects();

	void Update();	// 更新処理
	void LateUpdate();	// Updateのあとの更新処理
	void Draw();	// 描画処理(カメラとの距離で描画順を決める)

	// 配列にオブジェクトを入れる
	void SetObject(const std::string& _objectName, std::unique_ptr<GameObject> _objPtr);

	// オブジェクトの名前からオブジェクトを返す
	template<class T> T* GetSceneObject(const std::string& _objectName);

	// オブジェクトの数を返す
	u_int GetObjectsNum()const { return static_cast<u_int>(list.size()); }
};

template<class T>
inline T* SceneObjects::GetSceneObject(const std::string& _objectName)
{
	// 先に3D空間状のオブジェクトから探す　→　無かったらUIの方を探す
	
	// 名前から探す
	auto itr = list.find(_objectName);
	if (itr == list.end())
	{
		// オブジェクトに無かったらUiで探す
		itr = uiList.find(_objectName);

		// それでもなかったら
		if (itr == list.end())
		{
			std::string message = "リスト内に名前のオブジェクトがありませんでした" + _objectName;
			ImGuiDebugLog::Add(message);
			return nullptr;
		}
	}

	// 指定した型に変換する(GameObjectを派生しているクラスのみ)
	T* retPtr = dynamic_cast<T*>(itr->second.get());
	if (retPtr == nullptr)	// GameObjectを派生していないクラスだったら
	{
		std::string message = _objectName + "を指定した型に変換できませんでした";
		ImGuiDebugLog::Add(message);
		return nullptr;
	}

	return retPtr;
}
