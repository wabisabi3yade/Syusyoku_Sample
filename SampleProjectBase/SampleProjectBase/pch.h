#pragma once
// プリコンパイル済みヘッダー
// よく使うインクルードファイルをまとめ、時間とリソースを節約


// マクロ定義 /////////////////
// リソースを解放するマクロ
#define SAFE_RELEASE(x)    { if(x) { (x)->Release(); (x) = nullptr; } }
// クラスの安全に解放するマクロ定義
#define CLASS_DELETE(p) {if(p != nullptr) { delete p; p = nullptr;} }

// インクルードファイル /////////////////
#include <d3d11.h>	// DX11の機能
#pragma comment(lib, "d3d11.lib")	// ライブラリ
#include <DirectXMath.h>	// 数学系の機能
#include <SimpleMath.h>	// DirectXTKの数学系
#pragma comment(lib, "DirectXTK.lib")	// DirectXTKライブラリ

#include <iostream>	// std使うために
#include <string>
#include <unordered_map>	// 順番を気にしないmap配列
#include <vector>	// 動的配列

#include "ImGuiMethod.h"	// ImGui便利関数
#include "Transform.h"	// 座標・回転・スケールのクラス
#include "MainApplication.h"
#include "Direct3D11.h"	// 自前のDirectX11クラス
#include "ResourceCollection.h"	// リソースを管理するクラス


// 構造体 /////////////////
// 頂点座標
struct Vertex
{
	// 座標
	DirectX::SimpleMath::Vector3 position = {};
	// 色(RGBA)
	DirectX::SimpleMath::Color color = {};

	// UV座標（テクスチャ座標）
	DirectX::SimpleMath::Vector2 uv = {};
	// 頂点の法線ベクトル
	DirectX::SimpleMath::Vector3 normal = {};
};


