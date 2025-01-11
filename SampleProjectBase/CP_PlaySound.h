#pragma once
#include "Component.h"

namespace HashiTaku
{
	/// @brief 開始時に音を再生する
	class CP_PlaySound : public Component
	{
		/// @brief 再生する音のパラメータ
		PlaySoundParameter soundParameter;

		/// @brief 再生した時のID
		int soundId;

		/// @brief BGMとして再生するか？
		bool isBGM;
	public:
		CP_PlaySound();
		~CP_PlaySound() {}

		/// @brief 削除時処理
		void OnDestroy() override;

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const json& _data) override;
	private:
		/// @brief 開始処理
		void Start() override;

		void ImGuiDebug() override;
	};
}

