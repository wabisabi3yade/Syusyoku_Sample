#pragma once
#include "SceneMoveInfo.h"
#include "SubScene_Base.h"

// SubScene�̑J�ڂ�����N���X
class ChangeSubScene
{
protected:
	// �ύX���������T�u�V�[���ϐ�
	SubScene_Base* pSubScene;
	// �V����
	SceneMoveInfo* moveInfo;

public:
	ChangeSubScene(SceneMoveInfo* _moveInfo);
	virtual ~ChangeSubScene();

	/// <summary>
	/// �T�u�V�[�����w�肵���V�[���ɕύX����
	/// </summary>
	/// <param name="_nextSubId">�ύX��̃T�u�V�[���̗񋓌^�ϐ�</param>
	/// <returns>�ύX������̃T�u�V�[���̃|�C���^</returns>
	virtual SubScene_Base* ChangeScene() = 0;
};

