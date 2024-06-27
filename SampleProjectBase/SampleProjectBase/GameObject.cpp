#include "pch.h"
#include "GameObject.h"
// コンポーネント
#include "Component.h"
#include "Collider.h"
// 登録
#include "CollisionRegister.h"
#include "SObjectRegister.h"

using json = nlohmann::json;

void GameObject::ActiveProcess()
{
	CollisionRegister* colRegister = CollisionRegister::GetInstance();
	for (auto& c : pComponents)
	{
		// 当たり判定チェッククラスに追加
		Collider* col = dynamic_cast<Collider*>(c.get());
		if (col != nullptr)
		{
			colRegister->AddCollider(*col);	// 追加
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
		// 当たり判定チェッククラスから削除
		Collider* col = dynamic_cast<Collider*>(c.get());
		if (col != nullptr)
		{
			colRegister->PopCollider(*col);	// 削除
		}

		if (!c->GetIsStartYet())
		{

		}
	}
}

GameObject::GameObject() : isActive(true), name("")
{
	/*saveValues = std::make_unique<SaveJsonValue>();*/
}

void GameObject::UpdateBase()
{
	if (!isActive) return;

	transform.UpdateVector();	// 方向ベクトルを更新する

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

void GameObject::Draw()
{
	if (!isActive) return;

	for (auto& itr : pComponents)
	{
		if (!itr->isEnable) continue;
		itr->Draw();
	}
}

void GameObject::Destroy()
{
	// シーンオブジェクトから自身を削除する
	SObjectRegister::GetInstance()->PopObject(*this);
}

void GameObject::ImGuiSet()
{
#ifdef _DEBUG
	if (ImGui::TreeNode(name.c_str()))	// 名前Tree
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
				itr->SetParameter();
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
#endif // _DEBUG
}


void GameObject::SetActive(bool _isActive)
{
	if (isActive == _isActive) return;	// 同じ状態に変えようとするなら終わる

	if (_isActive)
		ActiveProcess();	// アクティブ時の処理をする
	else
		NotActiveProcess();	// 非アクティブ時の処理をする
}
