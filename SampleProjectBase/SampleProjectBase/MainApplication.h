#pragma once

#include "Singleton_Base.h"
#include "Window.h"
#include "InputClass.h"

// 消す
class Camera;

class SceneManager;
class Direct3D11;
class VariableFrameRate;

// ゲームループなどアプリを動かすのに主要な機能をもつクラス(シングルトンパターン)
class MainApplication
	: public Singleton_Base<MainApplication>
{
	// 基底クラスにコンストラクタを渡すために
	friend class Singleton_Base<MainApplication>;

	std::unique_ptr<Window> pWindow;	// ウィンドウ処理クラス
	Direct3D11* pD3D;	// Direct3Dの機能を持つクラス
	SceneManager* pSceneManager;	// シーンマネージャークラス
	std::unique_ptr<VariableFrameRate> variableFps;	// 可変フレームレートクラス
	std::unique_ptr<InputClass> input;	// 入力クラス

	MainApplication();
	~MainApplication();

	void Release();
public:
	// 初期化する
	// 引数：インスタンスハンドル
	void Init(HINSTANCE _hInst);

	// ゲームループの処理を書く
	void GameLoop();

	float DeltaTime();	// 前フレームとの時間

	GamePad* GamePad()const { return input->GetGamePad(); }	// ゲームパッド取得
};

