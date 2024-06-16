#pragma once
#include "SubScene_Base.h"
class Tank_InGameSub :
    public SubScene_Base
{
public:
	Tank_InGameSub(SceneMoveInfo* _moveInfo);
	~Tank_InGameSub();

	// 更新処理
	void Update() override;

	// Updateのあとに行われる更新処理
	void LateUpdate() override;

	// 描画をする
	void Draw() override;
};

