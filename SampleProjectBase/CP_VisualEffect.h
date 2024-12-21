#pragma once
#include "Component.h"
#include "VisualEffect.h"
#include "DX11EffecseerManager.h"

namespace HashiTaku
{
	/// @brief エフェクトを再生するコンポーネント
	class CP_VisualEffect : public Component
	{
		/// @brief DirectXのエフェクトマネジャー
		DX11EffecseerManager* pDxVfxManager;

		/// @brief 再生するエフェクト
		const VisualEffect* pVFX;

		/// @brief 再生するエフェクトのハンドル
		Effekseer::Handle vfxHandle;

		/// @brief 再生速度
		float playSpeed;

		/// @brief 現在のフレーム
		float currentPlayFrame;

		/// @brief 再生開始フレーム
		int playStartFrame;

		/// @brief 再生終了フレーム
		int playEndFrame;

		/// @brief 再生中か？
		bool isPlaying;

		/// @brief ループ再生するか？
		bool isLoop;

		/// @brief 再生時間の範囲指定するか？
		bool isTrimming;
	public:
		CP_VisualEffect();
		~CP_VisualEffect() {}

		void Init() override;

		/// @brief 再生速度を変更
		/// @param _playSpeed 再生速度
		void SetPlaySpeed(float _playSpeed);

		/// @brief エフェクトをセット
		/// @param _setVfx エフェクト
		void SetVisualEffect(const VisualEffect* _setVfx);

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const json& _data) override;
	private:
		void Start() override;

		void Update() override;

		/// @brief トリミング関連の更新処理
		void TrimmingUpdate();

		/// @brief 終了した時の処理
		void PlayEndUpdate();

		/// @brief エフェクトを再生開始処理
		void BeginPlayVFX();

		/// @brief エフェクト再生停止する
		void StopPlayVFX();

		void ImGuiDebug() override;
	};
}