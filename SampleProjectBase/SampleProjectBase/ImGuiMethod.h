#pragma once
#include "ImGuiDebugLog.h"

// imguiの処理をまとめたクラス(staticクラス)
class ImGuiMethod
{
	ImGuiMethod();
	~ImGuiMethod() {};
 public:
	 /// <summary>
	 /// ImGuiの初期化処理
	 /// </summary>
	 /// <returns>初期化の成功状態</returns>
	 static void Initialize(HWND _hwnd, ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);

	 /// <summary>
	 /// フレーム更新処理
	 /// </summary>
	 static void NewFrame();

	 /// <summary>
	 /// 描画処理
	 /// </summary>
	 static void Draw();

	 /// <summary>
	 /// 終了処理
	 /// </summary>
	 static void End();
};

