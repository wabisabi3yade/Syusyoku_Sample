#pragma once
#include "SubScene_Base.h"
class Tank_ResultSub :
    public SubScene_Base
{
public:
	Tank_ResultSub(SceneMoveInfo* _moveInfo);
	~Tank_ResultSub();

	// 更新処理
	void Update() override;

	// Updateのあとに行われる更新処理
	void LateUpdate() override;

	// 描画をする
	void Draw() override;
};

