#pragma once

// プリコンパイル済みヘッダー
// よく使うインクルードファイルをまとめ、時間とリソースを節約

// インクルードファイル /////////////////
#include <d3d11.h>	// DX11の機能
#pragma comment(lib, "d3d11.lib")	// ライブラリ
#include <DirectXMath.h>	// 数学系の機能
#include <iostream>	// std使うために
#include <vector>	// 動的配列
#include <unordered_map>	// 順番を気にしないmap配列
#include "Float2.h"	// float型変数2つクラス
#include "Float3.h"	// float型変数3つクラス
#include "Float4.h"	// float型変数3つクラス
#include "Transform.h"	// 座標・回転・スケールのクラス

#include "MessageError.h"	// エラーテキストを出す

// クラス前方宣言
class D3D11_Renderer;	// Direct3Dの描画クラス

// マクロ定義 /////////////////
// リソースを解放するマクロ
#define SAFE_RELEASE(x)    { if(x) { (x)->Release(); (x) = nullptr; } }
// クラスの安全に解放するマクロ定義
#define CLASS_DELETE(p) {if(p != nullptr) { delete p; p = nullptr;} }

// 構造体 /////////////////
// 頂点座標
struct Vertex
{
	// 座標
	Float3 position = {};
	// 色(RGBA)
	Float4 color = {};

	// UV座標（テクスチャ座標）
	DirectX::XMFLOAT2 uv = {};
	// 頂点の法線ベクトル
	Float3 normal = {};
};


