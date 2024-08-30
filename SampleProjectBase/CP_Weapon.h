#pragma once
#include "CloneComponent.h"

class Bone;
class SkeletalMesh;
class CP_MeshRenderer;

/// @brief ����ɂ���R���|�[�l���g
class CP_Weapon : public Component , public CloneComponent<CP_Weapon>
{
	std::string grabBoneName;

	/// @brief ��������{�[��
	const Bone* pGrabBone;

	const CP_MeshRenderer* pMeshRenderer;

	/// @brief ���{�[���̍��W
	DirectX::SimpleMath::Vector3 offsetPosition;

	/// @brief ���{�[���̉�]
	DirectX::SimpleMath::Vector3 offsetAngles;

	/// @brief ���[�h���̊p�x
	DirectX::SimpleMath::Vector3 loadMeshAngles;

	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Quaternion rot;

	/// @brief ���[�h���̃X�P�[��
	float loadMeshScale;
public:
	CP_Weapon();
	~CP_Weapon() {}

	void Init() override;

	void Start() override;

	void LateUpdate() override;

	void Draw() override;

	void ImGuiSetting() override;

	// ��������{�[�����擾����
	void SetGrabBone(const Bone* _grabBone);

	// �{�[�����Z�b�g
	void SetGrabBoneName(const std::string& _grabName);

	/// @brief �X�P���^�����b�V����������擾����
	/// @param _skeletalMesh ���f����X�P���^�����b�V��
	void SetSkeletalMeshData(SkeletalMesh& _skeletalMesh);

	// �{�[�����擾
	std::string GetGrabBoneName() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �{�[��������W���X�V����
	void UpdateTransform();

	void ImGuiSetBone();

	/// @brief �X�V�ł����Ԃ��m�F
	/// @return �X�V�ł��邩�H
	bool IsCanUpdate();

	void SetBoneFromParent();
};

