#pragma once
#include "CloneComponent.h"

class Bone;
class SkeletalMesh;

/// @brief ����ɂ���R���|�[�l���g
class CP_Weapon : public Component , public CloneComponent<CP_Weapon>
{
	/// @brief ��������{�[����
	std::string grabBoneName;

	/// @brief ��������{�[��
	const Bone* pGrabBone;

	/// @brief ���{�[���̍��W
	DirectX::SimpleMath::Vector3 offsetPosition;

	/// @brief ���{�[���̉�]
	DirectX::SimpleMath::Vector3 offsetAngles;

public:
	CP_Weapon();
	~CP_Weapon() {}

	void LateUpdate() override;

	void Draw() override;

	void ImGuiSetting() override;

	/// @brief �X�P���^�����b�V������K�v�ȏ����擾����
	/// @param _sk �X�P���^�����b�V��
	void SetSkeletalMesh(const SkeletalMesh& _sk);

	// ��������{�[�����擾����
	void SetGrabBone(const Bone& _grabBone);

	// �{�[�����Z�b�g
	void SetGrabBoneName(const std::string& _grabName);

	// �{�[�����擾
	std::string GetGrabBoneName() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void UpdateTransform();
};

