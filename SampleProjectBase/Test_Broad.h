#pragma once
#include "BroadScene_Base.h"

// �e�X�g�p��ǃN���X�̃T�u�V�[���܂Ƃ�
class Test_Broad :
    public BroadScene_Base
{
    // �V�[���J�ڂ̏�����
    void InitChangeScene(SceneMoveInfo* _moveInfo) override;

public:
    Test_Broad(SceneMoveInfo* _pMoveInfo);
    ~Test_Broad();

    void Release() override;
};

