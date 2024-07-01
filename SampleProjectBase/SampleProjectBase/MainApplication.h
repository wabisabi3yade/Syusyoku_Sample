#pragma once

#include "Singleton_Base.h"
#include "Window.h"
#include "InputClass.h"
#include "VariableFrameRate.h"

class SceneManager;
class Direct3D11;



// ゲームループなどアプリを動かすのに主要な機能をもつクラス(シングルトンパターン)
class MainApplication
{
	static std::unique_ptr<Window> pWindow;	// ウィンドウ処理クラス
	static Direct3D11* pD3D;	// Direct3Dの機能を持つクラス
	static SceneManager* pSceneManager;	// シーンマネージャークラス
	static std::unique_ptr<VariableFrameRate> variableFps;	// 可変フレームレートクラス
	static std::unique_ptr<InputClass> input;	// 入力クラス

	// アプリケーション終了
	static bool isEscapeDisplay;

	MainApplication(){};
	~MainApplication(){};

	static bool Escape();
public:
	// 初期化する
	// 引数：インスタンスハンドル
	static void Init(HINSTANCE _hInst);

	// ゲームループの処理を書く
	static void GameLoop();

	static void Release();

	static float DeltaTime();

	static InputClass& GetInput() { return *input; }
};

