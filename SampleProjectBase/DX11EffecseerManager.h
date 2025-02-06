#pragma once
#include "Singleton_Base.h"

namespace HashiTaku
{
	class CreateVfxInfo;
	class VisualEffect;

	/// @brief DX11環境で初期化や解放などを行うクラス
	class DX11EffecseerManager : public Singleton_Base<DX11EffecseerManager>
	{
	private:
		friend class Singleton_Base<DX11EffecseerManager>;

		struct PlayHandleParameter
		{
			/// @brief エフェクトのハンドル
			Effekseer::Handle handle{ -1 };

			/// @brief ベースの再生速度
			float baseSpeed{ 1.0f };
		};

		/// @brief 現在再生しているエフェクトのハンドルリスト
		std::list<PlayHandleParameter> playingHandleList;

		/// @brief エフェクシアの描画変数
		EffekseerRendererDX11::RendererRef renderer;

		/// @brief マネジャー変数
		Effekseer::ManagerRef manager;

		/// @brief D3D描画クラス
		D3D11_Renderer* pRenderer;

		/// @brief 前フレームのタイムスケール
		float prevTimeScale;
		
		/// @brief エフェクトを完全に停止させる
		bool isPause;

	public:
		/// @brief エフェクトの何もなしを表すハンドル
		static constexpr int NONE_VFX_HANDLE{ -9999 };


#ifdef EDIT
		// エフェクトを表示させるか？
		bool isVfxDisplay{ true };
#endif // EDIT
	public:
		/// @brief 初期化関数
		void Init();

		/// @brief 状態をリセットする
		void Reset();

		/// @brief 描画する
		void EffectDraw();

		/// @brief 更新処理
		void Update();

		/// @brief 再生中のエフェクトを削除
		void AllEffectDestroy();

		/// @brief エフェクトが存在しているか取得
		/// @param _vfxHandle 確認するエフェクトのハンドル
		/// @return 存在しているか？
		bool ExistEffect(Effekseer::Handle _vfxHandle) const;

		/// @brief エフェクトを再生する
		/// @param _effect 再生するエフェクト
		/// @param _playSpeed 再生速度
		/// @param _pos 座標
		/// @param _scale スケール
		/// @param _eularAngles 角度
		/// @param _startFrame 再生開始フレーム
		/// @return 再生しているハンドル
		Effekseer::Handle Play(
			const Effekseer::EffectRef& _effect,
			float _playSpeed,
			const DXSimp::Vector3& _pos,
			const DXSimp::Vector3& _scale = DXSimp::Vector3::One,
			const DXSimp::Vector3& _eularAngles = DXSimp::Vector3::Zero,
			int _startFrame = 0);

		Effekseer::Handle Play(const CreateVfxInfo& _createVfx,
			const DXSimp::Vector3& _pos,
			const DXSimp::Vector3& _eularAngles = DXSimp::Vector3::Zero);

		/// @brief 全てのエフェクトを停止する
		void SetPause(bool _setPause);

		/// @brief 色を変更するなら
		/// @param _efkHandle エフェクトのハンドル
		/// @param _color 色
		void ChangeColor(Effekseer::Handle _efkHandle, const DXSimp::Color& _color);

		/// @brief 指定したエフェクトを削除する
		/// @param _deleteHandle 削除するエフェクトハンドル
		void DestroyVfx(const Effekseer::Handle _deleteHandle);

		/// @brief マネジャーを取得
		/// @return エフェクシアマネジャー変数
		const Effekseer::ManagerRef& GetManager() const;
	private:
		DX11EffecseerManager();
		~DX11EffecseerManager();

		/// @brief エフェクト全体のタイムスケールをシーンに合わせる
		void UpdateTimeScale();

		/// @brief エフェクトが存在しているかどうかチェック
		void UpdateCheckExist();

		/// @brief カメラ行列をセット
		void UpdateCamMatrix();

		/// @brief カメラを向く角度を求める
		/// @param _efkPos エフェクトの座標
		/// @param _outAngles 結果
		void CalcLookCameraAngles(const DXSimp::Vector3& _efkPos, DXSimp::Vector3& _outAngles);

		/// @brief エフェクシアの行列を作成
		/// @param _dxMtx DIrectXの行列
		/// @param エフェクシアの行列
		void CreateEffekseerMtx(const DXSimp::Matrix& _dxMtx,
			Effekseer::Matrix44& _outMtx);

		/// @brief エフェクシアのVector3を作成
		/// @param _dxMtx DIrectXのVector3
		/// @param エフェクシアのVector3
		Effekseer::Vector3D CreateEffekseerVector3(const DXSimp::Vector3& _dxVec3);
	};

	/// @brief エフェクト生成情報
	class CreateVfxInfo : public IImGuiUser, public ISaveLoad
	{
	public:
		/// @brief エフェクト色
		DXSimp::Color effectColor{ DXSimp::Vector4::One };

		/// @brief エフェクト名
		VisualEffect* pHitVfx{ nullptr };

		/// @brief  スケール
		float scale{ 1.0f };

		/// @brief 速度
		float speed{ 1.0f };

		/// @brief 開始フレーム
		int startFrame{ 0 };

		/// @brief 指定色を反映する
		bool isApplyColor{ false };

		/// @brief カメラを向き続けるか？
		bool isLookCamera{ false };

		json Save() override;
		void Load(const json& _data) override;

		void ImGuiDebug() override;
	};
}