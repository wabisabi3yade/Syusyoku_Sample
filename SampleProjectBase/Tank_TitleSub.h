#pragma once
#include "SubScene_Base.h"

class Tank_TitleSub :
    public SubScene_Base
{

public:
    Tank_TitleSub(SceneMoveInfo* _moveInfo);
    ~Tank_TitleSub();

	// 更新処理
	void Update() override;

	// Updateのあとに行われる更新処理
	void LateUpdate() override;

	// 描画をする
	void Draw() override;
};

