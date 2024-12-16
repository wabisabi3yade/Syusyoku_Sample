#include "pch.h"
#include "ComponentFactory.h"
#include "GameObject.h"

namespace HashiTaku
{
	void ComponentFactory::Init()
	{
		// �����蔻��
		ResistComponnent<CP_RigidBody>();
		ResistComponnent<CP_BoxCollider>();
		ResistComponnent<CP_SphereCollider>();
		ResistComponnent<CP_CylinderCollider>();
		ResistComponnent<CP_CapsuleCollider>();

		// �`��
		ResistComponnent<CP_MeshRenderer>();
		ResistComponnent<CP_SpriteRenderer>();
		ResistComponnent<CP_OutLineRenderer>();
		ResistComponnent<CP_SkyBox>();

		// �A�j���[�V����
		ResistComponnent<CP_Animation>();
		ResistComponnent<CP_Rig>();

		// �J����
		ResistComponnent<CP_Camera>();
		ResistComponnent<CP_CameraMove>();

		// �v���C���[
		ResistComponnent<CP_Player>();

		// �G
		ResistComponnent<CP_Enemy>();
		ResistComponnent<CP_Boss>();

		// �L�����N�^�[����
		ResistComponnent<CP_Weapon>();
		ResistComponnent<CP_GroundCheck>();

		// �C���Q�[����
		ResistComponnent<CP_HitStopManager>();
		ResistComponnent<CP_BattleManager>();

		// UI
		ResistComponnent<CP_UIRenderer>();
		ResistComponnent<CP_UISlider>();
		ResistComponnent<CP_Button>();
		ResistComponnent<CP_TitleButtonGroup>();
		ResistComponnent<CP_Fade>();

		// �G�t�F�N�g
		ResistComponnent<CP_VisualEffect>();
	}

	std::unique_ptr<Component> ComponentFactory::CreateByName(const std::string& _componentName)
	{
		auto itr = pComponents.find(_componentName);

		if (itr == pComponents.end())
		{
			HASHI_DEBUG_LOG(_componentName + "�̓��X�g�ɂ���܂���");
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