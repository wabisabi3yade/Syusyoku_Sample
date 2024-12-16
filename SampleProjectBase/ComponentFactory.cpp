#include "pch.h"
#include "ComponentFactory.h"
#include "GameObject.h"

namespace HashiTaku
{
	void ComponentFactory::Init()
	{
		// 当たり判定
		ResistComponnent<CP_RigidBody>();
		ResistComponnent<CP_BoxCollider>();
		ResistComponnent<CP_SphereCollider>();
		ResistComponnent<CP_CylinderCollider>();
		ResistComponnent<CP_CapsuleCollider>();

		// 描画
		ResistComponnent<CP_MeshRenderer>();
		ResistComponnent<CP_SpriteRenderer>();
		ResistComponnent<CP_OutLineRenderer>();
		ResistComponnent<CP_SkyBox>();

		// アニメーション
		ResistComponnent<CP_Animation>();
		ResistComponnent<CP_Rig>();

		// カメラ
		ResistComponnent<CP_Camera>();
		ResistComponnent<CP_CameraMove>();

		// プレイヤー
		ResistComponnent<CP_Player>();

		// 敵
		ResistComponnent<CP_Enemy>();
		ResistComponnent<CP_Boss>();

		// キャラクター共通
		ResistComponnent<CP_Weapon>();
		ResistComponnent<CP_GroundCheck>();

		// インゲーム内
		ResistComponnent<CP_HitStopManager>();
		ResistComponnent<CP_BattleManager>();

		// UI
		ResistComponnent<CP_UIRenderer>();
		ResistComponnent<CP_UISlider>();
		ResistComponnent<CP_Button>();
		ResistComponnent<CP_TitleButtonGroup>();
		ResistComponnent<CP_Fade>();

		// エフェクト
		ResistComponnent<CP_VisualEffect>();
	}

	std::unique_ptr<Component> ComponentFactory::CreateByName(const std::string& _componentName)
	{
		auto itr = pComponents.find(_componentName);

		if (itr == pComponents.end())
		{
			HASHI_DEBUG_LOG(_componentName + "はリストにありません");
			return nullptr;
		}

		auto create = itr->second->Create();
		create->SetName(_componentName);
		return std::move(create);
	}

	void ComponentFactory::CreateImGuiCombo(GameObject& _targetObject)
	{
#ifdef EDIT
		static std::string compName = "";
		std::vector<const std::string*> conponentsName = GetComponentsName();

		if (ImGui::Button("Add"))
		{
			_targetObject.SetComponent(CreateByName(compName));
		}

		ImGuiMethod::ComboBox("Component", compName, conponentsName);
#endif // EDIT
	}

	std::vector<const std::string*> ComponentFactory::GetComponentsName()
	{
		std::vector<const std::string*> s;

		for (auto& itr : pComponents)
		{
			s.push_back(&itr.first);
		}

		return s;
	}
}