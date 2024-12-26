#pragma once
#include "Singleton_Base.h"
#include "SoundAsset.h"

// XTK Audio URL:https://github.com/microsoft/DirectXTK/wiki/Audio

namespace HashiTaku
{
	/// @brief サウンドを再生するときのパラメータ
	class PlaySoundParameter : public IImGuiUser, public ISaveLoad
	{
	public:
		/// @brief 再生するサウンド
		SoundAsset* pSound;

		/// @brief 音量(0.0～1.0)
		float volume;

		/// @brief 音の高さ(-1.0～1.0、等速0.0)
		float pitch;

		/// @brief ループ再生するか？
		bool isLoop;

		/// @brief リスナーで3D空間上の音を再生するか？
		bool isUseLisneter;
	public:
		PlaySoundParameter();
		~PlaySoundParameter() {}

		json Save() override;
		void Load(const json& _data) override;

	private:
		void ImGuiDebug() override;
	};

	/// @brief サウンドマネジャー
	class DXSoundManager : public Singleton_Base<DXSoundManager>
	{
		friend class MainApplication;	// 初期化を行うため
		friend class Singleton_Base<DXSoundManager>;	// 自身のsインスタンス作成のため

		/// @brief 再生中のサウンドパラメータ
		struct PlaySoundInstance
		{
			/// @brief 再生中のサウンドインデックス
			int soundId{ -1 };

			/// @brief サウンドインスタンス
			std::unique_ptr<DX::SoundEffectInstance> pSoundInstance;
		};

		/// @brief リソースを作成するエンジン
		std::unique_ptr<DirectX::AudioEngine> pAudioEngine;

		/// @brief 3D空間で再生するときのリスナー(基本カメラ)
		const DX::AudioListener* pAudioLisner;

		/// @brief 再生中のサウンドリスト
		std::list<PlaySoundInstance> playSoundInstances;
	public:
		/// @brief 更新処理
		void Update();

		/// @brief 再生処理
		/// @param _playSound 再生したい音
		/// @param _volume 音量(1.0が等倍)
		/// @param _pitch 音の高さ
		/// @param _isLoop ループするか？
		/// @param _useListener 3D音響を使用するか
		/// @param _soundPos サウンドのワールド座標
		/// @return 再生しているサウンドのID
		int Play(const SoundAsset& _playSound,
			float _volume = 0.5f,
			float _pitch = 0.0f,
			bool _isLoop = false,
			bool _useListener = false,
			const DXSimp::Vector3& _soundPos = DXSimp::Vector3::Zero);

		/// @brief 再生処理
		/// @param _soundParameter 再生するサウンド情報
		/// @param _soundPos サウンドのワールド座標
		/// @return 再生しているサウンドのID
		int Play(const PlaySoundParameter& _soundParameter,
			const DXSimp::Vector3& _soundPos = DXSimp::Vector3::Zero);

		/// @brief 再生中の音を全て停止する
		void StopAll();

		/// @brief 指定したサウンドを止める
		/// @param _soundId サウンドID
		void StopSound(int _soundId);

		/// @brief 3D空間で再生するときのリスナーをセット
		/// @param _pSetListener リスナー
		void SetAudioListener(const DX::AudioListener& _pSetListener);

		/// @brief リスナーを削除する
		void RemoveAudioListener();

		/// @brief エンジンを取得(ロード時に必要)
		/// @return エンジン
		DirectX::AudioEngine& GetEngine();

	private:
		DXSoundManager();
		~DXSoundManager();

		/// @brief 初期化
		void Init();

		/// @brief 再生中の音が終わっているか確認
		void CheckEndSound();

		/// @brief 空いているインデックス番号を取得する
		/// @return 空いているインデックス番号
		int GetEmptyIndex();
	};

}

