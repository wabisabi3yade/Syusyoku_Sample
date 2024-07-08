#pragma once

// �V�[���̉ғ���ԃN���X
class SceneRunningState
{
public:
	// ���̃V�[���̉ғ����
	enum RUNNING_STATE
	{
		RUNNING,	// �ғ���
		FINISH,	// �I���i�Ⴄ�V�[���Ɉړ�����j
		RUNNING_NUM
	};

private:
	RUNNING_STATE runState;	// ���݂̉ғ���ԕϐ�

public:
	SceneRunningState();
	~SceneRunningState();

	// ����������
	void Init();

	// �V�[���J�ڂ���Ƃ��ɉғ���Ԃ��I���ɕϊ�����֐�
	void SetFinshState();

	// ���݂̉ғ���Ԃ��擾����֐�
	RUNNING_STATE GetRunningState()const { return runState; }

};

