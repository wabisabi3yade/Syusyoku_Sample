#pragma once
#include "SubScene_Base.h"

class Tank_TitleSub :
    public SubScene_Base
{

public:
    Tank_TitleSub(SceneMoveInfo* _moveInfo);
    ~Tank_TitleSub();

	// �X�V����
	void Update() override;

	// Update�̂��Ƃɍs����X�V����
	void LateUpdate() override;

	// �`�������
	void Draw() override;
};

