#pragma once
// プリコンパイル済みヘッダー
// よく使うインクルードファイルをまとめ、時間とリソースを節約

// マクロ定義 /////////////////
// リソースを解放するマクロ
#define SAFE_RELEASE(x)    { if(x) { (x)->Release(); (x) = nullptr; } }

// クラスの安全に解放するマクロ定義
#define CLASS_DELETE(p) {if(p != nullptr) { delete p; p = nullptr;} }

// max min関数を無効にする
#define NOMINMAX

// ImGuiを表示する定義
#define EDIT_DISPLAY
#if defined(_DEBUG) || defined(EDIT_DISPLAY)
#define EDIT
#endif


// インクルードファイル /////////////////
#include <iostream>	// std使うために
#include <algorithm>
#include <string>
#include <unordered_map>	// 順番を気にしないmap配列
#include <typeinfo>	// 型名を取得
#include <filesystem>	// パス名
#include <vector>	// 動的配列
#include <numbers>	// 数学
#include <d3d11.h>	// DX11の機能
#include <wrl.h>	// ComPtr
#pragma comment(lib, "d3d11.lib")

// 外部ライブラリ
#include <DirectXMath.h>	// 数学系の機能
#include <SimpleMath.h>	// DirectXTKの数学系
#pragma comment(lib, "DirectXTK.lib")	// DirectXTKライブラリ
#include <nlohmann/json.hpp>	// json書き出し

// 外部ヘッダー

// 自前クラス
#include "MathUseful.h"	// 自作数学便利関数
#include "ImGuiMethod.h"	// ImGui便利関数
#include "ImGuiDebugLog.h"	// デバッグログ
#include "Transform.h"	// 座標・回転・スケールのクラス
#include "MainApplication.h"
#include "Direct3D11.h"	// 自前のDirectX11クラス