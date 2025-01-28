#pragma once
#include "Component.h"

namespace HashiTaku
{
	class CP_UIRenderer;
	class CP_UISlider;

	/// @brief スタイリッシュランクを制御するクラス
	class CP_StylishUI : public Component
	{
		/// @brief ランク
		enum class StylishRank
		{
			None,	// なにもなし(一番下)
			D,
			C,
			B,
			A,
			S,
			SS,
			SSS,
			MaxNum
		};

		/// @brief ランクごとのパラメータ
		struct RankParameter
		{
			/// @brief 画面に表示するテクスチャ
			const Texture* displayTexture{ nullptr };

			/// @brief このランクに入るためのポイント数
			float requireEnterPoints{ 0.0f };

			/// @brief 減るときのポイント速度
			float decadePointSpeed{ 100.0f };
		};

		/// @brief ランクのパラメータリスト
		std::array<RankParameter, static_cast<u_int>(StylishRank::MaxNum)> rankParameters;

		/// @brief ランク描画オブジェクト名
		std::string rankUIRendererName;

		/// @brief スタイリッシュゲージオブジェクト名
		std::string stylishGageName;

		/// @brief UI描画
		CP_UIRenderer* pUIRenderer;

		/// @brief スタイリッシュゲージ
		CP_UISlider* pStylishGage;

		/// @brief 現在のスタイリッシュランク
		StylishRank curStylishRank;

		/// @brief 現在のスタイリッシュポイント
		float curStylishPoint;

		/// @brief この時間が経過したら減っていくようにする
		float decadeBeginTime;

		/// @brief 減るまでの経過時間
		float elapsedToDecadeTime;

		/// @brief ポイントが減り続けているか？
		bool isPointDecading;
	public:
		CP_StylishUI();
		~CP_StylishUI() {}

		/// @brief スタイリッシュポイントを加算
		/// @param _addPoint 加算するポイント 
		void AddStylishPoint(float _addPoint);

		/// @brief スタイリッシュポイントを減算
		/// @param _decadePoint 減算するポイント 
		void DecadeStylishPoint(float _decadePoint);
	private:
		void Update() override;

		/// @brief ポイントを減らす更新処理
		void DecadePointUpdate();

		/// @brief 入るためのランクポイントを取得
		/// @param _checkRank 確認したいランク
		/// @return ランクポイント
		float GetEnterRankPoint(StylishRank _checkRank) const;

		/// @brief ランクを変更する
		void ChangeRank(StylishRank _targetRank);

		/// @brief 現在のポイントをゲージに反映
		void ApplyStylishGage();

		/// @brief 活動できるか取得
		/// @return 活動できるか？
		bool GetCanActive() const;

		/// @brief 次のランクを取得する
		/// @param _targetRank 次のランクを見たいランク
		/// @return 次のランク
		StylishRank GetNextRank(StylishRank _targetRank) const;

		/// @brief 前のランクを取得する
		/// @param _targetRank 前のランクを見たいランク
		/// @return 前のランク
		StylishRank GetPrevRank(StylishRank _targetRank) const;

		/// @brief 現在最大のランク状態か取得する
		/// @return 現在最大のランク状態か？
		bool GetIsCurMaxRank() const;

		void ImGuiDebug() override;
	};
}

