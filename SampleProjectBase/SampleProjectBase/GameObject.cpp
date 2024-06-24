#include "pch.h"
#include "GameObject.h"

#include "Component.h"
#include "Collider.h"
#include "CollisionRegister.h"
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

		if (!c->GetIsStartYet())
		{

		}
	}
}
void GameObject::Update()
{
	if (!isActive) return;

	transform.UpdateVector();	// �����x�N�g�����X�V����

	for (auto& itr : pComponents)
	{
		itr->Update();
	}
}

void GameObject::LateUpdate()
{
	if (!isActive) return;

	for (auto& itr : pComponents)
	{
		itr->LateUpdate();
	}
}

void GameObject::Draw()
{
	if (!isActive) return;

	for (auto& itr : pComponents)
	{
		itr->Draw();
	}
}

void GameObject::ImGuiSet()
{
#ifdef _DEBUG
	if (ImGui::TreeNode(name.c_str()))	// ���OTree
	{
		ImGui::Checkbox("isActive", &isActive);
		ImGuiMethod::DragFloat3(transform.position, "pos");
		ImGuiMethod::DragFloat3(transform.rotation, "rot");
		ImGuiMethod::DragFloat3(transform.scale, "scale");

		for (auto& itr : pComponents)
		{
			itr->SetParameter();
		}

		ImGui::TreePop();
	}
#endif // _DEBUG
}

void GameObject::SetActive(bool _isActive)
{
	if (isActive == _isActive) return;	// ������Ԃɕς��悤�Ƃ���Ȃ�I���

	if (_isActive)
		ActiveProcess();	// �A�N�e�B�u���̏���������
	else
		NotActiveProcess();	// ��A�N�e�B�u���̏���������
}
