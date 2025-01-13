#pragma once
#include "TransCondition_Base.h"
#include "InterpolateKind.h"
#include "AnimationParameters.h"
#include "AnimParamRemoveObserver.h"

namespace HashiTaku
{
	class AnimationNode_Base;
	namespace AnimParam
	{
		struct NotificationData;
	}

	/// @brief �A�j���[�V�����J�ڂ̏�������ݒ肷����
	class AnimTransitionArrow :
		public IImGuiUser,
		public ISaveLoad, 
		public AnimParam::IAnimParamRelater
	{
		/// @brief �J�ڏ������i�[���郊�X�g
		std::list<std::unique_ptr<TransCondition_Base>> conditionList;

		/// @brief �p�����[�^�폜���̒ʒm�󂯎��
		std::unique_ptr<AnimParam::AnimParamObserver> pRemoveParamObserver;

		/// @brief	�J�ڐ�A�j���m�[�h
		AnimationNode_Base* pToNode;

		/// @brief �R���g���[���[���ϐ�
		AnimationParameters* pAnimParameters;

		/// @brief �J�ڂ������߂�Ƃ��ɗ��p����C�[�W���O
		EaseKind transitiionEase;

		/// @brief �A�j���[�V�����ԂŎg�p�����ԕ��@
		AnimInterpolateKind interpolateKind;

		/// @brief �J�ڊJ�n����
		float exitRatio;

		/// @brief �J�ڐ�̃A�j���[�V�����̎w�芄��
		float transTargetRatio;

		/// @brief �J�ڎ���
		float transitionTime;

		/// @brief �����`�F�b�N����Ƃ��̗D�揇��
		int priority;

		/// @brief �J�ڊJ�n���Ԃł����J�n���Ȃ��悤�ɂ���
		bool isHasExitRatio;

		/// @brief �������邩�H
		bool isActive;
#ifdef EDIT
		// �I�𒆂̖��O
		std::string selectParamName;
#endif
	public:
		/// @brief �R���X�g���N�^
		/// @param _fromNode �J�ڑO
		/// @param _toNode �J�ڐ�
		/// @param _animParameters �R���g���[���[���ϐ�
		AnimTransitionArrow(AnimationNode_Base& _toNode, AnimationParameters& _animParameters);
		virtual ~AnimTransitionArrow() {}

		/// @brief ���ɑJ�ڏ������쐬
		/// @param _val �Q�Ƃ���p�����[�^�l
		/// @param _name �p�����[�^��
		void AddCondition(const AnimParam::conditionValType& _val, const std::string& _name);

		/// @brief �p�����[�^������J�ڏ������폜����
		/// @param _condParamName �J�ڏ������Q�Ƃ��Ă���p�����[�^��
		void RemoveCondition(const std::string& _condParamName);

		/// @brief �������Q�Ƃ��Ă��閼�O��ς���
		/// @param _prevName �O�̖��O
		/// @param _afterName �ύX��̖��O
		void ReNameCondition(const std::string& _prevName, const std::string& _afterName);

		// �J�ڏI�����̑J�ڐ�̃A�j���[�V�����������Z�b�g
		void SetTransTargetRatio(float _transTargetRatio);

		// �J�ڂɂ����鎞�Ԃ��Z�b�g
		void SetTransitonTime(float _transTime);

		/// @brief �����`�F�b�N����Ƃ��̗D�揇�ʂ��擾
		/// @param _priority �D�揇��
		void SetPriority(int _priority);

		// �C�[�W���O�̎�ނ��Z�b�g
		void SetEaseKind(EaseKind _easeKind);

		// �J�ڂ̕�Ԏ�ނ��Z�b�g
		void SeInterpolateKind(AnimInterpolateKind _interpolateKind);

		// �J�ڐ�̃m�[�h���擾����
		AnimationNode_Base& GetToNode() const;

		/// @brief �J�ڐ�̊J�n�������擾
		/// @return �J�ڐ�̊J�n����
		float GetTargetRatio() const;

		/// @brief �J�ڎ��Ԃ��擾
		/// @return �J�ڎ���
		float GetTransitionTime() const;

		/// @brief �J�ڌ��̏I���������擾
		/// @return �J�ڌ��̏I������
		float GetExitRatio() const;

		/// @brief �D�揇�ʂ��擾
		/// @return �D�揇��
		int GetPriority() const;

		/// @brief �J�ڏI�����Ԃ������Ă��邩�擾
		/// @return �J�ڏI�����Ԃ����Ă��邩�H
		bool GetIsHasExit() const;

		/// @brief �������X�g���擾
		/// @return �������X�g
		const std::list<std::unique_ptr<TransCondition_Base>>& GetConditionList() const;

		// �C�[�W���O�̎�ނ��擾
		EaseKind GetEaseKind() const;

		// ��Ԃ̎�ނ��擾
		AnimInterpolateKind GetInterpolateKind() const;

		/// @brief ������Ԃ��擾
		/// @return ������Ԃł��邩�H
		bool GetIsActive() const;

		/// @brief �Z�[�u����
		/// @return �Z�[�u�f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����f�[�^ 
		void Load(const json& _data) override;

		void AcceptAnimParamData(const AnimParam::NotificationData& _notifyData);
	private:
		// ImGui�ŕ�Ԃ̎�ނ�ҏW
		void ImGuiSetInterpolate();

		// �J�ڏ�����ҏW
		void ImGuiSetCondistion();

	protected:
		void ImGuiDebug() override;
	};
}