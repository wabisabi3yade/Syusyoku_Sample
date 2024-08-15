#pragma once
// イージング基底クラス
class IEasing;

// 初期化を任せるクラス
class SceneManager;

// イージング関数チートシート：https://easings.net/ja
namespace HashiTaku
{
	/// @brief イージングの種類
	enum class EaseKind
	{
		Linear,

		InSine,
		OutSine,
		InOutSine,

		InQuad,
		OutQuad,
		InOutQuad,

		InCubic,
		OutCubic,
		InOutCubic,

		InQuart,
		OutQuart,
		InOutQuart,

		InQuint,
		OutQuint,
		InOutQuint,

		InExpo,
		OutExpo,
		InOutExpo,

		InCirc,
		OutCirc,
		InOutCirc,

		InBack,
		OutBack,
		InOutBack,

		InElastic,
		OutElastic,
		InOutElastic,

		InBounce,
		OutBounce,
		InOutBounce,

		MaxNum
	};

	/// @brief イージング関数
	class Easing
	{
		friend class SceneManager;

		/// @brief イージングのクラスをまとめたリスト
		static std::unordered_map<EaseKind, std::unique_ptr<IEasing>> easeList;

		/// @brief イージングの列挙型と名前との対応表
		static std::vector<std::string> easeNames;
	public:
		/// @brief イージングの値を取得
		/// @param _ratio 割合(0.0～1.0)
		/// @param _easeType イージング種類
		/// @return イージングの計算値
		static float EaseValue(float _ratio, EaseKind _easeType);

		/// @brief ImGui設定
		/// @param _easeFunc 関数オブジェクト
		/// @param _nowEase 現在のイージング
		static void ImGuiSelect(EaseKind& _nowEase);

	private:
		/// @brief 初期化　シーンマネージャーで
		static void Init();

		/// @brief イージング作成（初期化）
		/// @param _easeId 作成するイージング
		static void MakeEase(u_int _easeId);

		/// @brief string型から列挙型に変換
		/// @param _easeName イージング名
		/// @return イージングの種類
		static EaseKind StrToEase(const std::string& _easeName);
	};
}