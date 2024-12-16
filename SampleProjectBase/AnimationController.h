#pragma once
#include "Asset_Base.h"
#include"AnimationNode_Base.h"
#include "AnimationParameters.h"
#include "AnimTransitionArrow.h"
#include "AnimationNotifyFactory.h"
#include "AnimationNotify_Base.h"

// �N���X�t�F�[�h�E�������
#include "CrossFadeAnimation.h"
#include "InertInterpAnimation.h"
// vector�z��Ŏg�p���邽��include
#include "Bone.h"

namespace HashiTaku
{
	// �{�[��
	class BoneList;

	using AnimNotifyList = std::list<std::unique_ptr<AnimationNotify_Base>>;
	using TransArrowList = std::list<std::unique_ptr<AnimTransitionArrow>>;

	/// @brief �A�j���[�V�����m�[�h�֘A�̏��
	struct AnimNodeInfo
	{
		std::unique_ptr<AnimationNode_Base> pAnimNode;	// �A�j���[�V�����m�[�h
		TransArrowList transitionArrows; // �J�ڌ��ƂȂ��Ă�����
		std::string groupArrowsName;	// �����Ă���J�ڏ����O���[�v��
		AnimNotifyList notifyList; // �ʒm�C�x���g
	};

	/// @brief �A�j���[�V�����J�ڂ��Ǘ�����N���X
	class AnimationController : public Asset_Base, public IImGuiUser
	{
	private:
		/// @brief �A�j���[�V�����m�[�h�֘A�̏�񃊃X�g
		std::list<std::unique_ptr<AnimNodeInfo>> animNodeInfos;

		/// @brief �p�����[�^���X�g
		std::unique_ptr<AnimationParameters> pAnimParameters;

		/// @brief �ʒm�C�x���g�쐬�N���X
		std::unique_ptr<AnimationNotifyFactory> pNotifyFactory;

		/// @brief �J�ڏ������O���[�v�ɂ�������(�L�����Z���̃A�j���[�V���������ЂƂ܂Ƃ߂ɂł���)
		std::unordered_map<std::string, TransArrowList> groupArrows;

		/// @brief �ŏ��Ɏn�߂�m�[�h���
		AnimNodeInfo* pDefaultNodeInfo;

		/// @brief	�Ή����Ă���{�[���z��
		BoneList* pAssetBoneList;

		/// @brief �Đ����x
		float playSpeed;
	public:
		/// @brief �R���X�g���N�^
		/// @param _setType �R���g���[���[�̎��
		AnimationController();
		~AnimationController() {}

		/// @brief �A�j���[�V�����J�ڂ̖����쐬����
		/// @param _fromNodeName �J�ڌ��A�j���[�V����
		/// @param _toNodeName �J�ڐ�A�j���[�V����
		/// @return �쐬�������
		AnimTransitionArrow* CreateTransitionArrow(const std::string& _fromNodeName,
			const std::string& _toNodeName);

		/// @brief �A�j���[�V�����J�ڂ̖����쐬����
		/// @param _toNodeName �J�ڐ�A�j���[�V����
		/// @return �쐬�������
		std::unique_ptr<AnimTransitionArrow> CreateTransitionArrow(const std::string& _toNodeName);

		/// @brief �A�j���[�V���������O
		/// @param _animName �A�j���[�V�����̖��O
		void RemoveNode(const std::string& _animName);

		/// @brief �A�j���[�V�����̐i�߂鑬�x���擾
		/// @return �i�߂鑬�x
		float GetPlaySpeed() const;

		/// @brief �f�t�H���g�m�[�h�����擾
		/// @return �f�t�H���g�m�[�h���
		AnimNodeInfo* GetDefaultNode() const;

		// ���O����m�[�h�����擾 
		AnimNodeInfo* GetNodeInfo(const std::string& _name) const;

		// �m�[�h����m�[�h�����擾����
		const AnimNodeInfo* GetNodeInfo(const AnimationNode_Base& _node) const;

		/// @brief �m�[�h�̐����擾
		/// @return �m�[�h��
		void GetNodeArray(std::list<const AnimationNode_Base*>& _animNodeArray) const;

		/// @brief �A�j���[�V�����̃p�����[�^���擾
		/// @return �A�j���[�V�����̃p�����[�^
		const AnimationParameters& GetAnimationParameters() const;

		/// @brief �O���[�v�̑J�ږ�󃊃X�g���擾����
		/// @param _groupName �O���[�v��
		/// @return �O���[�v�̑J�ږ�󃊃X�g(nullptr�Ȃ�O���[�v���q�b�g����)
		const TransArrowList* GetGroupArrows(const std::string& _groupName) const;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief ���O�̃m�[�h�����ɂ��邩�m�F����
		/// @param _nodeName �m�F���閼�O
		/// @return �m�[�h�������Ă��邩�H
		bool IsHaveNode(const std::string& _nodeName);

		/// @brief �m�[�h�����d�����Ȃ��悤�Ȗ��O�ɕϊ�
		/// @param _nodename �m�[�h��
		void NotDuplicateNodeName(std::string& _nodename);

		/// @brief �m�[�h�^�C�v����V�����m�[�h���쐬����
		/// @param _nodeType �m�[�h�̎��
		/// @param _nodeName �m�[�h��
		/// @return �쐬�����m�[�h���
		AnimNodeInfo* CreateNodeInfoByType(AnimationNode_Base::NodeType _nodeType, const std::string& _nodeName = "Default");

		/// @brief �J�ڏ����̃O���[�v���쐬����
		/// @param _groupName �O���[�v��
		/// @return �쐬�����O���[�v���(nullptr�Ȃ�쐬���s)
		TransArrowList* CreateGroupArrows(const std::string& _groupName);

		/// @brief �O���[�v�ɖ���ǉ�
		/// @param _groupName �ǉ�����O���[�v��
		/// @param _addArrow �ǉ�������
		void AddArrowToGroup(const std::string& _groupName,
			std::unique_ptr<AnimTransitionArrow> _addArrow);

		/// @brief �O���[�v�J�ڏ����̖��O�ꗗ���擾
		/// @param _outList ���X�g
		void GetGroupNamelist(std::vector<const std::string*>& _outList);

		/// @brief �J�ږ��̗D�揇�ʂŃ\�[�g
		static bool SortArrowPriority(const std::unique_ptr<AnimTransitionArrow>& _a1, const std::unique_ptr<AnimTransitionArrow>& _a2);

		// ImGui�Ńm�[�h�֌W
		void ImGuiNode(const std::vector<std::string>& _nodeNames);

		// ImGui�őJ�ږ��
		void ImGuiTransArrow(AnimNodeInfo& _nodeInfo, const std::vector<std::string>& _nodeNames);

		// ImGuif�Ń{�[�����X�g���Z�b�g
		void ImGuiSetBoneList();

		// ImGui�Ńf�t�H���g�m�[�h�ݒ�
		void ImGuiSetDefaultNode(const std::vector<std::string>& _nodeNames);

		// ImGui�Ńm�[�h�쐬
		void ImGuiCreateNode();

		// ImGui�Œʒm�C�x���g���쐬
		// �����F�ǉ���̃m�[�h���
		void ImGuiAnimNotify(AnimNodeInfo& _nodeInfo);

		// ImGui�ŃO���[�v�̑J�ڏ�����ҏW
		void ImGuiGroupArrows(const std::vector<std::string>& _nodeNames);

		// �m�[�h�����Z�[�u����
		nlohmann::json SaveNodeInfo(AnimNodeInfo& _nodeInfo);

		// �O���[�v�J�ڏ������Z�[�u����
		nlohmann::json SaveGroupArrows();

		// �m�[�h�������[�h����
		void LoadNodeInfo(const nlohmann::json& _nodeInfoData);

		// �ʒm�C�x���g�����[�h����
		void LoadNotify(const nlohmann::json& _nodeInfoData);

		// �J�ږ������[�h����
		void LoadTransArrow(const nlohmann::json& _nodeInfoData);

		// �O���[�v�������[�h����
		void LoadGroupArrow(const nlohmann::json& _groupArrowData);
	protected:
		void ImGuiDebug() override;
	};
}