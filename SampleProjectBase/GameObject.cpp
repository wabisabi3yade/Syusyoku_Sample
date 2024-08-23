#include "pch.h"
#include "GameObject.h"
// �R���|�[�l���g
#include "Component.h"
#include "CP_Collider.h"

// �V�X�e��
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

void GameObject::ActiveProcess()
{
	CollisionChecker& collisionChecker = InSceneSystemManager::GetInstance()->
		GetCollisonChecker();

	for (auto& c : pComponents)
	{
		// �����蔻��`�F�b�N�N���X�ɒǉ�
		CP_Collider* col = dynamic_cast<CP_Collider*>(c.get());
		if (col != nullptr)
		{
			collisionChecker.AddCollider(*col);	// �ǉ�
		}
	}
}

void GameObject::NotActiveProcess()
{
	CollisionChecker& collisionChecker = InSceneSystemManager::GetInstance()->
		GetCollisonChecker();

	for (auto& c : pComponents)
	{
		// �����蔻��`�F�b�N�N���X����폜
		CP_Collider* col = dynamic_cast<CP_Collider*>(c.get());
		if (col != nullptr)
		{
			collisionChecker.PopCollider(*col);	// �폜
		}
	}
}

GameObject& GameObject::Copy(const GameObject& _other)
{
	// �����Ȃ珈�����Ȃ�
	if (this == &_other) return *this;

	// �p�����[�^���
	transform = _other.transform;
	isActive = _other.isActive;
	name = _other.name;
	tag = _other.tag;
	layer = _other.layer;

	// �R���|�[�l���g���R�s�[
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

	// �R���|�[�l���g�̃Z�[�u
	for (auto& compData : _componentData)
	{
		// ���O����R���|�[�l���g�Z�b�g
		std::string compName;
		HashiTaku::LoadJsonString("name", compName, compData);

		std::unique_ptr<Component> pCreateComp = compFactory->CreateByName(compName);

		if (!pCreateComp)
		{
			HASHI_DEBUG_LOG("�R���|�[�l���g������ɍ쐬�ł��܂���ł���");
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

	// �����x�N�g�����X�V����
	transform.UpdateVector();

	for (auto& itr : pActiveComponents)
	{
		itr->Draw();
	}
}

void GameObject::Destroy()
{
	// �V�[���I�u�W�F�N�g���玩�g���폜����
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->
		GetSceneObjects();
	sceneObjects.DeleteObj(*this);
}

void GameObject::SetComponent(std::unique_ptr<Component> _pSetComponent)
{
	if (!_pSetComponent) return;

	// �R���|�[�l���g�t�@�N�g���[����擾
	_pSetComponent->gameObject = this;

	Component& comp = *_pSetComponent;

	// ���X�g�ɒǉ�
	pComponents.push_back(std::move(_pSetComponent));
	pActiveComponents.push_back(&comp);
	pStartComponents.push_back(&comp);

	// ��������
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
	// �A�N�e�B�u����O��
	pActiveComponents.remove(&_removeComonent);

	// Start�������܂��Ȃ�X�^�[�g�z�񂩂���O��
	if (!_removeComonent.GetIsAlreadyStart())
		pStartComponents.remove(&_removeComonent);
}

void GameObject::AddActiveComponent(Component& _addComonent)
{
	// �������Ă��Ȃ��Ȃ�
	if (!IsExistComponent(_addComonent))
	{
		HASHI_DEBUG_LOG(_addComonent.name + ":�I�u�W�F�N�g�ɃR���|�[�l���g�������ł��Ă��܂���");
		return;
	}

	if (!IsExistActiveComponent(_addComonent))	// �A�N�e�B�u�ɂȂ�������
		pActiveComponents.push_back(&_addComonent);

	if (!IsExistStartComponent(_addComonent))	// Start�ɂȂ�������
		pStartComponents.push_back(&_addComonent);
}

void GameObject::ImGuiSet()
{
	if (ImGui::TreeNode(name.c_str()))	// ���OTree
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

		// �R���|�[�l���g�ǉ�
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
	// �R���|�[�l���g�̃Z�[�u
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
	if (isActive == _isActive) return;	// ������Ԃɕς��悤�Ƃ���Ȃ�I���

	if (_isActive)
		ActiveProcess();	// �A�N�e�B�u���̏���������
	else
		NotActiveProcess();	// ��A�N�e�B�u���̏���������
}
