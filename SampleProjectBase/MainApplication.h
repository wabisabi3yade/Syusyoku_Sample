#pragma once

#include "Singleton_Base.h"
#include "Window.h"
#include "InputClass.h"
#include "VariableFrameRate.h"
#include "AppSystemDraw.h"

class SceneManager;
class Direct3D11;
class AssetCollection;

// ゲームループなどアプリを動かすのに主要な機能をもつクラス(シングルトンパターン)
class MainApplication
{
	/// @brief ウィンドウ処理
	static std::unique_ptr<Window> pWindow;

	/// @brief Direct3Dの機能
	static Direct3D11* pD3D;

	/// @brief シーンマネージャー
	static SceneManager* pSceneManager;

	/// @brief 可変フレームレートクラス
	static std::unique_ptr<VariableFrameRate> pVariableFps;

	/// @brief 入力管理
	static std::unique_ptr<InputClass> pInput;

	/// @brief アセットの管理
	static std::unique_ptr<AssetCollection> pAssetCollection;

	/// @brief アプリケーション終了するか確認中
	static bool isEscapeChecking;

	MainApplication() {};
	~MainApplication() {};

	/// @brief アプリケーション終了確認
	/// @return 終了フラグ
	static bool EscapeCheck();

	/// @brief システム関連の描画
	static void SystemDraw();

public:

	/// @brief 初期化
	/// @param _hInst ハンドルインスタンス 
	static void Init(HINSTANCE _hInst);

	/// @brief ゲームループ
	static void GameLoop();

	/// @brief 解放処理
	static void Release();

	/// @brief Δtを返す
	/// @return Δt(s)
	static float DeltaTime();

	/// @brief 入力クラスを返す
	/// @return 
	static InputClass& GetInput() { return *pInput; }

private:
	// セットアップ関数

	/// @brief ウィンドウ初期化
	/// @param _hInst ハンドルインスタンス
	static void WindowSetup(HINSTANCE _hInst);

	/// @brief Direct3Dの初期化
	static void D3DSetup(HWND _hwnd);

	/// @brief 可変フレームレート初期化
	static void VariableFrameSetup();

	/// @brief 入力初期化
	static void InputSetup(HWND _hwnd);

	/// @brief ImGui初期化
	static void ImGuiSetup();

	/// @brief アセット関係のシステムの初期化
	static void AssetSysytemSetup();

	/// @brief シェーダーの初期化
	static void ShaderSetup();

	/// @brief シーンマネージャー初期化
	static void SceneManagerSetup();

	/// @brief Updateできるか取得する
	/// @return Updateできるか？
	static bool CanUpdate();
};

