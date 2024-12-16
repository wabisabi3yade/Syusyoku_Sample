#include "pch.h"
#include "AnimationNotifyFactory.h"
#include "AnimNotifyDefine.h"

namespace HashiTaku
{
	bool AnimationNotifyFactory::ImGuiCombo(std::unique_ptr<AnimationNotify_Base>& _pCreateNotify)
	{
		static u_int selectType;

		// �^�C�v�̕�������擾����
		std::vector<std::string> nameList;
		for (auto& respawner : animNotifyList)
		{
			nameList.push_back(AnimationNotify_Base::GetTypeToStr(respawner.first));
		}

		if (nameList.empty()) return false;

		if (ImGui::Button("+"))
		{
			_pCreateNotify = Create(
				static_cast<AnimationNotify_Base::NotifyType>(selectType));
			return true;
		}

		ImGui::SameLine();
		ImGuiMethod::ComboBox("AddNotify", selectType, nameList);

		return false;
	}

	AnimationNotifyFactory::AnimationNotifyFactory(AnimationParameters& _animationParameters)
		: pAnimationParameters(&_animationParameters)
	{
		Init();	// ����������
	}

	std::unique_ptr<AnimationNotify_Base> AnimationNotifyFactory::Create(AnimationNotify_Base::NotifyType _notifyType)
	{
		// ��ނ̕�����
		std::string notifyTypeStr =
			AnimationNotify_Base::GetTypeToStr(_notifyType);

#ifdef _DEBUG
		if (!animNotifyList.contains(_notifyType))
		{
			HASHI_DEBUG_LOG(notifyTypeStr + "���������܂���ł���");
			return nullptr;
		}
#endif // _DEBUG

		// �쐬
		std::unique_ptr<AnimationNotify_Base> pCreate =
			animNotifyList[_notifyType]->Create();
		pCreate->SetNotifyName(notifyTypeStr);

		// �A�j���[�V�����p�����[�^���Z�b�g����K�v������Ȃ�
		if (IAnimParametersSetter* pParamSetter = dynamic_cast<IAnimParametersSetter*>(pCreate.get()))
		{
			pParamSetter->SetAnimationParameters(*pAnimationParameters);
		}

		return std::move(pCreate);
	}

	void AnimationNotifyFactory::Init()
	{
		ResisterNotify<ANE_DebugLog>(AnimationNotify_Base::NotifyType::ANE_DebugLog);
		ResisterNotify<ANS_DebugLog>(AnimationNotify_Base::NotifyType::ANS_DebugLog);
		ResisterNotify<ANE_ChangeParameter>(AnimationNotify_Base::NotifyType::ANE_ChangeParameter);
		ResisterNotify<ANS_ChangeParameter>(AnimationNotify_Base::NotifyType::ANS_ChangeParameter);
	}
}