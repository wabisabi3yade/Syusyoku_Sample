#include "pch.h"
#include "CP_Weapon.h"
#include "CP_RigidBody.h"
#include "CP_CameraMove.h"
#include "GameObject.h"
#include "AssetGetter.h"
#include "InSceneSystemManager.h"
#ifdef EDIT
#include "Geometory.h"
#endif // EDIT

namespace HashiTaku
{
#ifdef EDIT
constexpr DXSimp::Color ATTACK_COLOR(1.0f, 1.0f, 0.0f);
constexpr DXSimp::Color NO_ATTACK_COLOR(0.0f, 0.0f, 1.0f);
constexpr DXSimp::Vector3 DISPLAY_SCALE(1.0f, 1.0f, 1.0f);
#endif // EDIT

	CP_Weapon::CP_Weapon() :
		pAtkInfomation(nullptr),
		pAttacker(nullptr),
		attackTagCnt(0),
		isAttackCollision(false)
	{
		// ������
		for (u_int t_i = 0; t_i < ATTACK_TAG_MAX; t_i++)
		{
			attackableTags[t_i] = Tag::Default;
		}
	}

	void CP_Weapon::Draw()
	{
		DebugAttackFlag();
	}

	void CP_Weapon::OnCollisionStay(const CollisionInfo& _otherColInfo)
	{
		// �U���ł��邩�`�F�b�N
		if (!CanAttack()) return;

		GameObject& gameObject = _otherColInfo.pRigidBodyCp->GetGameObject();
		if (!CheckAttackableTag(gameObject)) return;	// �^�O�`�F�b�N
		if (!CheckAttackedRb(*_otherColInfo.pRigidBodyCp)) return;	// �U���ς݃`�F�b�N

		// �_���[�W�C���^�[�t�F�[�X���邩�`�F�b�N
		IDamageable* pDamager = gameObject.GetComponent<IDamageable>();
		if (!pDamager) return;

		// �U������
		OnAttack(*pDamager, _otherColInfo.contactPoint);

		// �ǉ�����
		AddAttackedRb(*_otherColInfo.pRigidBodyCp);
	}

	void CP_Weapon::SetAttackInfo(AttackInformation& _attackInformation)
	{
		pAtkInfomation = &_attackInformation;
		// �U���ς݂��N���A
		ClearAttackedRb();
	}

	void CP_Weapon::SetIsAttackCollision(bool _isAttackCollision)
	{
		isAttackCollision = _isAttackCollision;
	}

	void CP_Weapon::SetWeaponOwner(IAttacker& _weaponOwner)
	{
		pAttacker = &_weaponOwner;
	}

	void CP_Weapon::ClearAttackedRb()
	{
		attackedRbs.clear();
	}

	json CP_Weapon::Save()
	{
		auto data = Component::Save();

		auto& attackTagDatas = data["attackableTags"];
		for (u_int t_i = 0; t_i < attackTagCnt; t_i++)
		{
			attackTagDatas.push_back(attackableTags[t_i]);
		}

#ifdef EDIT
		data["isDebugDisplay"] = isDebugAttackDisplay;
#endif // EDIT
		return data;
	}

	void CP_Weapon::Load(const json& _data)
	{
		using namespace HashiTaku;

		Component::Load(_data);

		json attackTagDatas;
		if (LoadJsonDataArray("attackableTags", attackTagDatas, _data))
		{
			for (auto& tagData : attackTagDatas)
			{
				if (tagData.is_number_integer())
					AddAttackableTag(static_cast<Tag::Type>(tagData));
			}
		}

#ifdef EDIT
		LoadJsonBoolean("isDebugDisplay", isDebugAttackDisplay, _data);
#endif // EDIT

	}

	bool CP_Weapon::CanAttack() const
	{
		if (!isAttackCollision) return false; // �U���t���O�����Ă��Ȃ��Ȃ�
		if (!pAtkInfomation) return false;	// �U����񂪖���������

		return true;
	}

	void CP_Weapon::OnAttack(IDamageable& _damager, const DXSimp::Vector3& _contactPos)
	{
		// �U������
		DXSimp::Vector3 haveObjPos;
		// �_���[�W��^���鑤�̏���
		if (pAttacker)
		{
			// �U����^�������̃R�[���o�b�N
			pAttacker->OnAttacking(*pAtkInfomation);

			// ���L�I�u�W�F�N�g�̍��W���擾����
			haveObjPos = pAttacker->GetAttackerWorldPos();
		}
			
		// �_���[�W��H�炤���̏���
		_damager.OnDamage(*pAtkInfomation, _contactPos);
	}

	void CP_Weapon::AddAttackedRb(const CP_RigidBody& _rb)
	{
		// �ǉ�����
		attackedRbs.push_back(&_rb);
	}

	bool CP_Weapon::CheckAttackableTag(GameObject& _targetObject) const
	{
		// �U���^�O�ƈꏏ��
		Tag::Type objTag = _targetObject.GetTag();
		for (u_int t_i = 0; t_i < attackTagCnt; t_i++)
		{
			if (attackableTags[t_i] == objTag)
				return true;
		}

		return false;
	}

	bool CP_Weapon::CheckAttackedRb(const CP_RigidBody& _targetRb) const
	{
		// ���ɍU������Ă���Ƃ���ɓ����Ă��邩
		u_int attackedRbCnt = static_cast<u_int>(attackedRbs.size());
		for (u_int rb_i = 0; rb_i < attackedRbCnt; rb_i++)
		{
			if (attackedRbs[rb_i] == &_targetRb)
				return false;
		}

		return true;
	}

	void CP_Weapon::DebugAttackFlag()
	{
#ifdef EDIT
		// �U���\����`��
		using namespace DXSimp;
		if (!isDebugAttackDisplay) return;

		Geometory::SetPosition(GetTransform().GetPosition());
		Geometory::SetScale(DISPLAY_SCALE);

		if (isAttackCollision)
			Geometory::SetColor(ATTACK_COLOR);
		else
			Geometory::SetColor(NO_ATTACK_COLOR);

		Geometory::DrawSphere();
#endif // EDIT
	}

	void CP_Weapon::AddAttackableTag(Tag::Type _addTag)
	{
		if (attackTagCnt > ATTACK_TAG_MAX)
		{
			HASHI_DEBUG_LOG("�ǉ�����ł�");
			return;
		}

		attackTagCnt++;	// �J�E���g��i�߂�
		attackableTags[attackTagCnt - 1] = _addTag;
	}

	void CP_Weapon::RemoveBackAttackableTag()
	{
		if (attackTagCnt == 0) return;

		attackTagCnt--;	// ���炷
	}

	void CP_Weapon::ImGuiDebug()
	{
#ifdef EDIT
		ImGui::Text("Attack Tag");
		// �ǉ����Ă���^�O
		for (u_int t_i = 0; t_i < attackTagCnt; t_i++)
		{
			// �\��
			ImGui::Text(std::string(magic_enum::enum_name(attackableTags[t_i])).c_str());
			ImGui::SameLine();

			ImGui::PushID(attackableTags[t_i]);

			ImGui::PopID();
		}
		// �폜
		if (ImGui::Button("X"))
		{
			RemoveBackAttackableTag();
		}

		// �ǉ�
		static Tag::Type selectTag = Tag::Default;
		Tag::ImGuiComboBox(selectTag);
		ImGui::SameLine();
		if (ImGui::Button("+"))
			AddAttackableTag(selectTag);

		ImGuiMethod::LineSpaceSmall();

		ImGui::Checkbox("Attack", &isAttackCollision);
		ImGui::Checkbox("Display", &isDebugAttackDisplay);
#endif // EDIT
	}
}