#pragma once

namespace HashiTaku
{
	/// @brief アニメーションカーブによるパラメータ編集(エルミート曲線)
	class AnimationCurve : public IImGuiUser, public ISaveLoad
	{
		// 補間の種類
		enum class InterpKind
		{
			Hermite, // エルミート
			Linear	// 線形
		};

		/// @brief エルミート曲線の点パラメータ
		struct HermitePlotParam
		{
			/// @brief 値
			float value{ 0.0f };

			/// @brief 時間
			float time{ 0.5f };

			/// @brief ベクトル
			float vector{ 0.5f };

			/// @brief 次までの補間方法
			InterpKind toNextInterp{ InterpKind::Linear };

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
		EaseKind easeKind;

		/// @brief エルミート曲線を使用するか？（使用しない場合、線形）
		bool isUsePlot;
#ifdef EDIT
		// カーブ名
		std::string curveName{ "animationCurve" };

		// 編集対象になっているかのプロット点
		HermitePlotParam* editingPlot{ nullptr };

		// 編集対象になっているかのプロット点
		HermitePlotParam* dragingPlot{ nullptr };

		// 速度ベクトル編集中か？
		bool isVectorEdit{ false };

		// 1フレーム前はドラッグ中か？
		bool prevIsDrag{ false };
#endif

	public:
		AnimationCurve();
		~AnimationCurve() {}

		// カーブ名をセット
		void SetCurveName(const std::string& _name);

		/// @brief アニメーションカーブを取得する
		/// @param _time 時間
		/// @return 値
		float GetValue(float _time)  const;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief 時間からエルミート曲線を取得する
		/// @param _getTime 2点間の割合(0.0～1.0)
		/// @param _p0 前の制御点
		/// @param _p1 後の制御点
		/// @return 時間の値
		float CalcHermiteCurve(float _getTime, const HermitePlotParam& _p0, const HermitePlotParam& _p1) const;

		/// @brief 時間からリニア曲線を取得する
		/// @param _getTime 2点間の割合(0.0～1.0)
		/// @param _p0 前の制御点
		/// @param _p1 後の制御点
		/// @return 時間の値
		float CalcLinearCurve(float _getTime, const HermitePlotParam& _p0, const HermitePlotParam& _p1) const;


		/// @brief プロット点の時間を昇順にする
		/// @param _p0 前
		/// @param _p1 後
		/// @return 入れ替えるか？
		static bool SortPointTime(const HermitePlotParam& _p0, const HermitePlotParam& _p1);

		/// @brief 各軸の最小値と最大値を取得する
		/// @return 各軸の最大値と最小値
		ImPlotRect GetPlotRect();

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
		bool CheckReactPlot(const HermitePlotParam& _plot);

		/// @brief クリックしたところが反応するかチェック
		/// @param _posXプロットX座標
		/// @param _posY プロットY座標
		/// @param _clickX クリックしたX座標
		/// @param clickY Y座標
		/// @return 反応する
		bool CheckReactPlot(float _posX, float _posY);

		/// @brief イージング種類をセット
		/// @param _easeKind 種類
		void SetEase(EaseKind _easeKind);

		/// @brief 始点と終点のプロットか確認
		/// @param _checkPlot 確認するプロット
		/// @return 始点または終点のプロットか？
		bool IsStartOrEndPlot(const HermitePlotParam* _checkPlot);

		void ImGuiDebug() override;

		// ImGuiでプロット点を編集
		void ImGuiPlotEditing();

		// ImGuiでベクトル点を編集
		void ImEditVectorPoint();

		// ImGuiでグラフを描画
		void ImDrawGraph();

		// ImGuiでプロット点を追加・削除を行う
		void ImAddPopPlot();
	};
}