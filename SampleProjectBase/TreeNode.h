#pragma once

/// @brief ノード内のパラメータベース
class NodeParam_Base
{
	
public:
	NodeParam_Base() {}
	virtual ~NodeParam_Base() {}
};

/// @brief ノード内のパラメータ
/// @tparam T パラメータの型名
template<class T>
class NodeParam : NodeParam_Base
{
	/// @brief ノード内にあるパラメータ
	T* parameter;

public:
	NodeParam() {}
	~NodeParam() {}

	/// @brief パラメータをセット
	/// @param _parameter パラメータ
	void SetParameter(T* _parameter);

	/// @brief ノードのパラメータを取得
	/// @return パラメータ
	T* GetParameter();
};

/// @brief 親子関係での1ノード
/// @tparam T 親子を指すクラス
class TreeNode
{
	/// @brief ノード名
	std::string nodeName;

	/// @brief このノードが持つパラメータ
	std::unique_ptr<NodeParam_Base> pHaveParameter;

	/// @brief 親ノード
	TreeNode* pParentNode;

	/// @brief 子ノード
	std::list<TreeNode*>  pChildNodes;

	/// @brief ID
	u_int idx;

	/// @brief パラメータが設定されているか
	bool isHaveParam;

public:
	TreeNode() : nodeName(""), pParentNode(nullptr), idx(-1) {}
	~TreeNode() {}

	/// @brief 親ノードをセット
	/// @param _parentNode 親ノード
	void SetParent(TreeNode& _parentNode);

	/// @brief 子供ノードを追加
	/// @param _chiledNode 子ノード
	void AddChiled(TreeNode& _chiledNode);

	/// @brief 子供ノードを除外
	/// @param _chiledNode 子ノード
	void RemoveChiled(TreeNode& _chiledNode);

	/// @brief 初期処理
	/// @tparam T 持たせるパラメータの型
	/// @param _parameter パラメータ
	/// @param _nodeName 名前
	template<class T> void Init(T& _parameter, const std::string& _nodeName);

	// 子ノードの数取得
	u_int GetChildNum() const;

	/// @brief 子供のパラメータを取得する
	/// @tparam T パラメータの型
	/// @param _arrayIdx 配列のID
	/// @return パラメータ
	template<class T> T* GetChild(u_int _arrayIdx);

	/// @brief 子供のパラメータを取得する
	/// @tparam T パラメータの型
	/// @param _nodeName ノード名
	/// @return パラメータ
	template<class T> T* GetChild(const std::string& _nodeName);

	// 名前を取得する
	std::string GetName() const;

private:
	/// @brief パラメータをセットする
	/// @tparam T パラメータの型
	/// @param _parameter パラメータ
	template<class T> void SetParameter(T* _parameter);
};

template<class T>
inline void NodeParam<T>::SetParameter(T* _parameter)
{
	parameter = _parameter;
}

template<class T>
inline T* NodeParam<T>::GetParameter()
{
	return parameter;
}

template<class T>
inline void TreeNode::Init(T& _parameter, const std::string& _nodeName)
{
	SetParameter(&_parameter);

	nodeName = _nodeName;
}

template<class T>
inline void TreeNode::SetParameter(T* _parameter)
{
	// 前に残っているパラメータを解放
	pHaveParameter.reset();

	// パラメータをセット
	NodeParam<T>* p = dynamic_cast<NodeParam<T>*>(pHaveParameter);
	p->SetParameter(_parameter);
}

template<class T>
inline T* TreeNode::GetChild(u_int _arrayIdx)
{
	assert(GetChildNum() > _arrayIdx);

	NodeParam_Base* p_Base = pChildNodes[_arrayIdx]->pHaveParameter.get();

	// パラメータをセット
	NodeParam<T>* p = dynamic_cast<NodeParam<T>*>(p_Base);
	T* parameter = p->GetParameter();

	assert(parameter != nullptr);

	return parameter;
}

template<class T>
inline T* TreeNode::GetChild(const std::string& _nodeName)
{
	// ノードを名前から探す
	NodeParam_Base* pNodeBase = nullptr;
	for (auto itr = pChildNodes.begin(); itr != pChildNodes.end(); itr++)
	{
		if (_nodeName != (*itr)->GetName())
			continue;

		pNodeBase = *itr;
	}

	if (pNodeBase == nullptr)
	{
		HASHI_DEBUG_LOG(_nodeName + "：子ノードがありません");
		return nullptr;
	}

	// パラメータをセット
	NodeParam<T>* p = dynamic_cast<NodeParam<T>*>(pNodeBase);
	T* parameter = p->GetParameter();

	assert(parameter != nullptr);

	return parameter;
}
