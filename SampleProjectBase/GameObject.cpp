#include "pch.h"
#include "GameObject.h"
// コンポーネント
#include "Component.h"
#include "CP_Collider.h"

// システム
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	using namespace DXSimp;

	void GameObject::UpdateDeltaTime(float _sceneDeltaScale)
	{
		// シーンのΔtスケール ×　オブジェクトの速度　× Δt
		deltaTime = _sceneDeltaScale * deltaTimeSpeed * MainApplication::DeltaTime();
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

		if (ImGui::Button("Remove Parent"))
		{
			pTransform->RemoveParent();
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

	void GameObject::LoadCreateComponnet(const json& _componentsData)
	{
		ComponentFactory* compFactory = ComponentFactory::GetInstance();

		for (auto& compData : _componentsData)
		{
			// 名前からコンポーネントセット
			std::string compName;
			LoadJsonString("name", compName, compData);

			std::unique_ptr<Component> pCreateComp = compFactory->CreateByName(compName);

			if (!pCreateComp)
			{
				HASHI_DEBUG_LOG("コンポーネントが正常に作成できませんでした");
				continue;
			}

			pCreateComp->gameObject = this;

			Component& comp = *pCreateComp;
			// リストに追加
			components.push_back(std::move(pCreateComp));

			pActiveComponents.push_back(&comp);
			pAwakeComponents.push_back(&comp);
			pStartComponents.push_back(&comp);

			if (!comp.GetIsEnable())	// 活動状態でないなら
			{
				RemoveActiveComponent(comp);
			}
		}
	}

	void GameObject::LoadComponentParameter(const json& _componentData)
	{
		// 初期処理とロードを行う
		for (auto& pComp : components)
		{
			pComp->Init();
			// データ内からコンポーネントのデータを探してロードする
			for (auto& compData : _componentData)
			{
				std::string dataCompName;
				if (LoadJsonString("name", dataCompName, compData))
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

		// 優先度で並べる
		SortCompPriority();
	}

	GameObject::GameObject() :
		isActive(true),
		name(""),
		deltaTimeSpeed(1.0f),
		deltaTime(0.0f),
		isDestroy(false)
	{
		pTransform = std::make_unique<Transform>(this);
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

	void GameObject::UpdateCall(float _sceneDeltaScale)
	{
		if (!isActive) return;

		// 経過時間を更新する
		UpdateDeltaTime(_sceneDeltaScale);

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

		for (auto& itr : pActiveComponents)
		{
			itr->DrawCall();
		}
	}

	void GameObject::OnCollisionEnter(const CollisionInfo& _otherColInfo)
	{
		for (auto& pActive : pActiveComponents)
			pActive->OnCollisionEnter(_otherColInfo);
	}

	void GameObject::OnCollisionStay(const CollisionInfo& _otherColInfo)
	{
		for (auto& pActive : pActiveComponents)
			pActive->OnCollisionStay(_otherColInfo);
	}

	void GameObject::OnCollisionExit(const CollisionInfo& _otherColInfo)
	{
		for (auto& pActive : pActiveComponents)
			pActive->OnCollisionExit(_otherColInfo);
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

		// コンポーネントファクトリーから取得
		_pSetComponent->gameObject = this;

		Component& comp = *_pSetComponent;

		// リストに追加
		components.push_back(std::move(_pSetComponent));

		pActiveComponents.push_back(&comp);
		pAwakeComponents.push_back(&comp);
		pStartComponents.push_back(&comp);


		// 初期処理
		comp.Init();
	}

	void GameObject::SetDestroy()
	{
		isDestroy = true;
	}

	void GameObject::DeleteComponent(Component& _deleteComonent)
	{
		// 削除されたときの処理
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
		// アクティブから外す
		pActiveComponents.remove(&_removeComonent);

		//// Awake処理がまだなら
		//if (!_removeComonent.GetIsAlreadyAwake())
		//	pAwakeComponents.remove(&_removeComonent);

		//// Start処理がまだなら
		//if (!_removeComonent.GetIsAlreadyStart())
		//	pStartComponents.remove(&_removeComonent);
	}

	void GameObject::AddActiveComponent(Component& _addComonent)
	{
		// 所持していないなら
		if (!IsExistComponent(_addComonent))
		{
			HASHI_DEBUG_LOG(_addComonent.name + ":オブジェクトにコンポーネントを所持できていません");
			return;
		}

		// アクティブになかったら
		if (!IsExistActiveComponent(_addComonent))
			pActiveComponents.push_back(&_addComonent);

		//// Awakeになかったら
		//if (!_addComonent.GetIsAlreadyAwake() && !IsExistAwakeComponent(_addComonent))
		//	/*pAwakeComponents.push_back(&_addComonent);*/ _addComonent.AwakeCall();

		//// Startになかったら
		//if (!_addComonent.GetIsAlreadyStart() && !IsExistStartComponent(_addComonent))
		//	/*pStartComponents.push_back(&_addComonent);*/ _addComonent.StartCall();
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
		// タグ設定
		Tag::Type curTag = tag.GetType();
		if (Tag::ImGuiComboBox(curTag))
			SetTag(curTag);

		ImGui::SameLine();

		// レイヤー設定
		Layer::Type curLayer = layer.GetType();
		if (Layer::ImGuiComboBox(curLayer))
			SetLayer(curLayer);

		ImGui::PopItemWidth();

		// 親オブジェクトを設定
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
			bool isDelete = false;	// コンポーネント削除フラグ

			if (ImGuiMethod::TreeNode((*cpItr)->GetName().c_str()))
			{
				// 削除ボタン
				isDelete = ImGui::Button("Delete");
				// コンポーネント内の編集
				(*cpItr)->ImGuiSettingCall();

				// 優先度
				int pri = (*cpItr)->GetPriority();
				if (ImGui::DragInt("Priority", &pri))
				{
					isChangePriority = true;	// 並べ替えるようにする
					(*cpItr)->SetPriority(pri);
				}

				ImGui::TreePop();
			}

			if (isDelete)	// 削除するなら
			{
				const auto& nextItr = std::next(cpItr);
				DeleteComponent(*(*cpItr));
				cpItr = nextItr;
			}
			else
				cpItr++;
		}

		// 優先度順に並べる
		if (isChangePriority)
			SortCompPriority();

		ImGui::Dummy(ImVec2(0, 10));

		// コンポーネント追加
		ComponentFactory* compFactory = ComponentFactory::GetInstance();
		compFactory->CreateImGuiCombo(*this);
	}

	json GameObject::Save()
	{
		json objectData;

		objectData["active"] = isActive;
		objectData["name"] = name;
		objectData["tag"] = tag.GetType();
		objectData["layer"] = layer.GetType();
		objectData["transform"] = pTransform->Save();

		auto& componentData = objectData["components"];
		// コンポーネントのセーブ
		for (auto& comp : components)
		{
			componentData.push_back(comp->Save());
		}

		return objectData;
	}


	void GameObject::Load(const json& _data)
	{
		using namespace HashiTaku;

		bool loadActive = true;
		LoadJsonBoolean("active", loadActive, _data);
		SetActive(loadActive);

		Tag::Type tagType;
		LoadJsonEnum<Tag::Type>("tag", tagType, _data);
		SetTag(tagType);

		Layer::Type layerType;
		LoadJsonEnum<Layer::Type>("layer", layerType, _data);
		SetLayer(layerType);

		if (IsJsonContains(_data, "components"))
		{
			const auto& componentsData = _data["components"];
			LoadCreateComponnet(componentsData);
		}

	}

	void GameObject::LateLode(const json& _data)
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

		InSceneSystemManager::GetInstance()->GetSceneObjects().SortUiList();
	}

	void GameObject::SetName(const std::string& _name)
	{
		if (_name.empty()) return;	// 名前がないなら
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

	void GameObject::SetTag(Tag::Type _setType)
	{
		tag.SetType(_setType);
	}

	void GameObject::SetLayer(Layer::Type _setType)
	{
		// 同じレイヤーなら終わる
		if (layer.GetType() == _setType) return;

		// レイヤーを変更
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

	float GameObject::GetDeltaSpeed() const
	{
		return deltaTimeSpeed;
	}

	bool GameObject::GetIsActive() const
	{
		return isActive;
	}

	Tag::Type GameObject::GetTag() const
	{
		return tag.GetType();
	}

	Layer::Type GameObject::GetLayer() const
	{
		return layer.GetType();
	}
}