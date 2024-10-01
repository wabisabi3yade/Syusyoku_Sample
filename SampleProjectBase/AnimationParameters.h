#pragma once
#include "AnimationParameterType.h"
#include "IObserever.h"

// 前方宣言
namespace HashiTaku
{
	namespace AnimParam
	{
		struct NotificationData;
	}
}

/// @brief アニメーションコントローラー内で使用するパラメータリスト
class AnimationParameters : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief パラメータリスト
	std::unordered_map<std::string, HashiTaku::AnimParam::conditionValType> animParameters;

	/// @brief トリガーの状態をリセットための配列
	std::list<TriggerType*> pResetTriggers;

	/// @brief パラメータ削除時の通知サブジェクト
	std::unique_ptr<HashiTaku::Subject<HashiTaku::AnimParam::NotificationData>> pAnimParamSubject;
public:
	AnimationParameters();
	~AnimationParameters() {}

	/// @brief パラメータを追加する
	/// @tparam T パラメータの型
	/// @param _paramNames パラメータ名
	template<HashiTaku::AnimParam::AnimParamConcept T>
	void AddParameter(std::string _paramNames = "new Parameter");

	/// @brief パラメータの名前を付け替える
	/// @param _prevName 変更前の名前
	/// @param _changeName 変更後の名前
	void RenameParameter(const std::string& _prevName, std::string _changeName);

	/// @brief パラメータを削除する
	/// @param _paramNames パラメータ名
	/// @return 削除したマップ配列の次のイテレータ
	void RemoveParameter(const std::string& _paramNames);

	/// @brief トリガーの状態をリセットする
	void ResetTrigger();

	/// @brief 指定したbool変数に値をセット
	/// @param _paramName パラメーター名
	/// @param _isBool セットする値
	void SetBool(const std::string& _paramName, bool _isBool);

	/// @brief 指定したint変数に値をセット
	/// @param _paramName パラメーター名
	/// @param _intVall セットする値
	void SetInt(const std::string& _paramName, int _intVal);

	/// @brief 指定したfloat変数に値をセット
	/// @param _paramName パラメーター名
	/// @param _floatVal セットする値
	void SetFloat(const std::string& _paramName, float _floatVal);

	/// @brief 指定したトリガー変数をtrueにする
	/// @param _paramName パラメーター名
	void SetTrigger(const std::string& _paramName);

	/// @brief 指定したbool変数に値を取得
	/// @param _paramName パラメーター名
	/// @param _isBool 取得する値
	bool GetBool(const std::string& _paramName);

	/// @brief 指定したint変数に値を取得
	/// @param _paramName パラメーター名
	/// @param _intVall 取得する値
	int GetInt(const std::string& _paramName);

	/// @brief 指定したfloat変数に値を取得
	/// @param _paramName パラメーター名
	/// @param _floatVal 取得する値
	float GetFloat(const std::string& _paramName);

	/// @brief 指定したトリガー変数の状態を取得
	/// @param _paramName パラメーター名
	/// @param _floatVal リガー変数の状態
	bool GetTrigger(const std::string& _paramName);

	/// @brief パラメータ名のリストを取得する
	/// @param _nameList 代入する名前配列
	void GetNameList(std::vector<const std::string*>& _nameList) const;

	/// @brief パラメータ名のリストを取得する
	/// @param _nameList 代入する名前配列
	void GetFloatNameList(std::vector<const std::string*>& _nameList) const;

	/// @brief 名前から値のアドレスを返す
	/// @param _name 名前
	/// @return 値のアドレス(ないならnullptr)
	const HashiTaku::AnimParam::conditionValType* GetValueAddress(const std::string& _name) const;

	/// @brief 名前と一致したパラメータ名のアドレスを返す
	/// @param _name 探索するの名前
	/// @return パラメータ名のアドレスのアドレス(ないならnullptr)
	const std::string* GetNameAddress(const std::string& _name) const;

	// パラメータの個数を取得
	u_int GetParameterCnt() const;

	/// @brief パラメータ削除サブジェクトを取得
	/// @return サブジェクトの参照
	HashiTaku::Subject<HashiTaku::AnimParam::NotificationData>& GetParamRemoveSubject();

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief コントローラー内でパラメータ名が被らないようにする
	/// @param _nodename 変更したいパラメータ名
	void NotDuplicateParamName(std::string& _paramName);

	/// @brief 不正な値を参照しているかチェックする(bool)
	/// @param _name パラメーター名
	/// @return 正常な値であるか？
	bool CheckBool(const std::string& _name);

	/// @brief 不正な値を参照しているかチェックする(int)
	/// @param _name パラメーター名
	/// @return 正常な値であるか？
	bool CheckInt(const std::string& _name);

	/// @brief 不正な値を参照しているかチェックする(float)
	/// @param _name パラメーター名
	/// @return 正常な値であるか？
	bool CheckFloat(const std::string& _name);

	/// @brief 不正な値を参照しているかチェックする(トリガー)
	/// @param _name パラメーター名
	/// @return 正常な値であるか？
	bool CheckTrigger(const std::string& _name);

	/// @brief タイプを判別する
	/// @param _parameter パラメーター
	/// @return 判別タイプ
	HashiTaku::AnimParam::TypeKind GetType(const HashiTaku::AnimParam::conditionValType& _parameter);

	void ImGuiSetting() override;
	// ImGuiでパラメータを新規に追加する
	void ImGuiAddParam();
	// ImGuiでパラメータを表示
	void ImGuiDisplay();
};


template<HashiTaku::AnimParam::AnimParamConcept T>
inline void AnimationParameters::AddParameter(std::string _paramNames)
{
	// 名前を複しないようにする
	NotDuplicateParamName(_paramNames);

	HashiTaku::AnimParam::conditionValType addParam;

	if constexpr (std::is_same<T, bool>::value)
		addParam = false;

	else if constexpr (std::is_same<T, int>::value)
		addParam = 0;

	else if constexpr (std::is_same<T, float>::value)
		addParam = 0.0f;

	else if constexpr (std::is_same<T, TriggerType>::value)
	{
		TriggerType t;
		addParam = t;
	}

	animParameters[_paramNames] = addParam;

	// トリガー型ならリセット配列に追加
	if (TriggerType* pTrigger = std::get_if<TriggerType>(&animParameters[_paramNames]))
		pResetTriggers.push_back(pTrigger);
}
