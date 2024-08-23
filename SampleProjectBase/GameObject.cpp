#include "pch.h"
#include "GameObject.h"
// コンポーネント
#include "Component.h"
#include "CP_Collider.h"

// システム
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

void GameObject::ActiveProcess()
{
	CollisionChecker& collisionChecker = InSceneSystemManager::GetInstance()->
		GetCollisonChecker();

	for (auto& c : pComponents)
	{
		// 当たり判定チェッククラスに追加
		CP_Collider* col = dynamic_cast<CP_Collider*>(c.get());
		if (col != nullptr)
		{
			collisionChecker.AddCollider(*col);	// 追加
		}
	}
}

void GameObject::NotActiveProcess()
{
	CollisionChecker& collisionChecker = InSceneSystemManager::GetInstance()->
		GetCollisonChecker();

	for (auto& c : pComponents)
	{
		// 当たり判定チェッククラスから削除
		CP_Collider* col = dynamic_cast<CP_Collider*>(c.get());
		if (col != nullptr)
		{
			collisionChecker.PopCollider(*col);	// 削除
		}
	}
}

GameObject& GameObject::Copy(const GameObject& _other)
{
	// 同じなら処理しない
	if (this == &_other) return *this;

	// パラメータ代入
	transform = _other.transform;
	isActive = _other.isActive;
	name = _other.name;
	tag = _other.tag;
	layer = _other.layer;

	// コンポーネントをコピー
	std::list<std::unique_ptr<Component>> pComponents;

	return *this;
}

bool GameObject::IsExistComponent(const Component& _pCheckComponent)
{
	auto itr = std::find_if(pComponents.begin(), pComponents.end(), [&](const std::unique_ptr<Component>& comp)
		{
			return comp.get() == &_pCheckComponent;
		});

	if (itr != pComponents.end()) return true;

	return false;
}

bool GameObject::IsExistActiveComponent(const Component& _pCheckComponent)
{
	auto itr = std::find_if(pActiveComponents.begin(), pActiveComponents.end(), [&](Component* comp)
		{
			return comp == &_pCheckComponent;
		});

	if (itr != pActiveComponents.end()) return true;

	return false;
}

bool GameObject::IsExistStartComponent(const Component& _pCheckComponent)
{
	auto itr = std::find_if(pStartComponents.begin(), pStartComponents.end(), [&](Component* comp)
		{
			return comp == &_pCheckComponent;
		});

	if (itr != pStartComponents.end()) return true;

	return false;
}

void GameObject::LoadComponent(const nlohmann::json& _componentData)
{
	ComponentFactory* compFactory = ComponentFactory::GetInstance();

	// コンポーネントのセーブ
	for (auto& compData : _componentData)
	{
		// 名前からコンポーネントセット
		std::string compName;
		HashiTaku::LoadJsonString("name", compName, compData);

		std::unique_ptr<Component> pCreateComp = compFactory->CreateByName(compName);

		if (!pCreateComp)
		{
			HASHI_DEBUG_LOG("コンポーネントが正常に作成できませんでした");
			continue;
		}

		Component& comp = *pCreateComp;
		SetComponent(std::move(pCreateComp));

		comp.Load(compData);
	}
}

GameObject::GameObject() : isActive(true), name("")
{
}

GameObject::GameObject(const GameObject& _other)
{
	Copy(_other);
}

GameObject& GameObject::operator=(const GameObject& _other)
{
	return Copy(_other);
}

void GameObject::UpdateBase()
{
	if (!isActive) return;

	// Start
	for (auto& comp : pStartComponents)
	{
		comp->StartBase();
	}
	pStartComponents.clear();

	// Update
	for (auto& comp : pActiveComponents)
	{
		comp->Update();
	}
}

void GameObject::LateUpdateBase()
{
	if (!isActive) return;

	for (auto& itr : pActiveComponents)
	{
		itr->LateUpdate();
	}
}

void GameObject::DrawBase()
{
	if (!isActive) return;

	// 方向ベクトルを更新する
	transform.UpdateVector();

	for (auto& itr : pActiveComponents)
	{
		itr->Draw();
	}
}

void GameObject::Destroy()
{
	// シーンオブジェクトから自身を削除する
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->
		GetSceneObjects();
	sceneObjects.DeleteObj(*this);
}

void GameObject::SetComponent(std::unique_ptr<Component> _pSetComponent)
{
	if (!_pSetComponent) return;

	// コンポーネントファクトリーから取得
	_pSetComponent->gameObject = this;

	Component& comp = *_pSetComponent;

	// リストに追加
	pComponents.push_back(std::move(_pSetComponent));
	pActiveComponents.push_back(&comp);
	pStartComponents.push_back(&comp);

	// 初期処理
	comp.Awake();
}

void GameObject::DeleteComponent(Component& _deleteComonent)
{
	pComponents.remove_if([&](std::unique_ptr<Component>& pComp)
		{
			return pComp.get() == &_deleteComonent;
		});

	pActiveComponents.remove(&_deleteComonent);
	pStartComponents.remove(&_deleteComonent);
}

void GameObject::RemoveActiveComponent(Component& _removeComonent)
{
	// アクティブから外す
	pActiveComponents.remove(&_removeComonent);

	// Start処理がまだならスタート配列からも外す
	if (!_removeComonent.GetIsAlreadyStart())
		pStartComponents.remove(&_removeComonent);
}

void GameObject::AddActiveComponent(Component& _addComonent)
{
	// 所持していないなら
	if (!IsExistComponent(_addComonent))
	{
		HASHI_DEBUG_LOG(_addComonent.name + ":オブジェクトにコンポーネントを所持できていません");
		return;
	}

	if (!IsExistActiveComponent(_addComonent))	// アクティブになかったら
		pActiveComponents.push_back(&_addComonent);

	if (!IsExistStartComponent(_addComonent))	// Startになかったら
		pStartComponents.push_back(&_addComonent);
}

void GameObject::ImGuiSet()
{
	if (ImGui::TreeNode(name.c_str()))	// 名前Tree
	{
		ImGui::Checkbox("isActive", &isActive);

		Vector3 v = transform.GetLocalPosition();
		ImGuiMethod::DragFloat3(v, "pos", 0.1f);
		transform.SetLocalPos(v);

		v = transform.GetLocalScale();
		ImGuiMethod::DragFloat3(v, "scale", 0.1f);
		transform.SetLocalScale(v);

	/*	v = transform.GetLocalEularAngles();
		ImGuiMethod::DragFloat3(v, "rot");
		transform.SetLocalEularAngles(v);*/

		std::list<Component*> deleteComponents;

		for (auto itr = pComponents.begin(); itr != pComponents.end(); itr++)
		{
			if (!ImGui::TreeNode((*itr)->GetName().c_str())) continue;

			std::string text = "isEnable ";
			std::string status = "true";
			if (!(*itr)->GetIsEnable())
				status = "false";

			text += status;

			ImGui::Text(text.c_str());
			ImGui::SameLine();

			if (ImGui::Button("Change"))
				(*itr)->TransitionEnable();

			if (ImGui::Button("Delete"))
				deleteComponents.push_back(itr->get());

			(*itr)->ImGuiSetting();
			ImGui::TreePop();
		}

		for (auto itr = deleteComponents.begin(); itr != deleteComponents.end(); itr++)
		{
			DeleteComponent(*(*itr));
		}

		ImGui::Dummy(ImVec2(0, 10));

		// コンポーネント追加
		ComponentFactory* compFactory = ComponentFactory::GetInstance();
		compFactory->CreateImGuiCombo(*this);

		ImGui::TreePop();
	}
}

nlohmann::json GameObject::Save()
{
	nlohmann::json objectData;

	objectData["active"] = isActive;
	objectData["name"] = name;
	objectData["tag"] = tag.GetType();
	objectData["layer"] = layer.GetType();
	objectData["transform"] = transform.Save();

	auto& componentData = objectData["components"];
	// コンポーネントのセーブ
	for (auto& comp : pComponents)
	{
		componentData.push_back(comp->Save());
	}

	return objectData;
}


void GameObject::Load(const nlohmann::json& _data)
{
	LoadJsonBoolean("active", isActive, _data);

	Tag::Type tagType;
	LoadJsonEnum<Tag::Type>("tag", tagType, _data);
	tag.SetType(tagType);

	Layer::Type layerType;
	LoadJsonEnum<Layer::Type>("layer", layerType, _data);
	layer.SetType(layerType);

	if (IsJsonContains(_data, "transform"))
		transform.Load(_data["transform"]);

	if (IsJsonContains(_data, "components"))
	{
		const auto& componentsData = _data["components"];
		LoadComponent(componentsData);
	}
}

void GameObject::SetName(const std::string& _name)
{
	if (_name == "") return;
	name = _name;
}

void GameObject::SetActive(bool _isActive)
{
	if (isActive == _isActive) return;	// 同じ状態に変えようとするなら終わる

	if (_isActive)
		ActiveProcess();	// アクティブ時の処理をする
	else
		NotActiveProcess();	// 非アクティブ時の処理をする
}
