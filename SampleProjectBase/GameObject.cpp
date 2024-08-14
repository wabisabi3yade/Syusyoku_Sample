#include "pch.h"
#include "GameObject.h"
// �R���|�[�l���g
#include "Component.h"
#include "CP_Collider.h"

// �V�X�e��
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

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

GameObject::GameObject() : isActive(true), name("")
{
	/*saveValues = std::make_unique<SaveJsonValue>();*/
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

	for (auto& itr : pActiveComponents)
	{
		itr->Update();
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

void GameObject::RemoveActiveComponent(Component& _removeComonent)
{
	pActiveComponents.remove(&_removeComonent);
}

void GameObject::AddActiveComponent(Component& _addComonent)
{
	// �������Ă��Ȃ��Ȃ�
	if (!IsExistComponent(_addComonent))
	{
		HASHI_DEBUG_LOG(_addComonent.name + ":�I�u�W�F�N�g�ɃR���|�[�l���g�������ł��Ă��܂���");
		return;
	}

	// �A�N�e�B�u�z��ɂ���Ȃ�
	if (IsExistActiveComponent(_addComonent))
	{
		HASHI_DEBUG_LOG(_addComonent.name + ":���ɃA�N�e�B�u��Ԃł�");
		return;
	}

	// �ǉ�
	pActiveComponents.push_back(&_addComonent);
}

void GameObject::ImGuiSet()
{
#ifdef EDIT
	if (ImGui::TreeNode(name.c_str()))	// ���OTree
	{
		ImGui::Checkbox("isActive", &isActive);
		ImGuiMethod::DragFloat3(transform.position, "pos");
		ImGuiMethod::DragFloat3(transform.scale, "scale");
		Vector3 angles = transform.GetEularAngles();
		ImGuiMethod::DragFloat3(angles, "rot");
		transform.SetEularAngles(angles);

		for (auto& itr : pComponents)
		{
			if (!ImGui::TreeNode(itr->name.c_str())) continue;

			std::string text = "isEnable:";
			std::string status = "true";
			if (!itr->GetIsEnable())
				status = "false";

			text += status;

			ImGui::Text(text.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Change"))
				itr->TransitionEnable();

			itr->ImGuiSetting();
			ImGui::TreePop();

		}

		ImGui::TreePop();
	}
#endif // EDIT
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
