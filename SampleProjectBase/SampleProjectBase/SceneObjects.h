#pragma once
#include "GameObject.h"

#ifdef _DEBUG
#include <typeinfo>
#endif // _DEBUG

// シーンで使用するオブジェクト配列クラス
class SceneObjects
{
	// シーンで使用するオブジェクト配列（オブジェクトの名前がキー値）
	std::unordered_map<std::string, std::unique_ptr<GameObject>> list;

public:
	SceneObjects();
	~SceneObjects();

	void Update();	// 更新処理
	void LateUpdate();	// Updateのあとの更新処理
	void Draw();	// 描画処理

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
	// 名前から探す
	auto itr = list.find(_objectName);
	if (itr == list.end())
	{
		std::string message = "リスト内に名前のオブジェクトがありませんでした" + _objectName;
		ImGuiDebugLog::AddDebugLog(message);
		return nullptr;
	}

	// 指定した型に変換する(GameObjectを派生しているクラスのみ)
	T* retPtr = dynamic_cast<T*>(itr->second.get());
	if (retPtr == nullptr)	// GameObjectを派生していないクラスだったら
	{
		std::string message = _objectName + "を指定した型に変換できませんでした";
#ifdef _DEBUG
		message + "　" + typeid(retPtr).name;	// 変換先の型名を表示
#endif // _DEBUG
		ImGuiDebugLog::AddDebugLog(message);
		return nullptr;
	}

	return retPtr;
}
