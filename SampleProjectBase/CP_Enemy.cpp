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
			HASHI_DEBUG_LOG("ターゲットが重複しました");
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

		// 揺れをリセット
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

		// ヒット揺れのパラメータ
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
			// レベルごとにロード
			for (auto& shData : hitShakeDatas)
			{
				if (a_i >= atkCnt) break; // 前回のレベルの数より少なくなっていたら

				// ロード
				acceptDamageShakeParams[a_i].Load(shData);
				a_i++;
			}
		}
	}

	void CP_Enemy::Start()
	{
		CP_Character::Start();

		// メッシュ描画取得
		pMeshRenderer = GetGameObject().GetComponent<CP_MeshRenderer>();
		if (!pMeshRenderer)
			HASHI_DEBUG_LOG("メッシュ描画が取得できません");

		// バトルマネジャーがあるなら敵を追加する
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->AddEnemy(*this);
		}
	}

	void CP_Enemy::Update()
	{
		// ヒットストップ中にメッシュを揺らす
		HitStopShaking();
	}

	void CP_Enemy::OnDestroy()
	{
		OnRemoveNotifyToTargeter();

		// バトルマネジャーがあるなら自身を削除する
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->RemoveEnemy(*this);
		}
	}

	void CP_Enemy::HitStopShaking()
	{
		// ヒットストップ中でない　もしくは　揺れるようにしていないなら処理しない
		if (!GetIsHitStopping()) return;

		// メッシュ描画がないなら処理しない
		if (!pMeshRenderer) return;

		// ヒットストップ中なのでアプリケーション側のΔtで
		acceptDamageShake.Update(MainApplication::DeltaTime());

		// メッシュを揺らす
		pMeshRenderer->SetMeshOffset(acceptDamageShake.GetShakeOffset());
	}

	bool CP_Enemy::OnDamageBehavior(AttackInformation& _attackInfo)
	{
		return true;
	}

	void CP_Enemy::OnTakeDamage(const AttackInformation& _attackInfo, const DXSimp::Vector3& _contactPos)
	{
		CP_Character::OnTakeDamage(_attackInfo, _contactPos);

		// 揺れるようにする
		u_int atkLevelId = static_cast<u_int>(_attackInfo.GetAttackLevel());
		pReserveShakeParam = &acceptDamageShakeParams[atkLevelId];

		// 攻撃の向きにゆらす
		DXSimp::Vector3 attackVec = GetTransform().GetPosition() - _contactPos;
		attackVec.Normalize();
		pReserveShakeParam->shakeVec = Vec3::Abs(attackVec);

		// 揺れを予約
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
		// ヒット時の揺れパラメータ
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