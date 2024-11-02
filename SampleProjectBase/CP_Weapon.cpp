#include "pch.h"
#include "CP_Weapon.h"
#include "GameObject.h"
#include "CP_HitStopManager.h"
#ifdef EDIT
#include "Geometory.h"
constexpr DirectX::SimpleMath::Color ATTACK_COLOR(1.0f, 1.0f, 0.0f);
constexpr DirectX::SimpleMath::Color NO_ATTACK_COLOR(0.0f, 0.0f, 1.0f);
constexpr DirectX::SimpleMath::Vector3 DISPLAY_SCALE(1.0f, 1.0f, 1.0f);
#endif // EDIT


CP_Weapon::CP_Weapon() : isAttackCollision(false)
{
}

void CP_Weapon::Draw()
{
	DebugAttackFlag();
}

void CP_Weapon::OnCollisionEnter(const HashiTaku::CollisionInfo& _otherColInfo)
{
	// 攻撃フラグがついていないなら
	if (!isAttackCollision) return;

	// 攻撃できるか確認
	GameObject& gameObject = _otherColInfo.pRigidBodyCp->GetGameObject();
	CheckAttackable(gameObject);

	// ダメージを受けるインターフェースがあるか確認
	HashiTaku::IDamageable* pDamager = gameObject.GetComponent<HashiTaku::IDamageable>();
	if (!pDamager) return;

	OnAttack(*pDamager);
}

void CP_Weapon::SetAttackInfo(const HashiTaku::AttackInformation& _attackInformation)
{
	atkInfomation = _attackInformation;
}

void CP_Weapon::SetIsAttackCollision(bool _isAttackCollision)
{
	isAttackCollision = _isAttackCollision;
}

nlohmann::json CP_Weapon::Save()
{
	auto data = Component::Save();

	auto& attackTagDatas = data["attackableTags"];
	for (auto& tag : attackableTags)
	{
		attackTagDatas.push_back(tag);
	}

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

}

void CP_Weapon::OnAttack(HashiTaku::IDamageable& _damager)
{
	// ダメージを与える
	_damager.OnDamage(atkInfomation);

	// ヒットストップ
	if (CP_HitStopManager* pHitStop = CP_HitStopManager::GetInstance())
	{
		pHitStop->HitStopBegin(atkInfomation.GetHitStopFlame());
	}
}

bool CP_Weapon::CheckAttackable(GameObject& _targetObject)
{
	// 重複していたら
	auto itr = std::find(attackableTags.begin(), attackableTags.end(), _targetObject.GetTag());
	if (itr == attackableTags.end()) return false;

	return true;
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

void CP_Weapon::AddAttackableTag(HashiTaku::Tag::Type _addTag)
{
	// 重複していたら
	auto itr = std::find(attackableTags.begin(), attackableTags.end(), _addTag);
	if (itr != attackableTags.end()) return;

	attackableTags.push_back(_addTag);
}

void CP_Weapon::ImGuiSetting()
{
#ifdef EDIT
	ImGui::Text("Attack Tag");
	// 追加しているタグ
	for (auto itr = attackableTags.begin(); itr != attackableTags.end();)
	{
		// 表示
		ImGui::Text(std::string(magic_enum::enum_name(*itr)).c_str());
		ImGui::SameLine();

		ImGui::PushID(*itr);

		// 削除
		bool isDelete = ImGui::Button("X");

		if (isDelete)
			itr = attackableTags.erase(itr);
		else
			itr++;

		ImGui::PopID();
	}

	// 追加
	static HashiTaku::Tag::Type selectTag = HashiTaku::Tag::Default;
	HashiTaku::Tag::ImGuiComboBox(selectTag);
	ImGui::SameLine();
	if (ImGui::Button("+"))
		AddAttackableTag(selectTag);

	ImGuiMethod::LineSpaceSmall();

	ImGui::Checkbox("Attack", &isAttackCollision);
	ImGui::Checkbox("Display", &isDebugAttackDisplay);
#endif // EDIT
}
