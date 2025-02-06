#pragma once
#include "StateMachine.h"
#include "IActionController.h"

#ifdef EDIT
#include "Geometory.h"
#endif // EDIT

namespace HashiTaku
{
	// �A�N�V�����R���g���[���[�h���������̃R���Z�v�g
	template<typename T>
	concept ActionControllerConcept = std::is_base_of_v<IActionController, T>;

	/// @brief �L�����N�^�̃A�N�V�����X�e�[�g���
	class CharacterActState_Base :
		public StateNode_Base,
		public IImGuiUser,
		public ISaveLoad
	{
		/// @brief �L�����N�^�[�̃A�N�V�����R���g���[���[
		IActionController* pActionController;
	public:
		CharacterActState_Base();
		virtual ~CharacterActState_Base() {}

		/// @brief ������
		/// @param _actionController �A�N�V�����R���g���[���[�C���^�[�t�F�[�X
		void Init(IActionController& _actionController);

		/// @brief �A�j���[�V�����I�����̍s��
		/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
		/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
		virtual void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
		{}

		/// @brief �f�o�b�O���ŕ`�悵�����Ƃ��̏���
		virtual void DebugDisplay() {}

		virtual json Save() = 0;
		virtual void Load(const json& _data) = 0;
	protected:
		/// @brief ���x���Z�b�g
		/// @param _velocity ���x
		void SetVelocity(const DXSimp::Vector3& _velocity);

		/// @brief �I�u�W�F�N�g�̃�t���擾
		/// @return ��t
		float DeltaTime() const;

		/// @brief �A�N�V�����R���g���[���[�̃C���^�[�t�F�[�X���擾
		/// @return �A�N�V�����R���g���[���[
		IActionController& GetActionController();

		/// @brief ���g�̃g�����X�t�H�[�����擾����
		/// @return ���g�̃g�����X�t�H�[��
		Transform& GetMyTransform();

		/// @brief �w�肵���A�N�V�����R���g���[���[���擾����
		/// @tparam T �A�N�V�����R���g���[���[�̎��
		/// @return �擾����A�N�V�����R���g���[���[
		template<ActionControllerConcept T>
		T& GetDeliverActionController();

		virtual void ImGuiDebug() = 0;
	};

	template<ActionControllerConcept T>
	inline T& CharacterActState_Base::GetDeliverActionController()
	{
#ifdef EDIT
		T* pRetCon = dynamic_cast<T*>(pActionController);
		assert(pRetCon && "����ɃA�N�V�����R���g���[���[��ϊ��ł��܂���ł���");

		return *pRetCon;
#endif // EDIT

		return static_cast<T&>(*pActionController);
	}
}