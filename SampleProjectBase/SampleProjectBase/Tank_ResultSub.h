#pragma once
#include "SubScene_Base.h"
class Tank_ResultSub :
    public SubScene_Base
{
public:
	Tank_ResultSub(SceneMoveInfo* _moveInfo);
	~Tank_ResultSub();

	// �X�V����
	void Update() override;

	// Update�̂��Ƃɍs����X�V����
	void LateUpdate() override;

	// �`�������
	void Draw() override;
};

