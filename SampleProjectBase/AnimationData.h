#pragma once
#include "Asset_Base.h"

struct aiScene;
struct aiAnimation;
struct aiNodeAnim;
struct aiNode;

/// @brief �A�j���[�V�����̃f�[�^�N���X
class AnimationData : public Asset_Base
{ 
private:

	// ��Assimp�̌^����Ǝ��̌^�֕ϊ������� 20240722
	const aiScene* pAiScene;

	// ��Assimp�̌^����Ǝ��̌^�֕ϊ������� 20240722
	const aiAnimation* pAnimationData;

	// �A�j���[�V�����̃{�[��ID�z��
	std::vector<u_int> boneIndicies;

	/// @brief �A�j���[�V�����̎���
	float animationTime;

public:
	AnimationData() : pAiScene(nullptr), pAnimationData(nullptr), animationTime(0.0f) {}
	~AnimationData() {}

	// �m�[�h��2�L�[�ȏ゠�邩�m�F����(�e�p�����[�^)
	// �����F_nodeId �m�[�hID
	// �߂�l�F2�ȏ゠�邩�H
	bool HasScaleTwoKeys(u_int _nodeId) const;
	bool HasQuatTwoKeys(u_int _nodeId) const;
	bool HasPosTwoKeys(u_int _nodeId) const;

	// �A�j���[�V�����Z�b�g
	void SetAiScene(const aiScene* _pAiScene);

	/// @brief �m�[�h�ɑΉ������{�[����ID���Z�b�g����
	/// @param _nodeId �m�[�hID 
	void SetBoneIdx(u_int _nodeId, u_int _boneIdx);

	/// @brief �Z�Ԗڂ̃m�[�h�ɑΉ�����{�[�������擾
	/// @param _nodeId �m�[�h��Id
	/// @return �{�[���̖��O
	std::string GetBoneName(u_int _nodeId);

	// �`�����l���̐����擾
	u_int GetChannelCount();

	/// @brief �X�P�[�������߂�
	/// @param _nodeId �m�[�hID
	/// @param _playingTime �Đ�����
	/// @return �A�j���[�V�����̃X�P�[��
	DirectX::SimpleMath::Vector3 GetScale(u_int _nodeId, float _playingTime) const;
	DirectX::SimpleMath::Vector3 GetScale(u_int _nodeId, u_int _flame) const;

	/// @brief �N�H�[�^�j�I�������߂�
	/// @param _nodeId �m�[�hID
	/// @param _playingTime �Đ�����
	/// @return �A�j���[�V�����̃N�H�[�^�j�I��
	DirectX::SimpleMath::Quaternion GetQuaternion(u_int _nodeId, float _playingTime) const;
	DirectX::SimpleMath::Quaternion GetQuaternion(u_int _nodeId, u_int _flame) const;

	/// @brief ���W�����߂�
	/// @param _nodeId �m�[�hID
	/// @param _playingTime �Đ�����
	/// @return �A�j���[�V�����̍��W
	DirectX::SimpleMath::Vector3 GetPosition(u_int _nodeId, float _playingTime) const;
	DirectX::SimpleMath::Vector3 GetPosition(u_int _nodeId, u_int _flame) const;

	// �A�j���[�V�����S�̂̎��Ԃ��擾
	float GetAnimationTime() const;

	// �K�w�̃��[�g�ł���m�[�h��Ԃ�
	const aiNode* GetRootNode();

	// �m�[�h������A�j���[�V�����m�[�h���擾 
	const aiNodeAnim* GetAiNodeAnim(const std::string& _nodeName);

private:
	/// @brief �A�j���[�V�����̍Đ����Ԃ����߂�
	void CalculateAnimTime();

	// �Đ����Ԃ���e�p�����[�^�̃L�[���擾����
	// �����F_playingTime �Đ����ԁ@_pAiNodeAnim �m�[�h
	u_int FindPreviousRotKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const;
	u_int FindPreviousScaleKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const;
	u_int FindPreviousPosKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const;

	// �e�p�����[�^���m�[�hID�ƃL�[�l����擾����
	// �����F_nodeId �m�[�hID�@ u_int _key �L�[
	DirectX::SimpleMath::Vector3 GetScaleByKey(u_int _nodeId, u_int _key) const;
	DirectX::SimpleMath::Quaternion GetQuatByKey(u_int _nodeId, u_int _key) const;
	DirectX::SimpleMath::Vector3 GetPosByKey(u_int _nodeId, u_int _key) const;
};

