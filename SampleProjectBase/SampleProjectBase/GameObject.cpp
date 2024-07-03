#include "pch.h"
#include "GameObject.h"
// �R���|�[�l���g
#include "Component.h"
#include "Collider.h"
// �o�^
#include "CollisionRegister.h"
#include "SObjectRegister.h"

using json = nlohmann::json;

void GameObject::ActiveProcess()
{
	CollisionRegister* colRegister = CollisionRegister::GetInstance();
	for (auto& c : pComponents)
	{
		// �����蔻��`�F�b�N�N���X�ɒǉ�
		Collider* col = dynamic_cast<Collider*>(c.get());
		if (col != nullptr)
		{
			colRegister->AddCollider(*col);	// �ǉ�
		}

		if (!c->GetIsStartYet())
		{

		}

	}
}

void GameObject::NotActiveProcess()
{
	CollisionRegister* colRegister = CollisionRegister::GetInstance();
	for (auto& c : pComponents)
	{
		// �����蔻��`�F�b�N�N���X����폜
		Collider* col = dynamic_cast<Collider*>(c.get());
		if (col != nullptr)
		{
			colRegister->PopCollider(*col);	// �폜
		}

		// Start�֐����܂��Ȃ�
		if (!c->GetIsStartYet())
		{

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

	Update();

	for (auto& itr : pComponents)
	{
		if (!itr->isEnable) continue;
		itr->Update();
	}
}

void GameObject::LateUpdate()
{
	if (!isActive) return;

	for (auto& itr : pComponents)
	{
		if (!itr->isEnable) continue;
		itr->LateUpdate();
	}
}

void GameObject::DrawBase()
{
	if (!isActive) return;

	// �����x�N�g�����X�V����
	transform.UpdateVector();

	Draw();

	for (auto& itr : pComponents)
	{
		if (!itr->isEnable) continue;
		itr->Draw();
	}
}

void GameObject::Destroy()
{
	// �V�[���I�u�W�F�N�g���玩�g���폜����
	SObjectRegister::GetInstance()->PopObject(*this);
}

void GameObject::ImGuiSet()
{
#ifdef EDIT
	if (ImGui::TreeNode(name.c_str()))	// ���OTree
	{
		ImGui::Checkbox("isActive", &isActive);
		ImGuiMethod::DragFloat3(transform.position, "pos");
		ImGuiMethod::DragFloat3(transform.rotation, "rot");
		ImGuiMethod::DragFloat3(transform.scale, "scale");

		for (auto& itr : pComponents)
		{
			if (ImGui::TreeNode(itr->name.c_str()))
			{
				ImGui::Checkbox("isEnabled", &itr->isEnable);
				itr->ImGuiSetting();
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
#endif // EDIT
}


void GameObject::SetActive(bool _isActive)
{
	if (isActive == _isActive) return;	// ������Ԃɕς��悤�Ƃ���Ȃ�I���

	if (_isActive)
		ActiveProcess();	// �A�N�e�B�u���̏���������
	else
		NotActiveProcess();	// ��A�N�e�B�u���̏���������
}
