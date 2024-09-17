#include "pch.h"
#include "GameObject.h"
// コンポーネント
#include "Component.h"
#include "CP_Collider.h"

// システム
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

GameObject& GameObject::Copy(const GameObject& _other)
{
	// 同じなら処理しない
	if (this == &_other) return *this;

	// パラメータ代入
	pTransform = std::make_unique<Transform>(*_other.pTransform);
	isActive = _other.isActive;
	name = _other.name;
	tag = _other.tag;
	layer = _other.layer;

	// コンポーネントをコピー
	for (auto& comp : _other.pComponents)
	{
		CloneComponentBase* clone = dynamic_cast<CloneComponentBase*>(comp.get());

		if (clone)
			SetComponent(clone->Clone());
	}

	return *this;
}

void GameObject::ImGuiSetParent()
{
	const u_int buf = 256;
	static char str[buf] = "\0";

	ImGui::InputText("parent", str, buf);
	if (ImGui::Button("Set Parent"))
	{
		SceneObjects& sObj = InSceneSystemManager::GetInstance()->GetSceneObjects();
		GameObject* parentObj = sObj.GetSceneObject(str);

		if (!parentObj) return;
		pTransform->SetParent(parentObj->GetTransform());
	}
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

bool GameObject::IsExistAwakeComponent(const Component& _pCheckComponent)
{
	auto itr = std::find_if(pAwakeComponents.begin(), pAwakeComponents.end(), [&](Component* comp)
		{
			return comp == &_pCheckComponent;
		});

	if (itr != pAwakeComponents.end()) return true;

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

		if (!comp.GetIsEnable())	// 活動状態でないなら
		{
			RemoveActiveComponent(comp);
		}
	}
}

GameObject::GameObject() : isActive(true), name("")
{
	pTransform = std::make_unique<Transform>(this);
}

GameObject::GameObject(const GameObject& _other)
{
	Copy(_other);
}

GameObject& GameObject::operator=(const GameObject& _other)
{
	return Copy(_other);
}

GameObject::~GameObject()
{
	for (auto& pComp : pComponents)
		pComp->OnDestroy();
}

void GameObject::AwakeCall()
{
	if (!isActive) return;

	// Awake
	for (auto& comp : pAwakeComponents)
	{
		comp->AwakeCall();
	}
	pAwakeComponents.clear();
}

void GameObject::StartCall()
{
	if (!isActive) return;

	// Start
	for (auto& comp : pStartComponents)
	{
		comp->StartCall();
	}
	pStartComponents.clear();
}

void GameObject::UpdateCall()
{
	if (!isActive) return;

	// Update
	for (auto& comp : pActiveComponents)
	{
		comp->UpdateCall();
	}
}

void GameObject::LateUpdateCall()
{
	if (!isActive) return;

	for (auto& itr : pActiveComponents)
	{
		itr->LateUpdateCall();
	}
}

void GameObject::DrawCall()
{
	if (!isActive) return;

	// 方向ベクトルを更新する
	pTransform->UpdateVector();

	for (auto& itr : pActiveComponents)
	{
		itr->DrawCall();
	}
}

void GameObject::Destroy()
{
	// 削除する子トランスフォームリスト
	std::vector<Transform*> deleteChilds;
	for (u_int c_i = 0; c_i < pTransform->GetChildCnt(); c_i++)
	{
		deleteChilds.push_back(pTransform->GetChild(c_i));
	}

	// 子トランスフォームも削除
	for (u_int c_i = 0; c_i < static_cast<u_int>(deleteChilds.size()); c_i++)
	{
		deleteChilds[c_i]->GetGameObject().Destroy();
	}

	// シーンオブジェクトから自身を削除する
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->
		GetSceneObjects();

	sceneObjects.DeleteObj(*this);
}

void GameObject::OnChangeTransform()
{
	for (auto& pComp : pComponents)
		pComp->OnChangeTransform();
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
	pAwakeComponents.push_back(&comp);
	pStartComponents.push_back(&comp);


	// 初期処理
	comp.Init();
}

void GameObject::DeleteComponent(Component& _deleteComonent)
{
	// 削除されたときの処理
	_deleteComonent.OnDestroy();

	pActiveComponents.remove(&_deleteComonent);
	pAwakeComponents.remove(&_deleteComonent);
	pStartComponents.remove(&_deleteComonent);

	pComponents.remove_if([&](std::unique_ptr<Component>& pComp)
		{
			return pComp.get() == &_deleteComonent;
		});
}

void GameObject::RemoveActiveComponent(Component& _removeComonent)
{
	// アクティブから外す
	pActiveComponents.remove(&_removeComonent);

	// Awake処理がまだなら
	if (_removeComonent.GetIsAlreadyAwake())
		pAwakeComponents.remove(&_removeComonent);

	// Start処理がまだなら
	if (_removeComonent.GetIsAlreadyStart())
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

	// アクティブになかったら	// アクティブになかったら
	if (!IsExistActiveComponent(_addComonent))
		pActiveComponents.push_back(&_addComonent);

	// Awakeになかったら
	if (!_addComonent.GetIsAlreadyAwake() && !IsExistAwakeComponent(_addComonent))
		pAwakeComponents.push_back(&_addComonent);

	// Startになかったら
	if (!_addComonent.GetIsAlreadyStart() && !IsExistStartComponent(_addComonent))
		pStartComponents.push_back(&_addComonent);
}

void GameObject::OnActiveTrue()
{
	for (auto& pComp : pComponents)
	{
		bool isEnable = pComp->GetIsEnable();

		if (isEnable)
			pComp->OnEnableTrueCall();
	}
}

void GameObject::OnActiveFalse()
{
	for (auto& pComp : pComponents)
	{
		bool isEnable = pComp->GetIsEnable();

		if (isEnable)
			pComp->OnEnableFalseCall();
	}
}

void GameObject::ImGuiSetting()
{
	if (ImGui::TreeNode(name.c_str()))	// 名前Tree
	{
		bool changeActive = isActive;
		if (ImGui::Checkbox("isActive", &changeActive))
			SetActive(changeActive);

		ImGuiSetParent();

		Vector3 v = pTransform->GetLocalPosition();
		if (ImGuiMethod::DragFloat3(v, "pos", 0.1f))
			pTransform->SetLocalPosition(v);

		v = pTransform->GetLocalScale();
		if (ImGuiMethod::DragFloat3(v, "scale", 0.1f))
			pTransform->SetLocalScale(v);

		v = pTransform->GetLocalEularAngles();
		if (ImGuiMethod::DragFloat3(v, "rot"))
			pTransform->SetLocalEularAngles(v);

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

			(*itr)->ImGuiCall();
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
	objectData["transform"] = pTransform->Save();

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
	bool loadActive = true;
	LoadJsonBoolean("active", loadActive, _data);
	SetActive(loadActive);

	Tag::Type tagType;
	LoadJsonEnum<Tag::Type>("tag", tagType, _data);
	tag.SetType(tagType);

	Layer::Type layerType;
	LoadJsonEnum<Layer::Type>("layer", layerType, _data);
	layer.SetType(layerType);

	if (IsJsonContains(_data, "transform"))
		pTransform->Load(_data["transform"]);
}

void GameObject::LateLode(const nlohmann::json& _data)
{
	if (IsJsonContains(_data, "transform"))
		pTransform->LoadChildTransform(_data["transform"]);

	if (IsJsonContains(_data, "components"))
	{
		const auto& componentsData = _data["components"];
		LoadComponent(componentsData);
	}
}

void GameObject::SetName(const std::string& _name)
{
	if (_name == "") return;	// 名前がないなら
	name = _name;
}

void GameObject::SetActive(bool _isActive)
{
	if (isActive == _isActive) return;	// 同じ状態に変えようとするなら終わる
	isActive = _isActive;

	// それそれに変更した時の処理
	if (isActive)
		OnActiveTrue();
	else
		OnActiveFalse();

	// 子オブジェクトも適用する
	for (u_int c_i = 0; c_i < pTransform->GetChildCnt(); c_i++)
	{
		Transform* pChild = pTransform->GetChild(c_i);
		pChild->GetGameObject().SetActive(_isActive);
	}
}

Transform& GameObject::GetTransform()
{
	return *pTransform;
}

