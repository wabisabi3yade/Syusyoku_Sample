#pragma once
// イージング基底クラス
class Ease_Base;

// 初期化を任せるクラス
class SceneManager;

// イージング関数チートシート：https://easings.net/ja
namespace HashiTaku
{
	// イージング関数の戻り値として
	using EaseFunction = std::function<float(float)>;

	/// @brief イージングの種類
	enum class EaseKind
	{
		Linear,
		Sine,
		Quad,
		Cubic,
		Quart,
		Back,
		Elastic,
		Bounce,
		MaxNum
	};

	enum class EaseFuncKind
	{
		In,
		Out,
		InOut
	};

	struct EaseType
	{
		EaseKind ease;
		EaseFuncKind funcType;
	};

	/// @brief イージング関数
	class Easing
	{
		friend class SceneManager;

		/// @brief イージングのクラスをまとめたリスト
		static std::vector<std::unique_ptr<Ease_Base>> easeList;
		/// @brief stringと相互変換したいので辞書リスト(イージング種類)
		static std::unordered_map<std::string, EaseKind> easeDictionary;
		/// @brief stringと相互変換したいので辞書リスト(イージング関数)
		static std::unordered_map<std::string, EaseFuncKind> funcDictionary;

	public:
		/// @brief 指定したイージング関数を返す
		/// @param _easeFunc 格納する関数オブジェクト変数
		/// @param _getEase 取得したいイージング名
		/// @param _funcType 関数の種類
		static void GetEase(EaseFunction& _easeFunc, EaseType _easeType);

		/// @brief ImGui設定
		/// @param _easeFunc 関数オブジェクト
		/// @param _nowEase 現在のイージング
		static void ImGuiSelect(EaseFunction& _easeFunc, EaseType& _nowEase);

	private:
		/// @brief 初期化　シーンマネージャーで
		static void Init();

		/// @brief イージング作成（初期化）
		/// @param _easeId 作成するイージング
		static void MakeEase(u_int _easeId);

		// イージングの種類からstringの変換
		static std::string EaseToString(EaseKind _type);

		// 関数の種類からstringの変換
		static std::string FuncToString(EaseFuncKind _type);

		// 名前配列を取得
		static std::vector<std::string> GetEaseNames();
		static std::vector<std::string> GetFuncNames();
	};
}