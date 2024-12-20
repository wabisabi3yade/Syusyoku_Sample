#include "pch.h"
#include "AttackInformation.h"
#include "AssetGetter.h"
#include "VisualEffect.h"
#include "CameraShakeParameter.h"

namespace HashiTaku
{
	AttackInformation::AttackInformation()
		: atkDamage(0.0f), hitStopFrame(0), atkLevel(AttackLevel::Low), isCamShake(false)
	{
		SetAttackLevel(AttackLevel::Low);
	}

	void AttackInformation::SetDamageValue(float _atkDamage)
	{
		atkDamage = _atkDamage;
	}

	void AttackInformation::SetHitStopFlame(u_int _hitStopFlame)
	{
		hitStopFrame = _hitStopFlame;
	}

	void AttackInformation::SetAttackLevel(AttackLevel _atkLevel)
	{
		atkLevel = _atkLevel;
		ApplyFromAttackLevel();
	}

	const CreateVfxInfo& AttackInformation::GetHitVfxInfo() const
	{
		return hitVfxInfo;
	}

	const CameraShakeParameter& AttackInformation::GetCamShakeParam() const
	{
		return pCamShakeParam;
	}

	float AttackInformation::GetDamageValue() const
	{
		return atkDamage;
	}

	u_int AttackInformation::GetHitStopFlame() const
	{
		return hitStopFrame;
	}

	AttackInformation::AttackLevel AttackInformation::GetAttackLevel() const
	{
		return atkLevel;
	}

	bool AttackInformation::GetIsShake() const
	{
		return isCamShake;
	}

	nlohmann::json AttackInformation::Save()
	{
		nlohmann::json data;

		data["hitVfx"] = hitVfxInfo.Save();
		data["isCamShake"] = isCamShake;
		if (isCamShake)
			data["camShake"] = pCamShakeParam.Save();
		data["damage"] = atkDamage;
		data["level"] = atkLevel;

		return data;
	}

	void AttackInformation::Load(const nlohmann::json& _data)
	{
		nlohmann::json loadData;
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

		LoadJsonFloat("damage", atkDamage, _data);

		if (LoadJsonEnum<AttackLevel>("level", atkLevel, _data))
		{
			SetAttackLevel(atkLevel);
		}
	}

	void AttackInformation::ApplyFromAttackLevel()
	{
		using enum AttackInformation::AttackLevel;
		switch (atkLevel)
		{
		case Low:
			hitStopFrame = LOW_HITSTOP;
			break;

		case Mid:
			hitStopFrame = MID_HITSTOP;
			break;
		case High:
			hitStopFrame = HIGH_HITSTOP;
			break;
		case SuperHigh:
			hitStopFrame = SUPERHIGH_HITSTOP;
			break;
		}
	}

	void AttackInformation::ImGuiDebug()
	{
		// ���x��
		std::vector<std::string> levelNames
		{
			"Low",
			"Middle",
			"High",
			"SuperHigh"
		};

		ImGui::Text("HSframe:%d", hitStopFrame);
		ImGui::DragFloat("AtkDamage", &atkDamage, 0.1f, 0.0f, 9999.0f);
		u_int id = static_cast<u_int>(atkLevel);
		if (ImGuiMethod::ComboBox("AtkLevel", id, levelNames))
		{
			SetAttackLevel(static_cast<AttackLevel>(id));
		}

		hitVfxInfo.ImGuiCall();

		ImGui::Checkbox("IsCameraShake", &isCamShake);

		if (isCamShake)
			pCamShakeParam.ImGuiCall();
	}
}