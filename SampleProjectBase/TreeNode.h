#pragma once

/// @brief �m�[�h���̃p�����[�^�x�[�X
class NodeParam_Base
{
	
public:
	NodeParam_Base() {}
	virtual ~NodeParam_Base() {}
};

/// @brief �m�[�h���̃p�����[�^
/// @tparam T �p�����[�^�̌^��
template<class T>
class NodeParam : NodeParam_Base
{
	/// @brief �m�[�h���ɂ���p�����[�^
	T* parameter;

public:
	NodeParam() {}
	~NodeParam() {}

	/// @brief �p�����[�^���Z�b�g
	/// @param _parameter �p�����[�^
	void SetParameter(T* _parameter);

	/// @brief �m�[�h�̃p�����[�^���擾
	/// @return �p�����[�^
	T* GetParameter();
};

/// @brief �e�q�֌W�ł�1�m�[�h
/// @tparam T �e�q���w���N���X
class TreeNode
{
	/// @brief �m�[�h��
	std::string nodeName;

	/// @brief ���̃m�[�h�����p�����[�^
	std::unique_ptr<NodeParam_Base> pHaveParameter;

	/// @brief �e�m�[�h
	TreeNode* pParentNode;

	/// @brief �q�m�[�h
	std::list<TreeNode*>  pChildNodes;

	/// @brief ID
	u_int idx;

	/// @brief �p�����[�^���ݒ肳��Ă��邩
	bool isHaveParam;

public:
	TreeNode() : nodeName(""), pParentNode(nullptr), idx(-1) {}
	~TreeNode() {}

	/// @brief �e�m�[�h���Z�b�g
	/// @param _parentNode �e�m�[�h
	void SetParent(TreeNode& _parentNode);

	/// @brief �q���m�[�h��ǉ�
	/// @param _chiledNode �q�m�[�h
	void AddChiled(TreeNode& _chiledNode);

	/// @brief �q���m�[�h�����O
	/// @param _chiledNode �q�m�[�h
	void RemoveChiled(TreeNode& _chiledNode);

	/// @brief ��������
	/// @tparam T ��������p�����[�^�̌^
	/// @param _parameter �p�����[�^
	/// @param _nodeName ���O
	template<class T> void Init(T& _parameter, const std::string& _nodeName);

	// �q�m�[�h�̐��擾
	u_int GetChildNum() const;

	/// @brief �q���̃p�����[�^���擾����
	/// @tparam T �p�����[�^�̌^
	/// @param _arrayIdx �z���ID
	/// @return �p�����[�^
	template<class T> T* GetChild(u_int _arrayIdx);

	/// @brief �q���̃p�����[�^���擾����
	/// @tparam T �p�����[�^�̌^
	/// @param _nodeName �m�[�h��
	/// @return �p�����[�^
	template<class T> T* GetChild(const std::string& _nodeName);

	// ���O���擾����
	std::string GetName() const;

private:
	/// @brief �p�����[�^���Z�b�g����
	/// @tparam T �p�����[�^�̌^
	/// @param _parameter �p�����[�^
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
	// �O�Ɏc���Ă���p�����[�^�����
	pHaveParameter.reset();

	// �p�����[�^���Z�b�g
	NodeParam<T>* p = dynamic_cast<NodeParam<T>*>(pHaveParameter);
	p->SetParameter(_parameter);
}

template<class T>
inline T* TreeNode::GetChild(u_int _arrayIdx)
{
	assert(GetChildNum() > _arrayIdx);

	NodeParam_Base* p_Base = pChildNodes[_arrayIdx]->pHaveParameter.get();

	// �p�����[�^���Z�b�g
	NodeParam<T>* p = dynamic_cast<NodeParam<T>*>(p_Base);
	T* parameter = p->GetParameter();

	assert(parameter != nullptr);

	return parameter;
}

template<class T>
inline T* TreeNode::GetChild(const std::string& _nodeName)
{
	// �m�[�h�𖼑O����T��
	NodeParam_Base* pNodeBase = nullptr;
	for (auto itr = pChildNodes.begin(); itr != pChildNodes.end(); itr++)
	{
		if (_nodeName != (*itr)->GetName())
			continue;

		pNodeBase = *itr;
	}

	if (pNodeBase == nullptr)
	{
		HASHI_DEBUG_LOG(_nodeName + "�F�q�m�[�h������܂���");
		return nullptr;
	}

	// �p�����[�^���Z�b�g
	NodeParam<T>* p = dynamic_cast<NodeParam<T>*>(pNodeBase);
	T* parameter = p->GetParameter();

	assert(parameter != nullptr);

	return parameter;
}
