#include "pch.h"
#include "CP_Fade.h"
#include "GameObject.h"

namespace HashiTaku
{
	CP_Fade::CP_Fade() :
		pUIRenderer(nullptr),
		state(State::Wait),
		settingFadeTime(1.0f),
		fadeSpeed(1.0f),
		targetAlpha(0.0f),
		fadeCloseTime(1.0f),
		fadeOpenTime(1.0f),
		isStartOpen(false)
	{
	}

	void CP_Fade::OpenFade()
	{
		OpenFade(fadeOpenTime);
	}

	void CP_Fade::OpenFade(float _openTime, float _targetAlpha)
	{
		settingFadeTime = std::max(_openTime, Mathf::epsilon);
		targetAlpha = std::clamp(_targetAlpha, 0.0f, 1.0f);
		fadeSpeed = (targetAlpha - 1.0f) / settingFadeTime;
		state = State::Open;

		// 最初に黒くする
		if (pUIRenderer)
		{
			pUIRenderer->SetAlpha(1.0f);
		}
	}

	void CP_Fade::CloseFade()
	{
		CloseFade(fadeCloseTime);
	}

	void CP_Fade::CloseFade(float _closeTime, float _targetAlpha)
	{
		settingFadeTime = std::max(_closeTime, Mathf::epsilon);
		targetAlpha = std::clamp(_targetAlpha, 0.0f, 1.0f);
		fadeSpeed = (targetAlpha - 0.0f) / settingFadeTime;
		state = State::Close;

		// 最初に黒くする
		if (pUIRenderer)
		{
			pUIRenderer->SetAlpha(0.0f);
		}
	}

	void CP_Fade::SetOnEndFunction(const std::function<void()>& _func)
	{
		endFunction = _func;
	}

	json CP_Fade::Save()
	{
		auto data = Component::Save();

		data["startOpen"] = isStartOpen;
		data["openTime"] = fadeOpenTime;
		data["closeTime"] = fadeCloseTime;

		return data;
	}

	void CP_Fade::Load(const json& _data)
	{
		Component::Load(_data);

		LoadJsonBoolean("startOpen", isStartOpen, _data);
		LoadJsonFloat("openTime", fadeOpenTime, _data);
		LoadJsonFloat("closeTime", fadeCloseTime, _data);
	}

	void CP_Fade::Awake()
	{
		pUIRenderer = GetGameObject().GetComponent<CP_UIRenderer>();

		// シーン開始時にフェードを開ける
		if (isStartOpen)
			OpenFade();
	}

	void CP_Fade::Update()
	{
		if (!GetCanUpdate()) return;

		switch (state)
		{
		case State::Open:
			OpenUpdate();
			break;

		case State::Close:
			CloseUpdate();
			break;
		}
	}

	void CP_Fade::CloseUpdate()
	{
		float alpha = pUIRenderer->GetAlpha();
		alpha += fadeSpeed * DeltaTime();
		pUIRenderer->SetAlpha(alpha);

		if (alpha > targetAlpha)	// 終わったら
		{
			state = State::Wait;
			ExecEndFunction();
		}
	}

	void CP_Fade::CalcFadeSpeed()
	{
	}

	void CP_Fade::OpenUpdate()
	{
		float alpha = pUIRenderer->GetAlpha();
		alpha += fadeSpeed * DeltaTime();
		pUIRenderer->SetAlpha(alpha);

		if (alpha < targetAlpha)	// 終わったら
		{
			state = State::Wait;
			ExecEndFunction();
		}
	}

	bool CP_Fade::GetCanUpdate() const
	{
		if (state == State::Wait) return false;
		if (!pUIRenderer)
		{
			HASHI_DEBUG_LOG("UIRendererがありません");
			return false;
		}
		return true;
	}

	void CP_Fade::ExecEndFunction()
	{
		if (!endFunction) return;

		endFunction();
		endFunction = nullptr;
	}

	void CP_Fade::ImGuiDebug()
	{
		ImGui::Checkbox("StartOpen", &isStartOpen);
		ImGui::DragFloat("OpenTime", &fadeOpenTime, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("CloseTime", &fadeCloseTime, 0.01f, 0.0f, 100.0f);
	}
}