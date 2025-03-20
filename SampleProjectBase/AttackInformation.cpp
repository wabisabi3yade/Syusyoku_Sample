#include "pch.h"
#include "AttackInformation.h"
#include "IAttacker.h"
#include "AssetGetter.h"
#include "VisualEffect.h"

namespace HashiTaku
{
	/// @brief ヒットエフェクト登録可能最大数
	constexpr u_int MAX_RESIST_HITVFX(10);

	// ヒットストップフレーム
	std::array<u_int, static_cast<u_int>(AttackInformation::AttackLevel::MaxNum)>  AttackInformation::hitStopFrames =
	{
		4, 10, 18, 22
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

	const DXSimp::Vector3& AttackInformation::GetAttackVector() const
	{
		return attackVector;
	}

	const std::list<CreateVfxInfo>& AttackInformation::GetHitVfxList() const
	{
		return hitVfxInfoList;
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
		// ヒットエフェクトセーブ
		json& hitVfxDatas = data["hitVfxDatas"];
		for (auto& hitVfx : hitVfxInfoList)
		{
			hitVfxDatas.push_back(hitVfx.Save());
		}

		data["isCamShake"] = isCamShake;
		if (isCamShake)
			data["camShake"] = pCamShakeParam.Save();
		SaveJsonVector3("attackVector", attackVector, data);
		data["hitSE"] = hitSoundParameter.Save();
		data["damage"] = atkDamage;
		data["level"] = atkLevel;

		return data;
	}

	void AttackInformation::Load(const json& _data)
	{
		json loadData;
		// ヒットエフェクト
		if (LoadJsonDataArray("hitVfxDatas", loadData, _data))
		{
			u_int vfxCnt = static_cast<u_int>(loadData.size());
			hitVfxInfoList.resize(vfxCnt);
			u_int loop = 0;
			for (auto& vfx : hitVfxInfoList)
			{
				vfx.Load(loadData[loop]);
				loop++;
			}
		}

		LoadJsonBoolean("isCamShake", isCamShake, _data);
		if (isCamShake)
		{
			if (LoadJsonData("camShake", loadData, _data))
				pCamShakeParam.Load(loadData);
		}

		LoadJsonVector3("attackVector", attackVector, _data);
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

		// 攻撃ベクトル
		ImGui::DragFloat3("Vector", &attackVector.x, 0.01f, -1.0f, 1.0f);

		// エフェクト編集
		ImGuiHitVfx();

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

	void AttackInformation::ImGuiHitVfx()
	{
		if (!ImGuiMethod::TreeNode("Hit Vfx")) return;

		// ヒットエフェクト編集する
		u_int vfxCnt = hitVfxInfoList.size();
		u_int loop = 0;
		for (auto itr = hitVfxInfoList.begin(); itr != hitVfxInfoList.end();)
		{
			// 削除したか
			bool isDelete = false;
			if (ImGuiMethod::TreeNode(std::to_string(loop)))
			{
				isDelete = ImGui::Button("X");	// 削除ボタン
				itr->ImGuiCall();
				ImGui::TreePop();
			}

			// 削除するなら
			if (isDelete)
			{
				itr = hitVfxInfoList.erase(itr);
				continue;
			}

			// 次に進める
			++itr;
			loop++;
		}

		// エフェクト追加
		if (ImGui::Button("+"))
		{
			// 追加
			hitVfxInfoList.resize(std::min(vfxCnt + 1, MAX_RESIST_HITVFX));
		}

		ImGui::TreePop();
	}
}