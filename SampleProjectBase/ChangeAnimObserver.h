#pragma once
#include "IObserever.h"

namespace HashiTaku
{
	/// @brief �A�j���[�V�����ύX�Ɋւ�����
	struct ChangeAnimationInfo
	{
		/// @brief �J�ڌ��A�j���[�V�����m�[�h��
		const std::string* pFromAnimNodeName;

		/// @brief �J�ڐ�A�j���[�V�����m�[�h��
		const std::string* pToAnimNodeName;
	};

	/// @brief �A�j���[�V�����ύX��ʒm�����I�u�U�[�o�[
	class ChangeAnimObserver : public IObserver<ChangeAnimationInfo>
	{

	public:
		ChangeAnimObserver(const std::string& _observerName);
		virtual ~ChangeAnimObserver() {}

		/// @brief �ʒm�������Ƃ��̏���
		virtual void ObserverUpdate(const ChangeAnimationInfo& _value) = 0;
	};

	/// @brief �ύX�ʒm����T�u�W�F�N�g
	using ChangeAnimSubject = Subject<ChangeAnimationInfo>;
}



