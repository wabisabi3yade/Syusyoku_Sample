#pragma once
#include "Asset_Base.h"

struct aiScene;
struct aiAnimation;

// �A�j���[�V�����̃f�[�^�N���X
class AnimationData : public Asset_Base
{ 
	// ��Assimp�̌^����Ǝ��̌^�֕ϊ������� 20240722
	const aiScene* pAiScene;

	// ��Assimp�̌^����Ǝ��̌^�֕ϊ������� 20240722
	const aiAnimation* pAnimationData;

	// �A�j���[�V�����̃{�[��ID�z��
	std::vector<u_int> boneIndicies;

public:
	AnimationData() : pAiScene(nullptr), pAnimationData(nullptr) {}
	~AnimationData() {}

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
};

