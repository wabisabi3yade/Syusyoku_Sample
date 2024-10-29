#pragma once

namespace HashiTaku
{
	class StateNode_Base;

	/// @brief �X�e�[�g�}�V�����
	/// @tparam T �m�[�h��o�^����Ƃ��̃L�[�̌^
	template<class T>
	class StateMachine_Base
	{
		/// @brief �X�e�[�g�}�V����
		std::string stateMachineName;

	protected:
		/// @brief ��ԃm�[�h���X�g
		std::unordered_map<T, std::unique_ptr<StateNode_Base>> stateNodeList;

		/// @brief ���݂̃m�[�h�̃L�[
		T currentStateKey;

		/// @brief ���݂̃m�[�h
		StateNode_Base* pCurrentNode;

		/// @brief �f�t�H���g�m�[�h
		StateNode_Base* pDefaultNode;
	public:
		StateMachine_Base(const std::string& _stateMachineName);
		virtual ~StateMachine_Base() {}

		/// @brief �m�[�h��ǉ�����
		/// @param _registKey �ǉ�����L�[
		/// @param _addNode �ǉ�����m�[�h
		virtual void AddNode(const T& _registKey, std::unique_ptr<StateNode_Base> _addNode);

		/// @brief �m�[�h���폜����
		/// @param _registKey 
		virtual void RemoveNode(const T& _registKey);

		/// @brief �m�[�h��ύX����
		/// @param _changeKey �ύX����m�[�h�̃L�[
		virtual void ChangeNode(const T& _changeKey);

		/// @brief �J�n����
		virtual void Begin();

		/// @brief �f�t�H���g�m�[�h��ݒ肷��
		/// @param _defaultKey �f�t�H���g�m�[�h���w�肷��L�[
		void SetDefaultNode(const T& _defaultKey);
	};

	/// @brief �X�e�[�g�}�V���Ŏg�p����m�[�h
	class StateNode_Base
	{

	public:
		StateNode_Base() {}
		virtual ~StateNode_Base() {}

		/// @brief ��Ԑ؂�ւ��J�n����
		virtual void OnStart() {}

		/// @brief �X�V����
		virtual void Update() {}

		/// @brief ��Ԑ؂�ւ��I������
		virtual void OnEnd() {}
	};

	/// @brief �A�j���[�V����������X�e�[�g�}�V���Ŏg�p����m�[�h
	class StateNode_AnimationBase : public StateNode_Base
	{

	public:
		StateNode_AnimationBase() {}
		virtual ~StateNode_AnimationBase() {}

		/// @brief �A�j���[�V�����I�����̍s��
		/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
		/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
		virtual void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) {}
	};

	template<class T>
	inline StateMachine_Base<T>::StateMachine_Base(const std::string& _stateMachineName)
		: stateMachineName(_stateMachineName), pCurrentNode(nullptr), pDefaultNode(nullptr)
	{
	}

	template<class T>
	inline void StateMachine_Base<T>::AddNode(const T& _registKey, std::unique_ptr<StateNode_Base> _addNode)
	{
		if (stateNodeList.contains(_registKey))
		{
			HASHI_DEBUG_LOG(stateMachineName + "���ɂ���̂Œǉ��ł��܂���");
			return;
		}

		stateNodeList[_registKey] = std::move(_addNode);
	}

	template<class T>
	inline void StateMachine_Base<T>::RemoveNode(const T& _registKey)
	{
		if (!stateNodeList.contains(_registKey))
		{
			HASHI_DEBUG_LOG(stateMachineName + "���X�g���ɖ����̂ō폜�ł��܂���");
			return;
		}

		StateNode_Base& removeNode = *stateNodeList[_registKey];
		
		if (pDefaultNode == &removeNode)
			pDefaultNode = nullptr;

		if (pCurrentNode == &removeNode)
			pCurrentNode = nullptr;

		stateNodeList.erase(_registKey);
	}

	template<class T>
	inline void StateMachine_Base<T>::ChangeNode(const T& _changeKey)
	{
		if (!stateNodeList.contains(_changeKey))
		{
			HASHI_DEBUG_LOG(stateMachineName + "�w�肵���L�[���Ȃ��̂ŕύX�ł��܂���");
			return;
		}

		// �ύX�O�̏I������
		pCurrentNode->OnEnd();

		pCurrentNode = stateNodeList[_changeKey].get();
		currentStateKey = _changeKey;

		// �ύX��̊J�n����
		pCurrentNode->OnStart();
	}

	template<class T>
	inline void StateMachine_Base<T>::Begin()
	{
		pCurrentNode = pDefaultNode;
	}

	template<class T>
	inline void StateMachine_Base<T>::SetDefaultNode(const T& _defaultKey)
	{
		if (!stateNodeList.contains(_defaultKey))
		{
			HASHI_DEBUG_LOG(stateMachineName + "�Ńf�t�H���g�m�[�h���Z�b�g�ł��܂���ł���");
			return;
		}

		pDefaultNode = stateNodeList[_defaultKey].get();
	}
}

