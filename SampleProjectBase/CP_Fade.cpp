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
		// �h��Ԃ�����J�n����
		if (pUIRenderer)
			pUIRenderer->SetAlpha(1.0f);

		OpenFade(fadeOpenTime);
	}

	void CP_Fade::OpenFade(float _openTime, float _targetAlpha)
	{
		// �J���鎞�Ԃ�0�b�Ȃ璼���ɔ��f
		if (_openTime < Mathf::epsilon)
		{
			if (pUIRenderer)
				pUIRenderer->SetAlpha(_targetAlpha);

			return;
		}

		// ���݂̃��l���擾
		float curAlpha = 0.0f;
		if (pUIRenderer)
			curAlpha = pUIRenderer->GetAlpha();

		// ���݂̃��l����^�[�Q�b�g�܂ł̑��x�����߂�
		settingFadeTime = std::max(_openTime, Mathf::epsilon);
		targetAlpha = std::clamp(_targetAlpha, 0.0f, 1.0f);
		fadeSpeed = (targetAlpha - curAlpha) / settingFadeTime;
		state = State::Open;
	}

	void CP_Fade::CloseFade()
	{
		// ��������J�n����
		if (pUIRenderer)
			pUIRenderer->SetAlpha(0.0f);

		CloseFade(fadeCloseTime);
	}

	void CP_Fade::CloseFade(float _closeTime, float _targetAlpha)
	{
		// ���鎞�Ԃ�0�b�Ȃ璼���ɔ��f
		if (_closeTime < Mathf::epsilon)
		{
			if (pUIRenderer)
				pUIRenderer->SetAlpha(_targetAlpha);

			return;
		}

		// ���݂̃��l���擾
		float curAlpha = 0.0f;
		if(pUIRenderer)
			curAlpha = pUIRenderer->GetAlpha();

		// ���݂̃��l����^�[�Q�b�g�܂ł̑��x�����߂�
		settingFadeTime = std::max(_closeTime, Mathf::epsilon);
		targetAlpha = std::clamp(_targetAlpha, 0.0f, 1.0f);
		fadeSpeed = (targetAlpha - curAlpha) / settingFadeTime;
		state = State::Close;
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

		// �V�[���J�n���Ƀt�F�[�h���J����
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
		alpha += fadeSpeed * MainApplication::DeltaTime();
		pUIRenderer->SetAlpha(alpha);

		if (alpha > targetAlpha)	// �I�������
		{
			state = State::Wait;
			ExecEndFunction();
		}
	}

	void CP_Fade::OpenUpdate()
	{
		float alpha = pUIRenderer->GetAlpha();
		// �V�[���̃^�C���X�P�[���Ɋւ��Ȃ�
		alpha += fadeSpeed * MainApplication::DeltaTime();	
		pUIRenderer->SetAlpha(alpha);

		if (alpha < targetAlpha)	// �I�������
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
			HASHI_DEBUG_LOG("UIRenderer������܂���");
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