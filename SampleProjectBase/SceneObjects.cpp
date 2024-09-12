#include "pch.h"
#include "SceneObjects.h"
#include <string>

#include "SF_Define.h"

typedef std::unordered_map<std::string, std::unique_ptr<GameObject>> ObjectList;

SceneObjects::SceneObjects()
{
}

SceneObjects::~SceneObjects()
{
	objList.clear();	// 解放する
	uiList.clear();
}

void SceneObjects::Awake()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->AwakeCall();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->AwakeCall();
	}
}

void SceneObjects::Start()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->StartCall();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->StartCall();
	}
}

void SceneObjects::Update()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->UpdateCall();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->UpdateCall();
	}
}

void SceneObjects::LateUpdate()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->LateUpdateCall();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->LateUpdateCall();
	}
}

void SceneObjects::UpdateRigidBody()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->AwakeCall();
	}
}

void SceneObjects::Draw()
{
	// 3D空間上のオブジェクト描画
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->DrawCall();
	}


	// ↓平行投影をさせる //


	//------------------//

	// 2D空間（UI）のオブジェクト
	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->DrawCall();
	}
}

void SceneObjects::ImGuiSetting()
{
#ifdef EDIT

	ImGui::Begin(ShiftJisToUtf8("シーンオブジェクト").c_str());

	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->ImGuiCall();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->ImGuiCall();
	}

	ImGui::Dummy(ImVec2(0, 10));
	constexpr u_int buf = 256;

	// 生成
	static char createObjName[buf];
	ImGui::InputText("createName", createObjName, buf);
	if (ImGui::Button("new Object") && createObjName[0] != '\0')
		SceneFunction::ObjectFunc::CreateEmpty(createObjName);

	// 削除
	static char deleteObjName[buf];
	ImGui::InputText("deleteName", deleteObjName, buf);
	if (ImGui::Button("delete"))
		SceneFunction::ObjectFunc::DeleteObject(deleteObjName);

	ImGui::End();
#endif // EDIT
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

	HASHI_DEBUG_LOG(_deleteObj.GetName() + "はシーン内にありません");
}

GameObject* SceneObjects::GetSceneObject(const std::string& _objectName)
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
			std::string message = _objectName + "リスト内に名前のオブジェクトがありませんでした";
			HASHI_DEBUG_LOG(message);
			return nullptr;
		}
	}

	return itr->second.get();
}

nlohmann::json SceneObjects::SaveObject()
{
	nlohmann::json objectData;

	for (auto& obj : objList)
	{
		objectData.push_back(obj.second->Save());
	}


	for (auto& ui : uiList)
	{
		objectData.push_back(ui.second->Save());
	}

	return objectData;
}

void SceneObjects::LoadObject(const nlohmann::json& _objectsData)
{
	// オブジェクトを作成する
	for (const auto& data : _objectsData)
	{
		std::string objectName;
		HashiTaku::LoadJsonString("name", objectName, data);
		GameObject& go = SceneFunction::ObjectFunc::CreateEmpty(objectName);
		go.Load(data);
	}

	// オブジェクトをロード
	for (auto& data : _objectsData)
	{
		std::string objectName;
		HashiTaku::LoadJsonString("name", objectName, data);
		GameObject* go = GetSceneObject(objectName);

		go->LateLode(data);
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

