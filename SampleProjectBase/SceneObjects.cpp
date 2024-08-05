#include "pch.h"
#include "SceneObjects.h"
#include <string>
#include "Camera.h"

typedef std::unordered_map<std::string, std::unique_ptr<GameObject>> ObjectList;

SceneObjects::SceneObjects()
{
}

SceneObjects::~SceneObjects()
{
	objList.clear();	// 解放する
	uiList.clear();
}

void SceneObjects::Update()
{
#ifdef EDIT
	ImGui::Begin(ShiftJisToUtf8("シーンオブジェクト").c_str());
#endif // EDIT

	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->UpdateBase();
		itr->second->ImGuiSet();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->UpdateBase();
		itr->second->ImGuiSet();
	}

#ifdef EDIT
	ImGui::End();
#endif // EDIT


}

void SceneObjects::LateUpdate()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->LateUpdateBase();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->LateUpdateBase();
	}
}

void SceneObjects::Draw()
{
	// 3D空間上のオブジェクト描画
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->DrawBase();
	}


	// ↓平行投影をさせる //


	//------------------//

	// 2D空間（UI）のオブジェクト
	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->DrawBase();
	}
}

GameObject* SceneObjects::SetObject(std::unique_ptr<GameObject> _objPtr)
{
	// 名前が空か確認
	CheckEmptyName(*_objPtr.get());

	// 名前が重複しているか確認
	CheckDuplicationName(*_objPtr.get(), objList);
	CheckDuplicationName(*_objPtr.get(), uiList);

	// セットするリスト（オブジェクト側かUIか）
	ObjectList* setList = &objList;

	// UIなら
	if (IsUI(*_objPtr.get()))
		setList = &uiList;	// UIリストにセットする

	GameObject* retPtr = _objPtr.get();

	// 配列に入れる
	std::string setName = _objPtr->GetName();	
	setList->insert(std::pair<std::string, std::unique_ptr<GameObject>>(setName, std::move(_objPtr)));

	return retPtr;
}

void SceneObjects::DeleteObj(GameObject& _deleteObj)
{
	// 配列内に同じアドレスを探す
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		if (itr->second.get() != &_deleteObj) continue;

		// あったら
		objList.erase(itr);	// 削除する
		return;
	}

	// 配列内に同じアドレスを探す
	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		if (itr->second.get() != &_deleteObj) continue;

		// あったら
		uiList.erase(itr);	// 削除する
		return;
	}
}

void SceneObjects::CheckEmptyName(GameObject& _gameObject)
{
	if (_gameObject.GetName() != "") return;

	_gameObject.SetName("Empty");
}

void SceneObjects::CheckDuplicationName(GameObject& _gameObject, Objects& _objects)
{
	std::string objName = _gameObject.GetName();

	std::string setName;

	// 同じ名前のオブジェクトがあった場合名前の後ろに数字をつける
	u_int loop = 0;
	while (true)	// セットできるまで
	{
		// オブジェクトの名前＋数字
		std::string number = std::to_string(loop);
		if (loop == 0)
		{
			number = "";
		}
		setName = objName + number;

		auto itr = _objects.find(setName);
		// 探して無かったら
		if (itr == _objects.end())
		{
			break;	// ループを終わる
		}

		// あるなら
		loop++;	// ループ回数を増やす
	}

	_gameObject.SetName(setName);
}

bool SceneObjects::IsUI(GameObject& _gameObject)
{
	Layer::Type layer = _gameObject.GetLayer().GetType();
	return layer == Layer::Type::UI ? true : false;
}

