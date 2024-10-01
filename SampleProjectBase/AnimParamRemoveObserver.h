#pragma once
#include "IObserever.h"

class AnimTransitionArrow;
class BlendAnimationNode;

namespace HashiTaku
{
	namespace AnimParam
	{
		enum class AnimParamObseveEvent
		{
			Remove,	// �폜
			Rename	// ���O�ύX
		};

		struct NotificationData
		{
			/// @brief �C�x���g�̎��
			AnimParamObseveEvent eventType = AnimParamObseveEvent::Remove;

			/// @brief �폜���閼�O
			const std::string* pDeleteName{ nullptr };

			/// @brief �ύX�ꖼ�O
			const std::string* pAfterName{ nullptr };
		};

		/// @brief �C���^�[�t�F�[�X
		class IAnimParamRelater
		{
		public:
			IAnimParamRelater() {}
			virtual ~IAnimParamRelater() {}

			/// @brief �ύX���e���󂯂Ƃ�A�������s��
			/// @param _data �ʒm�f�[�^
			virtual void AcceptAnimParamData(const NotificationData& _notifyData) = 0;
		};

		/// @brief �A�j���[�V�����p�����[�^�ɕύX���������ꍇ�̖��̃I�u�U�[�o�[�p�^�[��
		class AnimParamObserver : public HashiTaku::IObserver<NotificationData>
		{
			/// @brief �I�u�U�[�o�[������������
			IAnimParamRelater* pInterface;
		public:
			AnimParamObserver(IAnimParamRelater& _relater, const std::string& _observerName);
			~AnimParamObserver() {}

			/// @brief �ʒm�������Ƃ��̏���
			/// @param _notifyData �ʒm�f�[�^
			void Update(const NotificationData& _notifyData) override;
		};
	}
}



