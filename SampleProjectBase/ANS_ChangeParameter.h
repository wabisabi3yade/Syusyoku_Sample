#pragma once
#include "AnimationNotifyState.h"
#include "IAnimParametersSetter.h"
#include "AnimationParameterType.h"

/// @brief アニメーション通知でパラメータを変更する(bool型しか変更する予定ないのでbool対応)
class ANS_ChangeParameter : public AnimationNotifyState, public IAnimParametersSetter
{
	//セクションの種類
	enum class SectionType
	{
		Init,	// 初期
		Tick,	// 更新中の値
		End,	// 終了
		Terminal, // 変更前終了処理
		Num
	};

	/// @brief セクション数
	static constexpr short SECTION_CNT = static_cast<short>(SectionType::Num);

	/// @brief 各セクションで使用されているか？
	std::array<bool, SECTION_CNT> isUsing;

	/// @brief 各セクションでセットするパラメータ
	std::array<bool, SECTION_CNT> setParameter;

	/// @brief 変更したいパラメータ名
	std::string changeParamName;

	/// @brief アニメーション内パラメータ
	AnimationParameters* pAnimationParameters;

	/// @brief 現在見ているパラメータの型
	HashiTaku::AnimParam::TypeKind parameterType;
public:
	ANS_ChangeParameter();
	~ANS_ChangeParameter() {};

	/// @brief アニメーションパラメータリストセット
	/// @param _animatiionParameters アニメーションリストパラメータ
	void SetAnimationParameters(AnimationParameters& _animatiionParameters) override;

	/// @brief パラメータを変更
	/// @param _parameterName 変更したいパラメータ名
	void ChangeAnimationParameter(const std::string& _parameterName);

	/// @brief クローン関数
	/// @return 生成した通知イベント
	std::unique_ptr<AnimationNotify_Base> Clone() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief イベント間の更新処理
	void Begin() override {};

	/// @brief イベント間の更新処理
	void Tick() override;

	/// @brief イベント終了処理
	void End() override;

	/// @brief 切り替えはじめの処理
	void OnInitialize() override;

	/// @brief 切り替え前終了処理
	void OnTerminal() override;

	/// @brief パラメータセットできるか
	/// @param _type セクション
	/// @return できるか？
	bool IsCanSet(SectionType _type);

	/// @brief セクションをshortに変換
	/// @param _type セクション
	/// @return short
	short CastShort(SectionType _type);

	void ImGuiDebug() override;
};

