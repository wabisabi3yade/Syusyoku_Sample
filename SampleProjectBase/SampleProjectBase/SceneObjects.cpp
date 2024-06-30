#include "pch.h"
#include "SceneObjects.h"
#include <string>
#include "Camera.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

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
#ifdef _DEBUG
	ImGui::Begin("SceneObjects");
#endif // _DEBUG

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

#ifdef _DEBUG
	ImGui::End();
#endif // _DEBUG


}

void SceneObjects::LateUpdate()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->LateUpdate();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->LateUpdate();
	}
}

void SceneObjects::Draw()
{
	// 3D空間上のオブジェクト描画
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->Draw();
	}


	// ↓平行投影をさせる //
	

	//------------------//
	
	// 2D空間（UI）のオブジェクト
	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->Draw();
	}
}

GameObject* SceneObjects::SetObject(const std::string& _objectName, std::unique_ptr<GameObject> _objPtr)
{
	// 既にオブジェクトの名前があるなら
	u_int loop = 0;
	std::string setName;	// セットするときの名前

	// セットするリスト（オブジェクト側かUIか）
	ObjectList* setList = &objList;

	if (_objPtr->GetLayer().GetType() == Layer::Type::UI)	// UIなら
	{
		setList = &uiList;	// UIリストにセットする
	}

	// 同じ名前のオブジェクトがあった場合名前の後ろに数字をつける
	while (true)	// セットできるまで
	{
		std::string number = std::to_string(loop);
		if (loop == 0)	// １回目は数字をつけないようにする
		{
			number = "";
		}

		setName = _objectName + number;	// オブジェクトの名前＋数字

		auto itr = setList->find(setName);
		if (itr == setList->end())	// 探して無かったら
		{
			break;	// ループを終わる
		}

		// あるなら
		loop++;	// ループ回数を増やす
	}

	_objPtr->SetName(setName);	// オブジェクトに名前を設定
	GameObject* retPtr = _objPtr.get();
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

void SceneObjects::Start()
{
	// Start処理をする
	for(auto itr = startComponents.begin(); itr != startComponents.end();)
	{
		(*itr)->Start();
		startComponents.erase(itr);
	}
}
