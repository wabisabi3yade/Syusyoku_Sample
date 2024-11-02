#pragma once

/// @brief アニメーションカーブによるパラメータ編集(エルミート曲線)
class AnimationCurve : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief エルミート曲線の点パラメータ
	struct HermitePlotParam
	{
		/// @brief 値
		float value{ 0.0f };

		/// @brief 時間
		float time{ 0.5f };

		/// @brief ベクトル
		float vector{ 0.5f };

		bool operator==(const HermitePlotParam& _other)
		{
			return (this->value == _other.value) &&
				(this->time == _other.time) &&
				(this->vector == _other.vector);
		}
	};

	/// @brief 曲線で使用している制御点
	std::list<HermitePlotParam> plotPoints;

	/// @brief イージング種類
	HashiTaku::EaseKind easeKind;

#ifdef EDIT
	// マウス編集中のプロット点
	HermitePlotParam* editingPlot{ nullptr };
	bool isVectorEdit{ false };
#endif

public:
	AnimationCurve();
	~AnimationCurve() {}

	/// @brief アニメーションカーブを取得する
	/// @param _time 時間
	/// @return 値
	float GetValue(float _time)  const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 時間からエルミート曲線を取得する
	/// @param _getTime 2点間の割合(0.0〜1.0)
	/// @param _p0 前の制御点
	/// @param _p1 後の制御点
	/// @return 時間の値
	float CalcHermiteCurve(float _getTime, const HermitePlotParam& _p0, const HermitePlotParam& _p1) const;

	/// @brief プロット点の時間を昇順にする
	/// @param _p0 前
	/// @param _p1 後
	/// @return 入れ替えるか？
	static bool SortPointTime(const HermitePlotParam& _p0, const HermitePlotParam& _p1);

	/// @brief プロット点を追加
	/// @param _time 時間
	void AddPlot(float _time);

	/// @brief プロット点を削除
	/// @param _deletePlot 削除するプロット点
	void RemovePlot(HermitePlotParam& _deletePlot);

	/// @brief クリックしたところが反応するかチェック
	/// @param _plot プロット
	/// @param _clickX クリックしたX座標
	/// @param clickY Y座標
	/// @return 反応する
	bool CheckReactPlot(const HermitePlotParam& _plot, float _clickX, float clickY);

	/// @brief イージング種類をセット
	/// @param _easeKind 種類
	void SetEase(HashiTaku::EaseKind _easeKind);

	void ImGuiSetting() override;

	// ImGuiでプロット点を編集
	void ImGuiPlotEditing();

	// ImGuiでベクトル点を編集
	void ImEditVectorPoint();

	// ImGuiでプロット点を追加・削除を行う
	void ImAddPopPlot();
};

