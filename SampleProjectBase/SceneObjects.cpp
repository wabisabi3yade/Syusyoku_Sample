#include "pch.h"
#include "SceneObjects.h"
#include "SF_Define.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
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
		for (auto& obj : objList)
		{
			obj.second->AwakeCall();
		}

		for (auto& ui : uiList)
		{
			ui.second->AwakeCall();
		}
	}

	void SceneObjects::Start()
	{
		for (auto& obj : objList)
		{
			obj.second->StartCall();
		}

		for (auto& ui : uiList)
		{
			ui.second->StartCall();
		}
	}

	void SceneObjects::Update()
	{
		for (auto& obj : objList)
		{
			obj.second->UpdateCall();
		}

		for (auto& ui : uiList)
		{
			ui.second->UpdateCall();
		}
	}

	void SceneObjects::LateUpdate()
	{
		auto objItr = objList.begin();
		for (; objItr != objList.end();)
		{
			GameObject& go = *(*objItr).second;
			go.LateUpdateCall();

			++objItr;

			// リストから削除するのでここで行う
			if (go.isDestroy)
				go.Destroy();
		}

		auto uiItr = uiList.begin();
		for (; uiItr != uiList.end();)
		{
			GameObject& go = *(*uiItr).second;
			go.LateUpdateCall();

			++uiItr;

			// リストから削除するのでここで行う
			if (go.isDestroy)
				go.Destroy();
		}
	}

	void SceneObjects::ObjectDraw()
	{
		// 3D空間上のオブジェクト描画
		for (auto& obj : objList)
		{
			obj.second->DrawCall();
		}
	}

	void SceneObjects::UIDraw()
	{
		UIDrawSetup();

		// 2D空間（UI）のオブジェクト
		for (auto& ui : drawUiList)
		{
			ui->DrawCall();
		}

		// 透視投影に戻す
		UIDrawEnd();
	}

	void SceneObjects::MoveToUIList(GameObject& _gameObject)
	{
		// UIじゃないなら
		if (!IsUI(_gameObject)) return;

		// オブジェクトから取り出す
		std::string objectName;
		std::unique_ptr<GameObject> takeoutObj;
		if (!TakeOutObject(_gameObject, objectName, takeoutObj, objList)) return;

		// 描画UIリストに追加
		SetDrawUIList(*takeoutObj);

		// UIリストに追加する
		uiList[objectName] = std::move(takeoutObj);
	}

	void SceneObjects::MoveToObjList(GameObject& _gameObject)
	{
		// UIなら
		if (IsUI(_gameObject)) return;

		// オブジェクトから取り出す
		std::string objectName;
		std::unique_ptr<GameObject> takeoutObj;
		if (!TakeOutObject(_gameObject, objectName, takeoutObj, uiList)) return;

		RemoveDrawUIList(*takeoutObj);

		// UIリストに追加する
		objList[objectName] = std::move(takeoutObj);
	}

	void SceneObjects::SetDrawUIList(GameObject& _uiObj)
	{
		// UIオブジェクトか？
		if (_uiObj.GetLayer() != Layer::Type::UI) return;

		// リスト内にあるなら
		auto itr = std::find(drawUiList.begin(), drawUiList.end(), &_uiObj);
		if (itr != drawUiList.end()) return;

		drawUiList.push_back(&_uiObj);	// 追加

		SortUiList();	// z値順に並べる
	}

	void SceneObjects::RemoveDrawUIList(GameObject& _uiObj)
	{
		drawUiList.remove(&_uiObj);
	}

	std::string SceneObjects::PrefabFileParh()
	{
		return "assets/data/prefab/";
	}

	void SceneObjects::CreatePrefab(const std::string& _prefabName)
	{
		// プレハブのパス名
		std::string prefabPath = PrefabFileParh();
		prefabPath += _prefabName + ".json";

		// ファイルからjsonを読み込み
		std::ifstream f(prefabPath);
		if (!f.is_open())
		{
			HASHI_DEBUG_LOG(prefabPath + " プレハブファイルが開けませんでした");
			return;
		}

		const json prefabData = json::parse(f);
		// プレハブをロード
		LoadObject(prefabData);
		LateLoadObject(prefabData);
		HASHI_DEBUG_LOG(_prefabName + " 作成完了");
	}

	void SceneObjects::ImGuiDebug()
	{
		ImGui::Begin("SceneObjects");

		// オブジェクト
		for (auto& obj : objList)
		{
			// 親オブジェクトがあるなら
			if (obj.second->GetTransform().GetParent()) continue;

			bool isDelete = ImGuiSettingObject(*obj.second);

			if (isDelete) break;
		}

		// オブジェクト
		for (auto& ui : uiList)
		{
			// 親オブジェクトがないなら
			if (ui.second->GetTransform().GetParent()) continue;

			bool isDelete = ImGuiSettingObject(*ui.second);

			if (isDelete) break;
		}

		ImGui::Dummy(ImVec2(0, 10));

		// 生成
		static char createObjName[IM_INPUT_BUF];
		ImGui::InputText("createName", createObjName, IM_INPUT_BUF);
		if (ImGui::Button("new Object") && createObjName[0] != '\0')
			SceneFunction::ObjectFunc::CreateEmpty(createObjName);

		// プレハブ生成
		if (ImGui::Button("prefab") && createObjName[0] != '\0')
			CreatePrefab(createObjName);

		ImGui::End();

		// リスト間移動をする
		MoveList();
	}

	json SceneObjects::SaveObject(GameObject& _go)
	{
		json objData;
		objData["gameObject"] = _go.Save();

		Transform& transform = _go.GetTransform();

		// 子オブジェクトをセーブ
		auto& childData = objData["childs"];
		u_int childCnt = transform.GetChildCnt();
		for (u_int c_i = 0; c_i < childCnt; c_i++)
		{
			// 子オブジェクトをセーブする
			GameObject& childGameObj = transform.GetChild(c_i)->GetGameObject();
			childData.push_back(SaveObject(childGameObj));
		}

		return objData;
	}

	void SceneObjects::ObjectToPrefab(GameObject& _toPrefabObject)
	{
		// シーンデータ
		json prefabData;

		// シーン内オブジェクトをセーブ
		prefabData = SaveObject(_toPrefabObject);

		// ファイル名を設定
		std::string savePathStr;
		savePathStr = PrefabFileParh();	// パス
		savePathStr += _toPrefabObject.GetName();	// ファイル名
		savePathStr += ".json";	// 拡張子PrefabFileParh()

		std::ofstream f(savePathStr);
		auto str = prefabData.dump(4);
		auto len = str.length();
		f.write(str.c_str(), len);

		HASHI_DEBUG_LOG(_toPrefabObject.GetName() + "のプレハブ化に成功しました");
	}

	bool SceneObjects::LoadObject(const json& _gameObjectData)
	{
		// オブジェクトを読み込む
		json objData;
		if (!LoadJsonData("gameObject", objData, _gameObjectData))
		{
			HASHI_DEBUG_LOG("オブジェクトが読み込めませんでした");
			return false;
		}

		// 名前からゲームオブジェクトを作成
		std::string objectName;
		LoadJsonString("name", objectName, objData);
		GameObject& go = SceneFunction::ObjectFunc::CreateEmpty(objectName);
		go.Load(objData);

		// UIならUIリスト
		if (IsUI(go))
			MoveToUIList(go);


		// 子オブジェクトを読み込む
		json childDatas;
		if (!LoadJsonDataArray("childs", childDatas, _gameObjectData))
		{
			return false;
		}

		// 再帰的に読み込む
		for (auto& childData : childDatas)
			LoadObject(childData);

		return true;
	}

	bool SceneObjects::LateLoadObject(const json& _gameObjectData)
	{
		// オブジェクトを読み込む
		json objData;
		if (!LoadJsonData("gameObject", objData, _gameObjectData))
		{
			HASHI_DEBUG_LOG("オブジェクトが読み込めませんでした");
			return false;
		}

		std::string objectName;
		LoadJsonString("name", objectName, objData);
		GameObject* go = GetSceneObject(objectName);

		go->LateLode(objData);

		// 子オブジェクトを読み込む
		json childDatas;
		if (!LoadJsonDataArray("childs", childDatas, _gameObjectData))
		{
			return false;
		}

		// 再帰的に読み込む
		for (auto& childData : childDatas)
			LateLoadObject(childData);

		return true;
	}

	bool SceneObjects::ImGuiSettingObject(GameObject& _gameObject)
	{
#ifdef  EDIT

		bool isDelete = false;

		const std::string& objName = _gameObject.GetName();
		if (ImGuiMethod::TreeNode(objName))
		{
			// 削除
			isDelete = ImGui::Button("Delete");

			ImGui::SameLine();

			// プレハブを作成
			if (ImGui::Button("Prefab"))
				ObjectToPrefab(_gameObject);

			_gameObject.ImGuiCall();

			ImGui::TreePop();
		}

		if (isDelete)
		{
			_gameObject.Destroy();
			return true;
		}

		// 子オブジェクトを表示
		Transform& t = _gameObject.GetTransform();
		ImGui::Indent();
		for (u_int c_i = 0; c_i < t.GetChildCnt(); c_i++)
		{
			isDelete = ImGuiSettingObject(t.GetChild(c_i)->GetGameObject());

			if (isDelete)
				return true;
		}
		ImGui::Unindent();

#endif //  EDIT
		return false;
	}

	bool SceneObjects::SortUIPosZFunction(const GameObject* _a1, const GameObject* _a2)
	{
		return _a1->GetConstTransform().GetPosition().z > _a2->GetConstTransform().GetPosition().z;
	}

	GameObject* SceneObjects::SetObject(std::unique_ptr<GameObject> _objPtr)
	{
		// 名前が空か確認
		CheckEmptyName(*_objPtr.get());

		// 名前が重複しているか確認
		CheckDuplicationName(*_objPtr.get(), objList);
		CheckDuplicationName(*_objPtr.get(), uiList);

		// セットするリスト（オブジェクト側かUIか）
		SceneObjectList* setList = &objList;

		// UIなら
		if (IsUI(*_objPtr.get()))
			setList = &uiList;	// UIリストにセットする

		GameObject* retPtr = _objPtr.get();

		// 配列に入れる
		std::string setName = _objPtr->GetName();
		setList->insert(std::pair<std::string, std::unique_ptr<GameObject>>(setName, std::move(_objPtr)));

		return retPtr;
	}

	void SceneObjects::DeleteGameObject(GameObject& _deleteObj)
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
			RemoveDrawUIList(*(*itr).second);
			uiList.erase(itr);	// 削除する
			return;
		}

		HASHI_DEBUG_LOG(_deleteObj.GetName() + "はシーン内にありません");
	}

	void SceneObjects::MoveTmpList(GameObject& _targetObj)
	{
		// 移動配列に追加する
		tmpMoveList.push_back(&_targetObj);
	}

	void SceneObjects::SortUiList()
	{
		// ソートする
		drawUiList.sort(SortUIPosZFunction);
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

	json SceneObjects::SaveObjectList()
	{
		json objectData;

		for (auto& obj : objList)
		{
			Transform& transform = obj.second->GetTransform();

			// 親オブジェクトがあるなら
			if (transform.GetParent()) continue;
			objectData.push_back(SaveObject(*obj.second));	// セーブ
		}


		for (auto& ui : uiList)
		{
			Transform& transform = ui.second->GetTransform();

			// 親オブジェクトがあるなら
			if (transform.GetParent()) continue;
			objectData.push_back(SaveObject(*ui.second));	// セーブ
		}

		return objectData;
	}

	void SceneObjects::LoadObjectList(const json& _objectsData)
	{
		// オブジェクトを作成する
		for (const auto& data : _objectsData)
		{
			// オブジェクトをロード
			LoadObject(data);
		}

		// オブジェクトをロード
		for (auto& data : _objectsData)
		{
			LateLoadObject(data);
		}
	}

	void SceneObjects::CheckEmptyName(GameObject& _gameObject)
	{
		if (_gameObject.GetName() != "") return;

		_gameObject.SetName("Empty");
	}

	void SceneObjects::CheckDuplicationName(GameObject& _gameObject, SceneObjectList& _objects)
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

	bool SceneObjects::TakeOutObject(const GameObject& _targetObject, std::string& _objectName, std::unique_ptr<GameObject>& _outObject, SceneObjectList& _objectList)
	{
		// 同じアドレスを探索していく
		for (auto itr = _objectList.begin(); itr != _objectList.end(); itr++)
		{
			if (&_targetObject == itr->second.get())
			{
				// アドレスと名前を渡す
				_objectName = itr->first;
				_outObject = std::move(itr->second);

				// 削除する
				_objectList.erase(itr);

				return true;
			}
		}

		return false;
	}

	bool SceneObjects::IsUI(GameObject& _gameObject)
	{
		using namespace HashiTaku;

		Layer::Type layer = _gameObject.GetLayer();
		return layer == Layer::Type::UI ? true : false;
	}

	void SceneObjects::UIDrawSetup()
	{
		D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
		CP_Camera& camera = InSceneSystemManager::GetInstance()->GetMainCamera();

		// UI描画のため平行投影に切り替え
		camera.SetOrthographic();

		// 深度バッファをリセットする
		renderer.GetDeviceContext()->ClearDepthStencilView(
			renderer.GetDepthStencil(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,
			0
		);
	}

	void SceneObjects::UIDrawEnd()
	{
		CP_Camera& camera = InSceneSystemManager::GetInstance()->GetMainCamera();
		camera.SetPerspective();
	}

	void SceneObjects::MoveList()
	{
		if (tmpMoveList.empty()) return;

		// 一時移動配列に入っているオブジェクトを移動させる
		for (auto& pObj : tmpMoveList)
		{
			if (pObj->GetLayer() == Layer::Type::UI)
				MoveToUIList(*pObj);
			else
				MoveToObjList(*pObj);
		}

		tmpMoveList.clear();
	}
}