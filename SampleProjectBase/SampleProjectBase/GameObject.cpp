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

		// Start関数がまだなら
		if (!c->GetIsStartYet())
		{

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

	// 方向ベクトルを更新する
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
	// シーンオブジェクトから自身を削除する
	SObjectRegister::GetInstance()->PopObject(*this);
}

void GameObject::ImGuiSet()
{
#ifdef EDIT
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
	if (isActive == _isActive) return;	// 同じ状態に変えようとするなら終わる

	if (_isActive)
		ActiveProcess();	// アクティブ時の処理をする
	else
		NotActiveProcess();	// 非アクティブ時の処理をする
}
