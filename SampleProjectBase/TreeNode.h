#pragma once

namespace HashiTaku
{
	// �{�[��
	class Bone;

	/// @brief ���f���̃c���[�m�[�h
	class TreeNode
	{
		/// @brief �m�[�h��
		std::string nodeName;

		/// @brief �e�m�[�h
		TreeNode* pParentNode;

		/// @brief �q�m�[�h
		std::list<std::unique_ptr<TreeNode>>  pChildNodes;

		/// @brief �e�m�[�h����̃��[�J�����W�n�̕ϊ��s��
		DXSimp::Matrix transformMtx;

		/// @brief �Ή������{�[��
		Bone* pLinkBone;
	public:
		TreeNode() : nodeName(""), pParentNode(nullptr), pLinkBone(nullptr) {}
		~TreeNode() {}

		/// @brief �e�m�[�h���Z�b�g
		/// @param _parentNode �e�m�[�h
		void SetParent(TreeNode& _parentNode);

		/// @brief �q���m�[�h��ǉ�
		/// @param _chiledNode �q�m�[�h�̃��j�[�N�|�C���^
		void AddChild(std::unique_ptr<TreeNode> _chiledNode);

		/// @brief �q���m�[�h�����O
		/// @param _chiledNode �q�m�[�h
		void RemoveChiled(TreeNode& _chiledNode);

		/// @brief �Ή������{�[�������邩�m�F
		/// @return �{�[�������邩�H
		bool HasBone() const;

		/// @brief �{�[����ID����Ή������c���[�m�[�h���q�m�[�h����T��
		/// @param _boneIdx �T�������c���[�m�[�h�̃{�[��ID
		/// @return �c���[�m�[�h(������Ȃ����nullptr)
		const TreeNode* FindTreeNode(int _boneIdx) const;

		// �m�[�h�����Z�b�g
		void SetNodeName(const std::string& _nodeName);

		// ���[�J�����W�n�ւ̕ϊ��s��Z�b�g
		void SetTransformMtx(const DXSimp::Matrix& _transformMtx);

		// �Ή������{�[�����Z�b�g
		void SetBone(Bone& _bone);

		/// @brief �q�m�[�h���擾
		/// @param _arrayIdx �z��C���f�b�N�X
		/// @return �q�m�[�h
		const TreeNode* GetChild(u_int _arrayIdx) const;

		// �q�m�[�h�̐��擾
		u_int GetChildNum() const;

		// ���O���擾����
		const std::string& GetName() const;

		/// @brief �{�[��ID���擾
		/// @return �{�[��ID
		u_int GetBoneIdx() const;

		// ���[�J�����W�܂ł̈ړ��s����擾
		const DXSimp::Matrix& GetTransformMtx() const;
	};
}