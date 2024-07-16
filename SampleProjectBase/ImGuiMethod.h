#pragma once

struct ImFont;

// imguiの処理をまとめたクラス(staticクラス)
class ImGuiMethod
{
public:
	// フォントの大きさ
	enum class FontSize
	{
		Small,	// 小
		Mid,	// 中
		Big,	// 大
		Num
	};

private:
	/// @brief フォントリスト(サイズ違い)
	static ImFont* pFonts[static_cast<u_int>(FontSize::Num)];

	ImGuiMethod() {}
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

	 /// @brief	フレーム終了処理
	 static void EndFrame();

	 /// <summary>
	 /// 描画処理
	 /// </summary>
	 static void Draw();

	 /// <summary>
	 /// 終了処理
	 /// </summary>
	 static void Terminal();
	 
	 /// @brief DragFloat3でVector3に対応させた関数
	 /// @param _vector3 Vector3
	 /// @param _name 変数名
	 /// @param _speed 速度
	 static void DragFloat3(DirectX::SimpleMath::Vector3& _vector3, const std::string& _name, float _speed = 1.0f);
	 
	 /// @brief ColorEdit4でColorに対応させた関数
	 /// @param _color Color
	 /// @param _name 変数名
	 static void ColorEdit4(DirectX::SimpleMath::Color& _color, const std::string& _name);

	 /// @brief フォントの大きさを変える
	 /// @param _size フォントの大きさ
	 static void SetFontSize(FontSize _size);

	 /// @brief フォントの大きさを戻す
	 static void PopFontSize();
private:

	/// @brief フォントを読み込む
	static void ImGuiLoadFonts();
};

