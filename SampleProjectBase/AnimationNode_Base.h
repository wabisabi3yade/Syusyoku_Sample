#pragma once
#include "AnimationData.h"
#include "AnimationCurve.h"

namespace HashiTaku
{
	class BoneList;

	/// @brief �A�j���[�V�����m�[�h�Ɋ��N���X
	class AnimationNode_Base : public IImGuiUser, public ISaveLoad
	{
	public:
		// �m�[�h�̎��
		enum class NodeType
		{
			Single,	// ������̃A�j���[�V����
			Blend,	// �u�����h����Ă���A�j���[�V����
			Layerd,	// ���ʂ���
		};

#ifdef EDIT
		/// @brief �񋓌^�𕶎���ɕϊ�
		static std::vector<std::string> edit_nodeTypeStrings;
#endif // EDIT
	private:
		/// @brief �m�[�h��
		std::string nodeName;

		/// @brief �A�j���[�V�����J�[�u
		std::unique_ptr<AnimationCurve> pCurveSpeed;

		/// @brief �m�[�h�̎��
		NodeType nodeType;

		/// @brief �m�[�h�̍Đ����x�{��(1.0�͓���)
		float playNodeSpeedTimes;

		/// @brief �A�j���[�V�����̎���
		float animationTime;

		/// @brief ���[�v�Đ����邩�H
		bool isLoop;

		/// @brief �A�j���[�V�����I�����Ă��邩�H
		bool isFinish;

		/// @brief �ړ����W�̃��[�g���[�V������K�p���邩(XZ���W)
		bool isRootMotionPosXZ;

		/// @brief �ړ����W�̃��[�g���[�V������K�p���邩(Y���W)
		bool isRootMotionPosY;

	public:
		AnimationNode_Base(const std::string& _nodeName,
			NodeType _type);
		virtual ~AnimationNode_Base() {}

		// �m�[�h�����Z�b�g
		void SetNodeName(const std::string& _nodeName);

		// ���[�v���邩�Z�b�g
		void SetIsLoop(bool _isLoop);

		/// @brief �I���t���O���Z�b�g����
		void SetFinish();

		// �m�[�h���擾
		const std::string& GetNodeName() const;

		// �m�[�h�̎�ނ��擾
		NodeType GetNodeType() const;
		// �Đ����Ԃ��擾
		float GetAnimationTime() const;

		/// @brief �S�̃L�[�������߂�
		/// @return �S�̂̃L�[��
		virtual u_int GetAllKeyFrame() const;

		/// @brief ���[�v�Đ��t���O���擾
		/// @return ���[�v�Đ��t���O
		bool GetIsLoop() const;

		// �A�j���[�V�����I���t���O���擾����
		bool GetIsFinish() const;

		/// @brief XZ�������[�g���[�V�����ňړ����邩�H
		/// @return �ړ����邩�H
		bool GetIsRootMotionXZ() const;

		/// @brief Y�������[�g���[�V�����ňړ����邩�H
		/// @return �ړ����邩�H
		bool GetIsRootMotionY() const;

		/// @brief �Đ����x�{�������߂�
		/// @return �Đ����x�{��
		float GetPlaySpeedTimes() const;

		/// @brief �A�j���[�V�����J�[�u�̒l���擾����
		/// @param _ratio ����(0.0�`1.0)
		/// @return �A�j���[�V�����J�[�u�̒l(��{�I��0.0�`1.0)
		float GetCurveValue(float _ratio) const;

		json Save() override;
		void Load(const json& _data) override;
	private:
		// �m�[�h�̃p�����[�^�[�ݒ�
		void ImGuiSetParameter();
	protected:

		// �Đ����Ԃ��Z�b�g����
		void SetAnimationTime(float _time);

		void ImGuiDebug() override;
	};
}