#include "pch.h"
#include "GameObject.h"
// �R���|�[�l���g
#include "Component.h"
#include "CP_Collider.h"

// �V�X�e��
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

GameObject& GameObject::Copy(const GameObject& _other)
{
	// �����Ȃ珈�����Ȃ�
	if (this == &_other) return *this;

	// �p�����[�^���
	pTransform = std::make_unique<Transform>(*_other.pTransform);
	isActive = _other.isActive;
	name = _other.name;
	tag = _other.tag;
	layer = _other.layer;

	// �R���|�[�l���g���R�s�[
	for (auto& comp : _other.components)
	{
		CloneComponentBase* clone = dynamic_cast<CloneComponentBase*>(comp.get());

		if (clone)
			SetComponent(clone->Clone());
	}

	return *this;
}

void GameObject::UpdateDeltaTime()
{
	deltaTime = deltaTimeSpeed * MainApplication::DeltaTime();
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
	auto itr = std::find_if(components.begin(), components.end(), [&](const std::unique_ptr<Component>& comp)
		{
			return comp.get() == &_pCheckComponent;
		});

	if (itr != components.end()) return true;

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

void GameObject::SortCompPriority()
{
	pActiveComponents.sort(SortCompPriorityFunc);
	pAwakeComponents.sort(SortCompPriorityFunc);
	pStartComponents.sort(SortCompPriorityFunc);
}

bool GameObject::SortCompPriorityFunc(const Component* _c1, const Component* _c2)
{
	return _c1->GetPriority() > _c2->GetPriority();
}

void GameObject::LoadCreateComponnet(const nlohmann::json& _componentsData)
{
	ComponentFactory* compFactory = ComponentFactory::GetInstance();

	for (auto& compData : _componentsData)
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

		pCreateComp->gameObject = this;

		Component& comp = *pCreateComp;
		// ���X�g�ɒǉ�
		components.push_back(std::move(pCreateComp));

		pActiveComponents.push_back(&comp);
		pAwakeComponents.push_back(&comp);
		pStartComponents.push_back(&comp);

		if (!comp.GetIsEnable())	// ������ԂłȂ��Ȃ�
		{
			RemoveActiveComponent(comp);
		}
	}
}

void GameObject::LoadComponentParameter(const nlohmann::json& _componentData)
{
	// ���������ƃ��[�h���s��
	for(auto& pComp : components)
	{
		pComp->Init();
		// �f�[�^������R���|�[�l���g�̃f�[�^��T���ă��[�h����
		for (auto& compData : _componentData)
		{
			std::string dataCompName;
			if (HashiTaku::LoadJsonString("name", dataCompName, compData))
			{
				bool isSame = pComp->GetName() == dataCompName;

				if (isSame)
				{
					pComp->Load(compData);
					break;
				}
			}
		}
	}

	// �D��x�ŕ��ׂ�
	SortCompPriority();
}

GameObject::GameObject() : isActive(true), name(""), deltaTimeSpeed(1.0f), deltaTime(0.0f)
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
	for (auto& pComp : components)
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

	// �o�ߎ��Ԃ��X�V����
	UpdateDeltaTime();

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

	// �����x�N�g�����X�V����
	pTransform->UpdateVector();

	for (auto& itr : pActiveComponents)
	{
		itr->DrawCall();
	}
}

void GameObject::OnCollisionEnter(const HashiTaku::CollisionInfo& _otherColInfo)
{
	for (auto& pActive : pActiveComponents)
		pActive->OnCollisionEnter(_otherColInfo);
}

void GameObject::OnCollisionStay(const HashiTaku::CollisionInfo& _otherColInfo)
{
	for (auto& pActive : pActiveComponents)
		pActive->OnCollisionStay(_otherColInfo);
}

void GameObject::OnCollisionExit(const HashiTaku::CollisionInfo& _otherColInfo)
{
	for (auto& pActive : pActiveComponents)
		pActive->OnCollisionExit(_otherColInfo);
}

void GameObject::Destroy()
{
	// �폜����q�g�����X�t�H�[�����X�g
	std::vector<Transform*> deleteChilds;
	for (u_int c_i = 0; c_i < pTransform->GetChildCnt(); c_i++)
	{
		deleteChilds.push_back(pTransform->GetChild(c_i));
	}

	// �q�g�����X�t�H�[�����폜
	for (u_int c_i = 0; c_i < static_cast<u_int>(deleteChilds.size()); c_i++)
	{
		deleteChilds[c_i]->GetGameObject().Destroy();
	}

	// �V�[���I�u�W�F�N�g���玩�g���폜����
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->
		GetSceneObjects();

	sceneObjects.DeleteGameObject(*this);
}

void GameObject::OnChangePosition()
{
	for (auto& pComp : pActiveComponents)
		pComp->OnChangePosition();
}

void GameObject::OnChangeScale()
{
	for (auto& pComp : pActiveComponents)
		pComp->OnChangeScale();
}

void GameObject::OnChangeRotation()
{
	for (auto& pComp : pActiveComponents)
		pComp->OnChangeRotation();
}

void GameObject::SetComponent(std::unique_ptr<Component> _pSetComponent)
{
	if (!_pSetComponent) return;

	// �R���|�[�l���g�t�@�N�g���[����擾
	_pSetComponent->gameObject = this;

	Component& comp = *_pSetComponent;

	// ���X�g�ɒǉ�
	components.push_back(std::move(_pSetComponent));

	pActiveComponents.push_back(&comp);
	pAwakeComponents.push_back(&comp);
	pStartComponents.push_back(&comp);


	// ��������
	comp.Init();
}

void GameObject::DeleteComponent(Component& _deleteComonent)
{
	// �폜���ꂽ�Ƃ��̏���
	_deleteComonent.OnDestroy();

	pActiveComponents.remove(&_deleteComonent);
	pAwakeComponents.remove(&_deleteComonent);
	pStartComponents.remove(&_deleteComonent);

	components.remove_if([&](std::unique_ptr<Component>& pComp)
		{
			return pComp.get() == &_deleteComonent;
		});
}

void GameObject::RemoveActiveComponent(Component& _removeComonent)
{
	// �A�N�e�B�u����O��
	pActiveComponents.remove(&_removeComonent);

	// Awake�������܂��Ȃ�
	if (_removeComonent.GetIsAlreadyAwake())
		pAwakeComponents.remove(&_removeComonent);

	// Start�������܂��Ȃ�
	if (_removeComonent.GetIsAlreadyStart())
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

	// �A�N�e�B�u�ɂȂ�������	// �A�N�e�B�u�ɂȂ�������
	if (!IsExistActiveComponent(_addComonent))
		pActiveComponents.push_back(&_addComonent);

	// Awake�ɂȂ�������
	if (!_addComonent.GetIsAlreadyAwake() && !IsExistAwakeComponent(_addComonent))
		pAwakeComponents.push_back(&_addComonent);

	// Start�ɂȂ�������
	if (!_addComonent.GetIsAlreadyStart() && !IsExistStartComponent(_addComonent))
		pStartComponents.push_back(&_addComonent);
}

void GameObject::OnActiveTrue()
{
	for (auto& pComp : components)
	{
		bool isEnable = pComp->GetIsEnable();

		if (isEnable)
			pComp->OnEnableTrueCall();
	}
}

void GameObject::OnActiveFalse()
{
	for (auto& pComp : components)
	{
		bool isEnable = pComp->GetIsEnable();

		if (isEnable)
			pComp->OnEnableFalseCall();
	}
}

void GameObject::ImGuiDebug()
{

	ImGuiMethod::PushItemSmallWidth();
	// �^�O�ݒ�
	HashiTaku::Tag::Type curTag = tag.GetType();
	if (HashiTaku::Tag::ImGuiComboBox(curTag))
		SetTag(curTag);

	ImGui::SameLine();

	// ���C���[�ݒ�
	HashiTaku::Layer::Type curLayer = layer.GetType();
	if (HashiTaku::Layer::ImGuiComboBox(curLayer))
		SetLayer(curLayer);

	ImGui::PopItemWidth();

	// �e�I�u�W�F�N�g��ݒ�
	ImGuiSetParent();

	bool changeActive = isActive;
	if (ImGui::Checkbox("isActive", &changeActive))
		SetActive(changeActive);

	Vector3 v = pTransform->GetLocalPosition();
	if (ImGuiMethod::DragFloat3(v, "pos", 0.1f))
		pTransform->SetLocalPosition(v);

	v = pTransform->GetLocalScale();
	if (ImGuiMethod::DragFloat3(v, "scale", 0.1f))
		pTransform->SetLocalScale(v);

	v = pTransform->GetLocalEularAngles();
	if (ImGuiMethod::DragFloat3(v, "rot"))
		pTransform->SetLocalEularAngles(v);


	bool isChangePriority = false;
	for (auto cpItr = components.begin(); cpItr != components.end();)
	{
		bool isDelete = false;	// �R���|�[�l���g�폜�t���O

		if (ImGuiMethod::TreeNode((*cpItr)->GetName().c_str()))
		{
			// �폜�{�^��
			isDelete = ImGui::Button("Delete");
			// �R���|�[�l���g���̕ҏW
			(*cpItr)->ImGuiSettingCall();

			// �D��x
			int pri = (*cpItr)->GetPriority();
			if (ImGui::DragInt("Priority", &pri))
			{
				isChangePriority = true;	// ���בւ���悤�ɂ���
				(*cpItr)->SetPriority(pri);
			}

			ImGui::TreePop();
		}

		if (isDelete)	// �폜����Ȃ�
		{
			const auto& nextItr = std::next(cpItr);
			DeleteComponent(*(*cpItr));
			cpItr = nextItr;
		}
		else
			cpItr++;
	}

	// �D��x���ɕ��ׂ�
	if(isChangePriority)
		SortCompPriority();	

	ImGui::Dummy(ImVec2(0, 10));

	// �R���|�[�l���g�ǉ�
	ComponentFactory* compFactory = ComponentFactory::GetInstance();
	compFactory->CreateImGuiCombo(*this);
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
	// �R���|�[�l���g�̃Z�[�u
	for (auto& comp : components)
	{
		componentData.push_back(comp->Save());
	}

	return objectData;
}


void GameObject::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;

	bool loadActive = true;
	HashiTaku::LoadJsonBoolean("active", loadActive, _data);
	SetActive(loadActive);

	HashiTaku::Tag::Type tagType;
	LoadJsonEnum<HashiTaku::Tag::Type>("tag", tagType, _data);
	SetTag(tagType);

	HashiTaku::Layer::Type layerType;
	LoadJsonEnum<HashiTaku::Layer::Type>("layer", layerType, _data);
	SetLayer(layerType);

	if (IsJsonContains(_data, "components"))
	{
		const auto& componentsData = _data["components"];
		LoadCreateComponnet(componentsData);
	}
	
}

void GameObject::LateLode(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	if (IsJsonContains(_data, "transform"))
	{
		pTransform->Load(_data["transform"]);
		pTransform->LoadChildTransform(_data["transform"]);
	}

	if (IsJsonContains(_data, "components"))
	{
		const auto& componentsData = _data["components"];
		LoadComponentParameter(componentsData);
	}
}

void GameObject::SetName(const std::string& _name)
{
	if (_name.empty()) return;	// ���O���Ȃ��Ȃ�
	name = _name;
}

void GameObject::SetActive(bool _isActive)
{
	if (isActive == _isActive) return;	// ������Ԃɕς��悤�Ƃ���Ȃ�I���
	isActive = _isActive;

	// ���ꂻ��ɕύX�������̏���
	if (isActive)
		OnActiveTrue();
	else
		OnActiveFalse();

	// �q�I�u�W�F�N�g���K�p����
	for (u_int c_i = 0; c_i < pTransform->GetChildCnt(); c_i++)
	{
		Transform* pChild = pTransform->GetChild(c_i);
		pChild->GetGameObject().SetActive(_isActive);
	}
}

void GameObject::SetTag(HashiTaku::Tag::Type _setType)
{
	tag.SetType(_setType);
}

void GameObject::SetLayer(HashiTaku::Layer::Type _setType)
{
	// �������C���[�Ȃ�I���
	if (layer.GetType() == _setType) return;

	// ���C���[��ύX
	layer.SetType(_setType);

	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();
	sceneObjects.MoveTmpList(*this);
}

void GameObject::SetDeltaTimeSpeed(float _deltaSpeed)
{
	deltaTimeSpeed = _deltaSpeed;
}

Transform& GameObject::GetTransform()
{
	return *pTransform;
}

const Transform& GameObject::GetConstTransform() const
{
	return *pTransform;
}

const std::string& GameObject::GetName() const
{
	return name;
}

float GameObject::DeltaTime() const
{
	return deltaTime;
}

float GameObject::GetDeltdaSpeed() const
{
	return deltaTimeSpeed;
}

bool GameObject::GetIsActive() const
{
	return isActive;
}

HashiTaku::Tag::Type GameObject::GetTag() const
{
	return tag.GetType();
}

HashiTaku::Layer::Type GameObject::GetLayer() const
{
	return layer.GetType();
}

