#pragma once
#include "SubScene_Base.h"
class Tank_InGameSub :
    public SubScene_Base
{
public:
	Tank_InGameSub(SceneMoveInfo* _moveInfo);
	~Tank_InGameSub();

	// �X�V����
	void Update() override;

	// Update�̂��Ƃɍs����X�V����
	void LateUpdate() override;

	// �`�������
	void Draw() override;
};

