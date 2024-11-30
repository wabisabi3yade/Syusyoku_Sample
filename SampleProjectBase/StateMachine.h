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

		/// @brief ���̃t���[���ł͏�Ԃ�
		bool isCurFrameStateChange;

#ifdef EDIT
		T prevStateKey;	// �f�o�b�O�p�O��̏��
#endif 
	protected:
		/// @brief ��ԃm�[�h���X�g
		std::unordered_map<T, std::unique_ptr<StateNode_Base>> stateNodeList;

		/// @brief ���݂̃m�[�h�̃L�[
		T currentStateKey;

		/// @brief ���݂̃m�[�h
		StateNode_Base* pCurrentNode;

		/// @brief �f�t�H���g�m�[�h
		StateNode_Base* pDefaultNode;

		/// @brief ���̃t���[���őJ�ڂ͍s�����H
		bool isChangeCurFlame;
	public:
		StateMachine_Base(const std::string& _stateMachineName);
		virtual ~StateMachine_Base() {}

		/// @brief �X�V�֐��Ăяo��
		void UpdateCall();

		/// @brief �m�[�h��ǉ�����
		/// @param _registKey �ǉ�����L�[
		/// @param _addNode �ǉ�����m�[�h
		void AddNode(const T& _registKey, std::unique_ptr<StateNode_Base> _addNode);

		/// @brief �m�[�h���폜����
		/// @param _registKey 
		void RemoveNode(const T& _registKey);

		/// @brief �m�[�h��ύX����
		/// @param _changeKey �ύX����m�[�h�̃L�[
		/// @param _isForceChange �����I�ɕύX����
		/// @return �J�ڐ����������H
		bool ChangeNode(const T& _changeKey, bool _isForceChange = false);

		/// @brief �f�t�H���g�m�[�h��ݒ肷��
		/// @param _defaultKey �f�t�H���g�m�[�h���w�肷��L�[
		void SetDefaultNode(const T& _defaultKey);

		/// @brief ���݂̃m�[�h���擾
		/// @return ���݂̃m�[�h
		StateNode_Base* GetCurrentNode();

	protected:
		/// @brief �J�n����
		void Begin();

		/// @brief �X�V����
		virtual void Update();

		/// @brief �O��̃X�e�[�g���擾����
		/// @param _outState �O��̃X�e�[�g���󂯎��ϐ�
		void DebugGetPrevState(T& _outState);
	private:
		/// @brief �m�[�h�ύX�ł�������𐮂��Ă��邩�H
		/// @param _changeKey �J�ڐ�L�[
		/// @return �ύX�ł��邩�H
		bool CanChengeNode(const T& _changeKey);
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
	inline void StateMachine_Base<T>::UpdateCall()
	{
		// �O������̃R�[���o�b�N�ŕύX���Ă��邩���Ȃ̂Ń��Z�b�g
		isChangeCurFlame = false;

		// �X�V
		Update();

		// ���Z�b�g
		isChangeCurFlame = false;
	}

	template<class T>
	inline void StateMachine_Base<T>::AddNode(const T& _registKey, std::unique_ptr<StateNode_Base> _addNode)
	{
		if (stateNodeList.contains(_registKey))
		{
			HASHI_DEBUG_LOG(stateMachineName + "�Œǉ�����L�[�����ɂ���̂Œǉ��ł��܂���");
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
	inline bool StateMachine_Base<T>::ChangeNode(const T& _changeKey, bool _isForceChange)
	{
		// �J�ڂł��邩�m�F����
		if (!_isForceChange && !CanChengeNode(_changeKey))
			return false;

		if (!stateNodeList.contains(_changeKey))
		{
			HASHI_DEBUG_LOG(stateMachineName + "�w�肵���L�[���Ȃ��̂ŕύX�ł��܂���");
			return false;
		}

		// �ύX�O�̏I������
		pCurrentNode->OnEnd();


#ifdef EDIT
		prevStateKey = currentStateKey;	// �O��̃L�[�ɕۑ�
#endif

		// ���݂̃m�[�h��ύX
		pCurrentNode = stateNodeList[_changeKey].get();
		currentStateKey = _changeKey;

		// �ύX��̊J�n����
		pCurrentNode->OnStart();

		// �ύX�����t���O�𗧂Ă�
		isChangeCurFlame = true;

		return true;
	}

	template<class T>
	inline void StateMachine_Base<T>::Begin()
	{
		pCurrentNode = pDefaultNode;

		pCurrentNode->OnStart();
	}

	template<class T>
	inline bool StateMachine_Base<T>::CanChengeNode(const T& _changeKey)
	{
		// ���ɂ��̃t���[�����ɕύX����Ă���Ȃ�
		if (isChangeCurFlame) return false;

		return true;
	}

	template<class T>
	inline void StateMachine_Base<T>::Update()
	{
		pCurrentNode->Update();
	}

	template<class T>
	inline void StateMachine_Base<T>::DebugGetPrevState(T& _outState)
	{
#ifdef EDIT
		_outState = prevStateKey;
#endif // EDIT
	}

	template<class T>
	inline void StateMachine_Base<T>::SetDefaultNode(const T& _defaultKey)
	{
		if (!stateNodeList.contains(_defaultKey))
		{
			HASHI_DEBUG_LOG(stateMachineName + "�Ńf�t�H���g�m�[�h���Z�b�g�ł��܂���ł���");
			return;
		}

		currentStateKey = _defaultKey;
		pDefaultNode = stateNodeList[_defaultKey].get();

#ifdef EDIT
		prevStateKey = currentStateKey;
#endif // EDIT
	}
	template<class T>
	inline StateNode_Base* StateMachine_Base<T>::GetCurrentNode()
	{
		return pCurrentNode;
	}
}

