#include "pch.h"
#include "PerlinShakeParameter.h"

namespace HashiTaku
{
	PerlinShakeParameter::PerlinShakeParameter() : 
		power(1.0f),
		minPowerRatio(0.0f),
		time(0.3f),
		speed(1.0f),
		isFadeOut(false)
	{
	}

	json PerlinShakeParameter::Save()
	{
		json shakeData;

		SaveJsonVector3("shakeVec", shakeVec, shakeData);
		shakeData["power"] = power;
		shakeData["time"] = time;
		shakeData["speed"] = speed;
		shakeData["fadeOut"] = isFadeOut;

		return shakeData;
	}

	void PerlinShakeParameter::Load(const json& _data)
	{
		LoadJsonVector3("shakeVec", shakeVec, _data);
		LoadJsonFloat("power", power, _data);
		LoadJsonFloat("time", time, _data);
		LoadJsonFloat("speed", speed, _data);
		LoadJsonBoolean("fadeOut", isFadeOut, _data);
	}

	void PerlinShakeParameter::ImGuiDebug()
	{
		if (!ImGuiMethod::TreeNode("Shake Parameter")) return;

		ImGui::Checkbox("FadeOut", &isFadeOut);
		ImGui::DragFloat3("Vector", &shakeVec.x, 0.01f);
		ImGui::DragFloat("Power", &power, 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat("Time", &time, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("Speed", &speed, 0.01f, 0.0f, 100.0f);

		ImGui::TreePop();
	}
}
