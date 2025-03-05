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
		pVfxManager(nullptr),
		pSoundManager(nullptr),
		pCameraTransform(nullptr),
		attackTagCnt(0),
		attackVectorCnt(3),
		isAttackCollision(false)
	{
		// 初期化
		for (u_int t_i = 0; t_i < ATTACK_TAG_MAX; t_i++)
		{
			attackableTags[t_i] = Tag::Default;
		}
	}

	void CP_Weapon::Start()
	{
		// エフェクト・サウンドマネージャー
		pVfxManager = DX11EffecseerManager::GetInstance();
		pSoundManager = CP_SoundManager::GetInstance();

		// カメラのトランスフォーム取得
		pCameraTransform =
			&InSceneSystemManager::GetInstance()->GetMainCamera().GetTransform();
	}

	void CP_Weapon::LateUpdate()
	{
		// 攻撃の方向を更新する
		UpdateAttackVector();
	}

	void CP_Weapon::Draw()
	{
#ifdef EDIT
		DebugAttackFlag();

		if (isDebugMeasDisplay)
		{
			Geometory::SetPosition(GetMeasAtkVecWorldPos());
			Geometory::SetScale(DXSimp::Vector3::One * 0.3f);
			Geometory::DrawSphere();
		}

		// 攻撃ベクトルを表示
		constexpr float length = 3.0f;
		Geometory::AddLine(hitPos,
			hitPos + hitAtkVector * length,
			DXSimp::Color(1.0f, 0.0f, 0.0f, 0.0f));
#endif // EDIT
	}

	void CP_Weapon::UpdateAttackVector()
	{
		// 攻撃ベクトルを求める
		DXSimp::Vector3 currentPos = GetMeasAtkVecWorldPos();
		DXSimp::Vector3 vec = currentPos - prevWeaponPos;
		vec.Normalize();

		// 攻撃ベクトルをリストに追加する
		attackVectorList.push_back(vec);

		// 持つ数が上限を超えたら
		if (static_cast<u_int>(attackVectorList.size()) > attackVectorCnt)
		{
			// 1番前を除く
			attackVectorList.pop_front();
		}

		// 更新する
		prevWeaponPos = currentPos;
	}

	bool CP_Weapon::CanAttack(const CollisionInfo& _otherColInfo) const
	{
		if (!isAttackCollision) return false; // 攻撃フラグがついていないなら
		if (!pAtkInfomation) return false;	// 攻撃情報が無かったら

		GameObject& gameObject = _otherColInfo.pRigidBodyCp->GetGameObject();
		if (!CheckAttackableTag(gameObject)) return false;	// タグチェック
		if (!CheckAttackedRb(*_otherColInfo.pRigidBodyCp)) return false;	// 攻撃済みチェック

		return true;
	}

	void CP_Weapon::OnCollisionStay(const CollisionInfo& _otherColInfo)
	{
		// 攻撃できるかチェック
		if (!CanAttack(_otherColInfo)) return;

		// ダメージインターフェースあるかチェック
		IDamageable* pDamager = _otherColInfo.pRigidBodyCp->GetGameObject().GetComponent<IDamageable>();
		if (!pDamager) return;

		// 攻撃する
		OnAttack(*pDamager, _otherColInfo.contactPoint);

		//	攻撃したオブジェクトとして追加する
		AddAttackedRb(*_otherColInfo.pRigidBodyCp);
	}

	void CP_Weapon::SetAttackInfo(AttackInformation& _attackInformation)
	{
		pAtkInfomation = &_attackInformation;

		// 攻撃済みをクリア
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

		SaveJsonVector3("measAtkOffset", measAtkVecOffsetPos, data);

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

		LoadJsonVector3("measAtkOffset", measAtkVecOffsetPos, _data);

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

	void CP_Weapon::OnAttack(IDamageable& _damager, const DXSimp::Vector3& _contactPos)
	{
		// 攻撃処理
		DXSimp::Vector3 haveObjPos;
		// ダメージを与える側の処理
		if (pAttacker)
		{
			// 攻撃を与えた側のコールバック
			pAttacker->OnAttacking(*pAtkInfomation, _contactPos);

			// 所有オブジェクトの座標を取得する
			haveObjPos = pAttacker->GetAttackerWorldPos();
		}

		// ダメージ情報をまとめる
		DamageInfo damageInfo;
		damageInfo.pAttackInformation = pAtkInfomation;
		damageInfo.contactPos = _contactPos;

		// リストのベクトルを平均したものを攻撃ベクトルとする
		DXSimp::Vector3 sumVec;
		for (auto& vec : attackVectorList)
		{
			sumVec += vec;
		}
		sumVec.Normalize();
		damageInfo.attackVector = sumVec;

#ifdef EDIT
		hitPos = _contactPos;	// デバッグ用
		//hitAtkVector = sumVec;
#endif // EDIT

		// ダメージを食らう側の処理
		bool isAttackSuccess = _damager.OnDamage(damageInfo);

		// 攻撃成功したら
		if (isAttackSuccess)
		{
			OnAttackSuccess(damageInfo);
		}
	}

	void CP_Weapon::OnAttackSuccess(const DamageInfo& _damageInfo)
	{
		// ヒットエフェクト

		// 当たった角度を考慮してエフェクトを回転する
		DXSimp::Vector3 angles;
		const DXSimp::Vector3& atkVec = _damageInfo.pAttackInformation->GetAttackVector();
		const Transform& attackerTransform = pAttacker->GetAttackerTransform();
		DXSimp::Vector3 worldVec = attackerTransform.Right() * atkVec.x;
		worldVec += attackerTransform.Up() * atkVec.y;
		worldVec += attackerTransform.Forward() * atkVec.z;
		worldVec.Normalize();

#ifdef EDIT
		hitAtkVector = worldVec;
#endif // EDIT

		angles = CalcSlashWorldAngles(worldVec);

		HASHI_DEBUG_LOG("x" + std::to_string(angles.x)
		+ "y" + std::to_string(angles.y) +
			"z" +  std::to_string(angles.z));

		pVfxManager->Play(_damageInfo.pAttackInformation->GetHitVfxInfo(),
			_damageInfo.contactPos,
			angles);

		// サウンド
		if (pSoundManager)
		{
			pSoundManager->PlaySE(_damageInfo.pAttackInformation->GetHitSEParam(),
				_damageInfo.contactPos);
		}
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

	void CP_Weapon::DebugAttackFlag()
	{
#ifdef EDIT
		// 攻撃可能判定描画
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

	DXSimp::Vector3 CP_Weapon::GetMeasAtkVecWorldPos() const
	{
		const Transform& transform = GetTransform();
		DXSimp::Vector3 measPos = transform.GetPosition();
		measPos += transform.Up() * measAtkVecOffsetPos.y;
		measPos += transform.Right() * measAtkVecOffsetPos.x;
		measPos += transform.Forward() * measAtkVecOffsetPos.z;

		return measPos;
	}

	DXSimp::Vector3 CP_Weapon::CalcSlashWorldAngles(
		const DXSimp::Vector3& _slashWorldVec) const
	{
		DXSimp::Vector3 angles;
		angles.y = std::atan2(_slashWorldVec.z, _slashWorldVec.x); // Y軸回転 (Z軸回転)

		angles.z = std::atan2(_slashWorldVec.y, std::sqrt(_slashWorldVec.x *
			_slashWorldVec.x + _slashWorldVec.z * _slashWorldVec.z)); // X軸回転

		return angles * Mathf::radToDeg;
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
		ImGui::Checkbox("ColDisplay", &isDebugAttackDisplay);
		ImGui::DragScalar("AtkVecCnt", ImGuiDataType_U32, &attackVectorCnt);
		ImGui::DragFloat3("Meas Offset", &measAtkVecOffsetPos.x, 0.01f);
		ImGui::Checkbox("MeasDisplay", &isDebugMeasDisplay);
#endif // EDIT
	}
}