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

// //ImGuiを表示する定義
//#define EDIT_DISPLAY
#if defined(_DEBUG) || defined(EDIT_DISPLAY)
#define EDIT
#endif

// インクルードファイル /////////////////
#include <iostream>	// std使うために
#include <fstream>
#include <random>	// 乱数
#include <algorithm>
#include <string>
#include <unordered_map>	// 順番を気にしないmap配列
#include <unordered_set>	// 同一キーが格納できない配列
#include <typeinfo>	// 型名を取得
#include <filesystem>	// パス名
#include <vector>	// 動的配列
#include <variant>	// variant型
#include <numbers>	// 数学
#include <limits>
#include <d3d11.h>	// DX11の機能
#include <wrl.h>	// ComPtr
#pragma comment(lib, "d3d11.lib")

// 外部ライブラリ
#include <DirectXMath.h>	// 数学系の機能
#include <SimpleMath.h>	// DirectXTKの数学系
#include <Audio.h>	// XAudio2をXTKでラップしたファイル
#pragma comment(lib, "DirectXTK.lib")	// DirectXTKライブラリ
#include <nlohmann/json.hpp>	// json書き出し
#include <magic_enum.hpp>
#include <Effekseer.h>	// エフェクシア
#include <EffekseerRendererDX11.h>	// エフェクシアDx11対応

// ImGui
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
// ImPlot
#include <implot.h>

// 名前空間定義
namespace HashiTaku
{
	namespace DX = DirectX;
	namespace DXSimp = DirectX::SimpleMath;
	using json = nlohmann::json;
}

// 自前クラス
#include "IClassNameGetter.h"
#include "MathUseful.h"	// 自作数学便利関数
#include "ImGuiMethod.h"	// ImGui便利関数
#include "ImGuiDebugLog.h"	// デバッグログ
#include "IImGuiUser.h"	// ImGuiを使用するクラスのインターフェース
#include "Transform.h"	// 座標・回転・スケールのクラス
#include "MainApplication.h"
#include "HashiTaku_Random.h"	// 乱数クラス
#include "Direct3D11.h"	// 自前のDirectX11
#include "DX11BulletPhisics.h"	// DX11物理エンジン
#include "DX11EffecseerManager.h"	// エフェクトマネジャー
#include "DXSoundManager.h"	// サウンドマネジャー