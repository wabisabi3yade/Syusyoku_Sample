#pragma once
#include "Component.h"

namespace HashiTaku
{
	class Bone;
	class BoneList;

	/// @brief ����ɂ���R���|�[�l���g
	class CP_Rig : public Component
	{
		std::string grabBoneName;

		/// @brief ��������{�[��
		const Bone* pGrabBone;

		/// @brief ���[�h���̊p�x
		DirectX::SimpleMath::Quaternion loadMeshRot;

		/// @brief ���[�h���̃X�P�[��
		float loadMeshScale;

		/// @brief �{�[�����W��\������
		bool isDisplayPos;
	public:
		CP_Rig();
		~CP_Rig() {}

		void Init() override;

		void Start() override;

		void LateUpdate() override;

		void Draw() override;

		void ImGuiDebug() override;

		// ��������{�[�����擾����
		void SetGrabBone(const Bone* _grabBone);

		// �{�[�����Z�b�g
		void SetGrabBoneName(const std::string& _grabName);

		/// @brief �X�P���^�����b�V����������擾����
		/// @param _skeletalMesh ���f����X�P���^�����b�V��
		void SetSkeletalMeshData(BoneList& _skeletalMesh);

		// �{�[�����擾
		std::string GetGrabBoneName() const;

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief �{�[��������W���X�V����
		void UpdateTransform();

		void ImGuiSetBone();

		/// @brief �X�V�ł����Ԃ��m�F
		/// @return �X�V�ł��邩�H
		bool IsCanUpdate();

		void SetBoneFromParent();
	};
}