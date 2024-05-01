#pragma once

#include "Singleton_Base.h"
#include "Window.h"

// 消す
class Camera;

class SceneManager;
class Direct3D11;

// ゲームループなどアプリを動かすのに主要な機能をもつクラス(シングルトンパターン)
class MainApplication
	: public Singleton_Base<MainApplication>
{
	// 基底クラスにコンストラクタを渡すために
	friend class Singleton_Base<MainApplication>;	

	Window* pWindow;	// ウィンドウ処理クラス
	Direct3D11* pD3D;	// Direct3Dの機能を持つクラス
	SceneManager* pSceneManager;	// シーンマネージャークラス

	MainApplication();
	~MainApplication();

	void Release();
public:	
	// 初期化する
	// 引数：インスタンスハンドル
	void Init(HINSTANCE _hInst);

	// ゲームループの処理を書く
	void GameLoop();	
};

