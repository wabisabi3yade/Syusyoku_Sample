#include "pch.h"
#include "CameraShakeParameter.h"

namespace HashiTaku
{
	nlohmann::json CameraShakeParameter::Save()
	{
		json shakeData;

		SaveJsonVector3("shakeVec", shakeVec, shakeData);
		shakeData["power"] = power;
		shakeData["time"] = time;
		shakeData["speed"] = speed;
		shakeData["fadeOut"] = isFadeOut;

		return shakeData;
	}

	void CameraShakeParameter::Load(const nlohmann::json& _data)
	{
		LoadJsonVector3("shakeVec", shakeVec, _data);
		LoadJsonFloat("power", power, _data);
		LoadJsonFloat("time", time, _data);
		LoadJsonFloat("speed", speed, _data);
		LoadJsonBoolean("fadeOut", isFadeOut, _data);
	}

	void CameraShakeParameter::ImGuiDebug()
	{
		if (!ImGuiMethod::TreeNode("CamShake")) return;

		ImGui::Checkbox("FadeOut", &isFadeOut);
		ImGui::DragFloat3("Vector", &shakeVec.x, 0.01f);
		ImGui::DragFloat("Power", &power, 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat("Time", &time, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("Speed", &speed, 0.01f, 0.0f, 100.0f);

		ImGui::TreePop();
	}
}
