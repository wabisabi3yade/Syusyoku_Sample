#pragma once
#include "SceneMoveInfo.h"
#include "SubScene_Base.h"

// SubSceneの遷移をするクラス
class ChangeSubScene
{
protected:
	// 変更させたいサブシーン変数
	SubScene_Base* pSubScene;
	// 新しく
	SceneMoveInfo* moveInfo;

public:
	ChangeSubScene(SceneMoveInfo* _moveInfo);
	virtual ~ChangeSubScene();

	/// <summary>
	/// サブシーンを指定したシーンに変更する
	/// </summary>
	/// <param name="_nextSubId">変更先のサブシーンの列挙型変数</param>
	/// <returns>変更した後のサブシーンのポインタ</returns>
	virtual SubScene_Base* ChangeScene() = 0;
};

