#include "pch.h"
#include "CP_Enemy.h"
#include "CP_BattleManager.h"
#include "CP_HitStopManager.h"
#include "CP_MeshRenderer.h"
#include "InSceneSystemManager.h"

#ifdef EDIT
#include "Geometory.h"
#endif // EDIT

namespace HashiTaku
{
	CP_Enemy::CP_Enemy() :
		pReserveShakeParam(nullptr)
	{
		SetEnemyName("Boss");
	}

	const std::string& CP_Enemy::GetEnemyName() const
	{
		return enemyName;
	}

	const DXSimp::Vector3& CP_Enemy::GetWorldPos() const
	{
		return GetTransform().GetPosition();
	}

	void CP_Enemy::SetTargeter(IObjectTargeter& _targeter)
	{
		auto itr = std::find(pTargeters.begin(), pTargeters.end(), &_targeter);
		if (itr != pTargeters.end())
		{
			HASHI_DEBUG_LOG("�^�[�Q�b�g���d�����܂���");
			return;
		}

		pTargeters.push_back(&_targeter);
	}

	void CP_Enemy::RemoveTargeter(IObjectTargeter& _targeter)
	{
		pTargeters.remove(&_targeter);
	}

	void CP_Enemy::OnHitStopBegin()
	{
		CP_Character::OnHitStopBegin();
	}

	void CP_Enemy::OnHitStopEnd()
	{
		CP_Character::OnHitStopEnd();

		// �h������Z�b�g
		acceptDamageShake.EndShake();
		pMeshRenderer->SetMeshOffset(DXSimp::Vector3::Zero);
	}

	void CP_Enemy::OnRemoveNotifyToTargeter()
	{
		for (auto& targeter : pTargeters)
			targeter->RemoveNotify(*this);
	}

	json CP_Enemy::Save()
	{
		json data = CP_Character::Save();

		// �q�b�g�h��̃p�����[�^
		json& hitShakeDatas = data["hitShakeParams"];
		for (auto& shParam : acceptDamageShakeParams)
		{
			hitShakeDatas.push_back(shParam.Save());
		}

		return data;
	}

	void CP_Enemy::Load(const json& _data)
	{
		CP_Character::Load(_data);

		json hitShakeDatas;
		if (LoadJsonDataArray("hitShakeParams", hitShakeDatas, _data))
		{
			u_int atkCnt = static_cast<u_int>(acceptDamageShakeParams.size());
			u_int a_i = 0;
			// ���x�����ƂɃ��[�h
			for (auto& shData : hitShakeDatas)
			{
				if (a_i >= atkCnt) break; // �O��̃��x���̐���菭�Ȃ��Ȃ��Ă�����

				// ���[�h
				acceptDamageShakeParams[a_i].Load(shData);
				a_i++;
			}
		}
	}

	void CP_Enemy::Start()
	{
		CP_Character::Start();

		// ���b�V���`��擾
		pMeshRenderer = GetGameObject().GetComponent<CP_MeshRenderer>();
		if (!pMeshRenderer)
			HASHI_DEBUG_LOG("���b�V���`�悪�擾�ł��܂���");

		// �o�g���}�l�W���[������Ȃ�G��ǉ�����
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->AddEnemy(*this);
		}
	}

	void CP_Enemy::Update()
	{
		// �q�b�g�X�g�b�v���Ƀ��b�V����h�炷
		HitStopShaking();
	}

	void CP_Enemy::OnDestroy()
	{
		OnRemoveNotifyToTargeter();

		// �o�g���}�l�W���[������Ȃ玩�g���폜����
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->RemoveEnemy(*this);
		}
	}

	void CP_Enemy::HitStopShaking()
	{
		// �q�b�g�X�g�b�v���łȂ��@�������́@�h���悤�ɂ��Ă��Ȃ��Ȃ珈�����Ȃ�
		if (!GetIsHitStopping()) return;

		// ���b�V���`�悪�Ȃ��Ȃ珈�����Ȃ�
		if (!pMeshRenderer) return;

		// �q�b�g�X�g�b�v���Ȃ̂ŃA�v���P�[�V�������̃�t��
		acceptDamageShake.Update(MainApplication::DeltaTime());

		// ���b�V����h�炷
		pMeshRenderer->SetMeshOffset(acceptDamageShake.GetShakeOffset());
	}

	bool CP_Enemy::OnDamageBehavior(AttackInformation& _attackInfo)
	{
		return true;
	}

	void CP_Enemy::OnTakeDamage(const AttackInformation& _attackInfo, const DXSimp::Vector3& _contactPos)
	{
		CP_Character::OnTakeDamage(_attackInfo, _contactPos);

		// �h���悤�ɂ���
		u_int atkLevelId = static_cast<u_int>(_attackInfo.GetAttackLevel());
		pReserveShakeParam = &acceptDamageShakeParams[atkLevelId];

		// �U���̌����ɂ�炷
		DXSimp::Vector3 attackVec = GetTransform().GetPosition() - _contactPos;
		attackVec.Normalize();
		pReserveShakeParam->shakeVec = Vec3::Abs(attackVec);

		// �h���\��
		acceptDamageShake.BeginShake(*pReserveShakeParam);
	}

	void CP_Enemy::ImGuiDebug()
	{
		CP_Character::ImGuiDebug();

		ImGuiHitShake();
	}

	void CP_Enemy::ImGuiHitShake()
	{
#ifdef EDIT
		// �q�b�g���̗h��p�����[�^
		if (!ImGuiMethod::TreeNode("Hit SHake")) return;

		u_int atkLecelCnt = static_cast<u_int>(AttackInformation::AttackLevel::MaxNum);
		for (u_int a_i = 0; a_i < atkLecelCnt; a_i++)
		{
			ImGui::PushID(a_i);

			auto levelStr = magic_enum::enum_name(static_cast<AttackInformation::AttackLevel>(a_i));
			ImGui::Text(std::string(levelStr).c_str());

			acceptDamageShakeParams[a_i].ImGuiCall();

			ImGui::PopID();
		}

		ImGui::TreePop();
#endif // EDIT
	}

	void CP_Enemy::SetEnemyName(const std::string& _enemyName)
	{
		enemyName = _enemyName;
	}
}