#pragma once

namespace HashiTaku
{
	class StateNode_Base;

	/// @brief ステートマシン基底
	/// @tparam T ノードを登録するときのキーの型
	template<class T>
	class StateMachine_Base
	{
		/// @brief ステートマシン名
		std::string stateMachineName;

	protected:
		/// @brief 状態ノードリスト
		std::unordered_map<T, std::unique_ptr<StateNode_Base>> stateNodeList;

		/// @brief 現在のノードのキー
		T currentStateKey;

		/// @brief 現在のノード
		StateNode_Base* pCurrentNode;

		/// @brief デフォルトノード
		StateNode_Base* pDefaultNode;
	public:
		StateMachine_Base(const std::string& _stateMachineName);
		virtual ~StateMachine_Base() {}

		/// @brief ノードを追加する
		/// @param _registKey 追加するキー
		/// @param _addNode 追加するノード
		virtual void AddNode(const T& _registKey, std::unique_ptr<StateNode_Base> _addNode);

		/// @brief ノードを削除する
		/// @param _registKey 
		virtual void RemoveNode(const T& _registKey);

		/// @brief ノードを変更する
		/// @param _changeKey 変更するノードのキー
		virtual void ChangeNode(const T& _changeKey);

		/// @brief 開始処理
		virtual void Begin();

		/// @brief デフォルトノードを設定する
		/// @param _defaultKey デフォルトノードを指定するキー
		void SetDefaultNode(const T& _defaultKey);
	};

	/// @brief ステートマシンで使用するノード
	class StateNode_Base
	{

	public:
		StateNode_Base() {}
		virtual ~StateNode_Base() {}

		/// @brief 状態切り替え開始処理
		virtual void OnStart() {}

		/// @brief 更新処理
		virtual void Update() {}

		/// @brief 状態切り替え終了処理
		virtual void OnEnd() {}
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
			HASHI_DEBUG_LOG(stateMachineName + "既にあるので追加できません");
			return;
		}

		stateNodeList[_registKey] = std::move(_addNode);
	}

	template<class T>
	inline void StateMachine_Base<T>::RemoveNode(const T& _registKey)
	{
		if (!stateNodeList.contains(_registKey))
		{
			HASHI_DEBUG_LOG(stateMachineName + "リスト内に無いので削除できません");
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
			HASHI_DEBUG_LOG(stateMachineName + "指定したキーがないので変更できません");
			return;
		}

		// 変更前の終了処理
		pCurrentNode->OnEnd();

		pCurrentNode = stateNodeList[_changeKey].get();
		currentStateKey = _changeKey;

		// 変更後の開始処理
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
			HASHI_DEBUG_LOG(stateMachineName + "でデフォルトノードをセットできませんでした");
			return;
		}

		pDefaultNode = stateNodeList[_defaultKey].get();
	}
}

