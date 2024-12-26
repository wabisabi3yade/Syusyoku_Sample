#pragma once

struct ImFont;


namespace HashiTaku
{
	/// @brief InputTextで使用するバッファ数
	constexpr u_int IM_INPUT_BUF(256);

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
		/// @return 変更したか？
		static bool DragFloat3(DXSimp::Vector3& _vector3, const std::string& _name, float _speed = 1.0f);

		/// @brief ColorEdit4でColorに対応させた関数
		/// @param _color Color
		/// @param _name 変数名
		static void ColorEdit4(DXSimp::Color& _color, const std::string& _name);


		/// @brief map配列用コンボボックス作成
		/// @param _caption キャプション
		/// @param _currentItem 現在選択中の名前
		/// @param _items 要素の全名前
		/// @return 変更したか？
		static bool ComboBox(const std::string& _caption, std::string& _currentItem, const std::vector<const std::string*>& _items);

		/// @brief enum用コンボボックス作成
		/// @param _caption キャプション
		/// @param _enumId enumの数字
		/// @param _items 列挙型の名前
		/// @return 変更したか？
		static bool ComboBox(const std::string& _caption, u_int& _enumId, const std::vector<std::string>& _items);
		static bool ComboBox(const std::string& _caption, std::string& _currentItem, const std::vector<std::string>& _items);

		/// @brief ツリーノード
		/// @return ノードが開かれているか？
		static bool TreeNode(const std::string& _caption);

		/// @brief クリックすると、テキスト入力できる
		/// @param _caption キャプション
		/// @param _text 変更したいテキスト
		/// @param _isEditing 編集中かどうか
		/// @return ノードが開かれているか？
		static bool EditableText(const std::string& _caption, std::string& _text, bool& _isEditing);

		/// @brief Vector2をテキスト
		/// @param _v 表示するテキスト
		static void Text(const DXSimp::Vector2& _v);

		/// @brief Vector3をテキスト
		/// @param _v 表示するテキスト
		static void Text(const DXSimp::Vector3& _v);

		/// @brief Quaternionをテキスト
		/// @param _q 表示するテキスト
		static void Text(const DXSimp::Quaternion& _q);

		/// @brief boolをテキスト
		/// @param _b 表示するテキスト
		/// 

		/// @brief boolをテキスト
		/// @param _text 表示するテキスト
		/// @param _b 表示させたいパラメータ
		static void Text(const std::string& _text, const bool _b);

		// ランダムなユニークIDをセット
		//static void PushRandID();

		/// @brief 行の空白を挿入する(小サイズ)
		static void LineSpaceSmall();

		/// @brief 行の空白を挿入する(中サイズ)
		static void LineSpaceMid();

		/// @brief 入力フィールドのサイズを調整する
		static void PushItemSmallWidth();

		/// @brief フォントの大きさを変える
		/// @param _size フォントの大きさ
		static void SetFontSize(FontSize _size);

		/// @brief フォントの大きさを戻す
		static void PopFontSize();
	private:

		/// @brief フォントを読み込む
		static void ImGuiLoadFonts();
	};
}