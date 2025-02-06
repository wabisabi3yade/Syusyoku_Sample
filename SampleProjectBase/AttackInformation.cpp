#include "pch.h"
#include "AttackInformation.h"
#include "IAttacker.h"
#include "AssetGetter.h"
#include "VisualEffect.h"

namespace HashiTaku
{
	// ヒットストップフレーム
	std::array<u_int, static_cast<u_int>(AttackInformation::AttackLevel::MaxNum)>  AttackInformation::hitStopFrames =
	{
		10, 14, 18, 22
	};
	// パッド振動力
	std::array<float, static_cast<u_int>(AttackInformation::AttackLevel::MaxNum)>  AttackInformation::padShakePowers =
	{
		0.4f, 0.6f, 1.0f, 1.0f
	};

	// パッド振動時間
	std::array<float, static_cast<u_int>(AttackInformation::AttackLevel::MaxNum)>  AttackInformation::padShakeTimes =
	{
		0.2f, 0.2f, 0.4f, 0.6f
	};

	AttackInformation::AttackInformation(IAttacker* _pAttacker) : 
		pAttacker(_pAttacker),
		atkDamage(0.0f),
		atkLevel(AttackLevel::Low),
		isCamShake(false)
	{
		SetAttackLevel(AttackLevel::Low);
	}

	void AttackInformation::SetDamageValue(float _atkDamage)
	{
		atkDamage = _atkDamage;
	}

	void AttackInformation::SetAttackLevel(AttackLevel _atkLevel)
	{
		atkLevel = _atkLevel;
	}

	void AttackInformation::NotifyHitAttack(const DXSimp::Vector3& _contactPos)
	{
		pAttacker->OnAttacking(*this, _contactPos);
	}

	DXSimp::Vector3 AttackInformation::GetAttackerWorldPos() const
	{
#ifdef EDIT
		if (!pAttacker) return DXSimp::Vector3::Zero;
#endif // EDIT

		return pAttacker->GetAttackerWorldPos();
	}

	const CreateVfxInfo& AttackInformation::GetHitVfxInfo() const
	{
		return hitVfxInfo;
	}

	const PerlinShakeParameter& AttackInformation::GetCamShakeParam() const
	{
		return pCamShakeParam;
	}

	const PlaySoundParameter& AttackInformation::GetHitSEParam() const
	{
		return hitSoundParameter;
	}

	IAttacker* AttackInformation::GetAttacker()
	{
		return pAttacker;
	}

	float AttackInformation::GetDamageValue() const
	{
		return atkDamage;
	}

	u_int AttackInformation::GetHitStopFlame() const
	{
		return hitStopFrames[static_cast<u_int>(atkLevel)];
	}

	AttackInformation::AttackLevel AttackInformation::GetAttackLevel() const
	{
		return atkLevel;
	}

	bool AttackInformation::GetIsCamShake() const
	{
		return isCamShake;
	}

	float AttackInformation::GetPadShakePower() const
	{
		return padShakePowers[static_cast<u_int>(atkLevel)];
	}

	float AttackInformation::GetPadShakeTime() const
	{
		return padShakeTimes[static_cast<u_int>(atkLevel)];
	}

	json AttackInformation::Save()
	{
		json data;

		data["hitVfx"] = hitVfxInfo.Save();
		data["isCamShake"] = isCamShake;
		if (isCamShake)
			data["camShake"] = pCamShakeParam.Save();
		data["hitSE"] = hitSoundParameter.Save();
		data["damage"] = atkDamage;
		data["level"] = atkLevel;

		return data;
	}

	void AttackInformation::Load(const json& _data)
	{
		json loadData;
		if (LoadJsonData("hitVfx", loadData, _data))
		{
			hitVfxInfo.Load(loadData);
		}

		LoadJsonBoolean("isCamShake", isCamShake, _data);
		if (isCamShake)
		{
			if (LoadJsonData("camShake", loadData, _data))
				pCamShakeParam.Load(loadData);
		}
		if (LoadJsonData("hitSE", loadData, _data))
		{
			hitSoundParameter.Load(loadData);
		}

		LoadJsonFloat("damage", atkDamage, _data);

		if (LoadJsonEnum<AttackLevel>("level", atkLevel, _data))
		{
			SetAttackLevel(atkLevel);
		}
	}

	void AttackInformation::ImGuiLevelParamerter()
	{
		if (!ImGuiMethod::TreeNode("LevelParameter")) return;

		// ヒットストップ
		if (ImGuiMethod::TreeNode("HitStop"))
		{
			u_int levelMaxId = static_cast<u_int>(AttackLevel::MaxNum);
			std::string levelStr;
			for (u_int l_i = 0; l_i < levelMaxId; l_i++)
			{
				AttackLevel level = static_cast<AttackLevel>(l_i);
				levelStr = magic_enum::enum_name(level);

				ImGui::PushID(l_i);
				ImGui::DragScalar(levelStr.c_str(), ImGuiDataType_U32, &hitStopFrames[l_i]);
				ImGui::PopID();
			}

			ImGui::TreePop();
		}
		
		// パッド振動
		if (ImGuiMethod::TreeNode("PadShake"))
		{
			u_int levelMaxId = static_cast<u_int>(AttackLevel::MaxNum);
			std::string levelStr;
			for (u_int l_i = 0; l_i < levelMaxId; l_i++)
			{
				AttackLevel level = static_cast<AttackLevel>(l_i);
				levelStr = magic_enum::enum_name(level);

				ImGui::PushID(l_i);
				ImGui::Text(levelStr.c_str());
				ImGui::DragFloat("Power", &padShakePowers[l_i], 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("Time", &padShakeTimes[l_i], 0.1f, 0.0f, 100.0f);
				ImGui::PopID();
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	void AttackInformation::ImGuiDebug()
	{
		// レベル
		std::vector<std::string> levelNames
		{
			"Low",
			"Middle",
			"High",
			"SuperHigh"
		};

		ImGui::DragFloat("AtkDamage", &atkDamage, 0.1f, 0.0f, 9999.0f);
		u_int id = static_cast<u_int>(atkLevel);
		if (ImGuiMethod::ComboBox("AtkLevel", id, levelNames))
		{
			SetAttackLevel(static_cast<AttackLevel>(id));
		}

		// エフェクト
		hitVfxInfo.ImGuiCall();

		// カメラシェイク
		ImGui::Checkbox("IsCameraShake", &isCamShake);
		if (isCamShake)
			pCamShakeParam.ImGuiCall();

		// サウンド
		if (ImGuiMethod::TreeNode("Sound"))
		{
			hitSoundParameter.ImGuiCall();
			ImGui::TreePop();
		}

		ImGuiLevelParamerter();
	}
}