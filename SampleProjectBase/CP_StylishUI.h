#pragma once
#include "Component.h"
#include "PerlinShake.h"

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
			None = 0,	// なにもなし(一番下)
			D,
			C,
			B,
			A,
			S,
			SS,
			SSS,
			MaxNum
		};

		enum class RankupDirectionState
		{
			Wait,	// 待機
			Scaling,	// スケーリング
			Shaking	// シェイク
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

		/// @brief ランク上がったときに文字をシェイクするときの計算クラス
		std::unique_ptr<PerlinShake> pRankupShakeVector;

		/// @brief ランク上がったときのシェイクパラメータ
		PerlinShakeParameter rankupShakeParam;

		/// @brief スタイリッシュゲージオブジェクト名
		std::string stylishGageName;

		/// @brief スタイリッシュゲージグループオブジェクト名
		std::string gageGroupObjName;

		/// @brief ランク描画
		CP_UIRenderer* pStylishRankRenderer;

		/// @brief スタイリッシュゲージ
		CP_UISlider* pStylishGage;

		/// @brief ゲージのグループオブジェクト
		GameObject* pGageGroupObj;

		/// @brief 現在のスタイリッシュランク
		StylishRank curStylishRank;

		/// @brief ランクUIの基礎座標
		DXSimp::Vector3 rankUIBasePos;

		/// @brief ランクUIの基礎スケール
		DXSimp::Vector3 rankUIBaseScale;

		/// @brief 現在のスタイリッシュポイント
		float curStylishPoint;

		/// @brief この時間が経過したら減っていくようにする
		float decadeBeginTime;

		/// @brief 減るまでの経過時間
		float elapsedToDecadeTime;

		/// @brief 現在のランク演出状態
		RankupDirectionState curRankupDirState;

		/// @brief ランク上がったときに使用するイージング
		EaseKind rankupEase;

		/// @brief ランク上がったときのスケール変更時間
		float rankupScalingTime;
		
		/// @brief ランクの上がったときの演出での開始スケール倍率
		float rankupScalingRatio;

		/// @brief ランク上がったときの演出経過時間
		float elapsedRankupDirectionTime;
	public:
		CP_StylishUI();
		~CP_StylishUI() {}

		/// @brief スタイリッシュポイントを加算
		/// @param _addPoint 加算するポイント 
		void AddStylishPoint(float _addPoint);

		/// @brief スタイリッシュポイントを減算
		/// @param _decadePoint 減算するポイント 
		void DecadeStylishPoint(float _decadePoint);

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		json Save() override;

		// ランクパラメータのセーブ処理
		json SaveRankParameters();

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const json& _data) override;

		// ランクパラメータのロード処理
		void LoadRankParameters(const json& _rankParamData);
	private:
		void Start() override;
		void Update() override;

		/// @brief ランク上がったときに必要なパラメータの初期化
		void InitRankupParam();

		/// @brief 必要なオブジェクトを取得
		void FindRequireObject();

		/// @brief ポイントを減らす更新処理
		void DecadePointUpdate();

		/// @brief ランク上がったときの更新処理
		void RankupDirectionUpdate(); 

		/// @brief ランク上がったときのスケーリング処理
		void RankupScalingUpdate();
	
		/// @brief ランク上がったときのシェイク処理
		void RankupShakeUpdate();

		/// @brief 入るためのランクポイントを取得
		/// @param _checkRank 確認したいランク
		/// @return ランクポイント
		float GetEnterRankPoint(StylishRank _checkRank) const;

		/// @brief ランクを変更する
		void ChangeRank(StylishRank _targetRank);

		/// @brief 現在のポイントをゲージに反映
		void ApplyStylishGage();

		/// @brief ランクが上がったときの画像の演出
		void BeginRankUpDirection();

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

		/// @brief 現在のランクをポイントから取得する
		/// @return 現在のランク
		StylishRank GetCurrentRankFromPoint();

		/// @brief 現在最大のランク状態か取得する
		/// @return 現在最大のランク状態か？
		bool GetIsCurMaxRank() const;

		void ImGuiDebug() override;

		// ランク上がったとき関連
		void ImGuiRankupDirection();

		// ランクパラメータ編集
		void ImGuiRankParameter();

		// オブジェクト名編集
		void ImGuiSetObject();
	};
}

