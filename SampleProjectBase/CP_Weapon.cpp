#include "pch.h"
#include "CP_Weapon.h"
#include "CP_RigidBody.h"
#include "GameObject.h"
#include "AssetGetter.h"

#ifdef EDIT
#include "Geometory.h"
constexpr DirectX::SimpleMath::Color ATTACK_COLOR(1.0f, 1.0f, 0.0f);
constexpr DirectX::SimpleMath::Color NO_ATTACK_COLOR(0.0f, 0.0f, 1.0f);
constexpr DirectX::SimpleMath::Vector3 DISPLAY_SCALE(1.0f, 1.0f, 1.0f);
#endif // EDIT

namespace HashiTaku
{
	CP_Weapon::CP_Weapon() :
		pHaveObjectPos(nullptr),
		pAtkInfomation(nullptr),
		attackTagCnt(0),
		isAttackCollision(false)
	{
		// 初期化
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
		// 攻撃できるかチェック
		if (!CanAttack()) return;

		GameObject& gameObject = _otherColInfo.pRigidBodyCp->GetGameObject();
		if (!CheckAttackableTag(gameObject)) return;	// タグチェック
		if (!CheckAttackedRb(*_otherColInfo.pRigidBodyCp)) return;	// 攻撃済みチェック

		// ダメージインターフェースあるかチェック
		IDamageable* pDamager = gameObject.GetComponent<IDamageable>();
		if (!pDamager) return;

		// 攻撃処理
		DXSimp::Vector3 haveObjPos;	// 所有オブジェクトの座標を取得する
		if (pHaveObjectPos) haveObjPos = *pHaveObjectPos;
		else
			HASHI_DEBUG_LOG("所有オブジェクトの座標が設定されていません");

		// 攻撃する
		OnAttack(*pDamager, haveObjPos);

		// ヒットエフェクトを生成
		CreateHitVfx(_otherColInfo.contactPoint);

		// 追加する
		AddAttackedRb(*_otherColInfo.pRigidBodyCp);
	}

	void CP_Weapon::SetAttackInfo(const AttackInformation& _attackInformation)
	{
		pAtkInfomation = &_attackInformation;
		// 攻撃済みをクリア
		ClearAttackedRb();
	}

	void CP_Weapon::SetIsAttackCollision(bool _isAttackCollision)
	{
		isAttackCollision = _isAttackCollision;
	}

	void CP_Weapon::SetHaveObjPosPointer(const DirectX::SimpleMath::Vector3* _pWorldPos)
	{
		pHaveObjectPos = _pWorldPos;
	}

	void CP_Weapon::ClearAttackedRb()
	{
		attackedRbs.clear();
	}

	nlohmann::json CP_Weapon::Save()
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

	void CP_Weapon::Load(const nlohmann::json& _data)
	{
		using namespace HashiTaku;

		Component::Load(_data);

		nlohmann::json attackTagDatas;
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
		if (!isAttackCollision) return false; // 攻撃フラグがついていないなら
		if (!pAtkInfomation) return false;	// 攻撃情報が無かったら

		return true;
	}

	void CP_Weapon::OnAttack(IDamageable& _damager,
		const DirectX::SimpleMath::Vector3& _haveObjPos)
	{
		// ダメージを与える
		_damager.OnDamage(*pAtkInfomation, _haveObjPos);
	}

	void CP_Weapon::AddAttackedRb(const CP_RigidBody& _rb)
	{
		// 追加する
		attackedRbs.push_back(&_rb);
	}

	bool CP_Weapon::CheckAttackableTag(GameObject& _targetObject) const
	{
		// 攻撃タグと一緒か
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
		// 既に攻撃されているところに入っているか
		u_int attackedRbCnt = static_cast<u_int>(attackedRbs.size());
		for (u_int rb_i = 0; rb_i < attackedRbCnt; rb_i++)
		{
			if (attackedRbs[rb_i] == &_targetRb)
				return false;
		}

		return true;
	}

	void CP_Weapon::CreateHitVfx(const DirectX::SimpleMath::Vector3& _contactPos)
	{
		const CreateVfxInfo& hitVfxInfo = pAtkInfomation->GetHitVfxInfo();

		// 攻撃情報からエフェクトを取得する
		auto* pVfx = hitVfxInfo.pHitVfx;
		if (!pVfx) return;

		// 再生
		DX11EffecseerManager::GetInstance()->Play(hitVfxInfo, _contactPos);
	}

	void CP_Weapon::DebugAttackFlag()
	{
#ifdef EDIT
		// 攻撃可能判定描画
		using namespace DirectX::SimpleMath;
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
			HASHI_DEBUG_LOG("追加上限です");
			return;
		}

		attackTagCnt++;	// カウントを進める
		attackableTags[attackTagCnt - 1] = _addTag;
	}

	void CP_Weapon::RemoveBackAttackableTag()
	{
		if (attackTagCnt == 0) return;

		attackTagCnt--;	// 減らす
	}

	void CP_Weapon::ImGuiDebug()
	{
#ifdef EDIT
		ImGui::Text("Attack Tag");
		// 追加しているタグ
		for (u_int t_i = 0; t_i < attackTagCnt; t_i++)
		{
			// 表示
			ImGui::Text(std::string(magic_enum::enum_name(attackableTags[t_i])).c_str());
			ImGui::SameLine();

			ImGui::PushID(attackableTags[t_i]);

			ImGui::PopID();
		}
		// 削除
		if (ImGui::Button("X"))
		{
			RemoveBackAttackableTag();
		}

		// 追加
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