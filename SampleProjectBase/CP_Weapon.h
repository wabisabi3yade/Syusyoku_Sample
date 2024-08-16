#pragma once
#include "Component.h"

class Bone;

/// @brief ����ɂ���R���|�[�l���g
class CP_Weapon : public Component
{
	/// @brief ��������{�[����
	std::string grabBoneName;

	/// @brief ��������{�[��
	const Bone* pGrabBone;

public:
	CP_Weapon();
	~CP_Weapon() {}

	void LateUpdate() override;

	void Draw() override;

	// ��������{�[�����擾����
	void SetGrabBone(const Bone& _grabBone);

	// �{�[�����Z�b�g
	void SetGrabBoneName(const std::string& _grabName);

	// �{�[�����擾
	std::string GetGrabBoneName() const;

private:
	void UpdateTransform();

	
};

