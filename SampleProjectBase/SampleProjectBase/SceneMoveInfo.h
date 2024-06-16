#pragma once
#include "SceneRunningState.h"

namespace BroadType
{
	// ��ǃV�[���̎�ޗ񋓌^
	enum Type
	{
		None = -1,	// �Ȃɂ��Ȃ�
		Test,	// �e�X�g
		Tank,	// �^���N�Q�[��
		Num
	};
}

// �V�[���J�ڂɕK�v�ȏ��i�J�ڐ�A�J�ڂ��邩�ǂ����j�������N���X
class SceneMoveInfo
{
	SceneRunningState* pRunningState;	// �V�[���̉ғ���ԁiFINISH�őJ�ڂ���j
	BroadType::Type broadType;	// �J�ڐ�̑�ǃV�[���^�C�v
	int subType;	// ���̃T�u�V�[���̃^�C�v

public:
	// �����F�ŏ��ɌĂ�	�@�T�u�V�[���@�A��ǃV�[��
	SceneMoveInfo(int _initSub, BroadType::Type _initBroad);
	~SceneMoveInfo();

	// �V�[���J�ڂ���Ƃ��ɌĂяo�����
	void OnChangeScene(int _nextSub, BroadType::Type _nextBroad);

	// �J�ڐ�̑�ǃV�[���̃^�C�v���擾
	BroadType::Type GetNextBroadType()const { return broadType; }

	// �J�ڐ�̃T�u�V�[���̃^�C�v���擾
	int GetNextSubType()const { return subType; }

	// �V�[���̉ғ���Ԃ��擾
	int GetRunningState()const;

	// �V�[���J�ڌ�Ȃǂɏ���������֐�
	void Init();
};