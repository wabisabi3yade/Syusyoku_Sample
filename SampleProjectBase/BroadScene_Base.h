#pragma once
#include "SceneMoveInfo.h"

class SubScene_Base;
class ChangeSubScene;

// 複数の関連したシーンが集まった大局クラス(分野によって分ける)
class BroadScene_Base
{
protected:
	SubScene_Base* pNowScene = nullptr;	// 今実行しているシーン変数
	ChangeSubScene* pChangeSubScene = nullptr;	// シーンを変更するクラス

	// シーン遷移クラスの初期化をする
	virtual void InitChangeScene(SceneMoveInfo* _moveInfo) = 0;

public:
	BroadScene_Base(SceneMoveInfo* _pMoveInfo);

	virtual ~BroadScene_Base();

	// サブシーンを遷移する
	SubScene_Base* ChangeSubScene();

	// シーンの実行関数(外部からはこれを読み込む)
	// ↓説明
	// 外部はシーンの中身がどうなってるかなど知る必要がない
	// 極力シンプルな呼び出しだけを行う
	void Exec(); 

	virtual void Release();
};

