#pragma once
#include "SingletonComponent.h"
namespace HashiTaku
{
	/// @brief シーン内で使いやすいようにしたサウンドマネジャー
	class CP_SoundManager : public SingletonComponent<CP_SoundManager>
	{
		/// @brief サウンド大基のマネジャー
		DXSoundManager* pDXSoundManager{ nullptr };

		/// @brief 現在再生しているBGMのID
		int curPlayBGMId{ -1 };

	public:
		/// @brief 初期化処理
		void Awake() override;

		/// @brief 削除時処理
		void OnDestroy() override;

		/// @brief BGMを再生する
		/// @param _bgmParameter 再生するBGM
		int PlayBGM(const PlaySoundParameter& _bgmParameter);

		/// @brief SEを再生する
		/// @param _seParameter 再生するSE
		/// @param _soundPos seを再生する座標
		int PlaySE(const PlaySoundParameter& _seParameter, const DXSimp::Vector3& _soundPos = DXSimp::Vector3::Zero);
	};

}

