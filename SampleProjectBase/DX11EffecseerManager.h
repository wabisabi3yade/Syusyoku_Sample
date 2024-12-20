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

		/// @brief エフェクシアの描画変数
		EffekseerRendererDX11::RendererRef renderer;

		/// @brief マネジャー変数
		Effekseer::ManagerRef manager;

		/// @brief D3D描画クラス
		D3D11_Renderer* pRenderer;
	public:
		/// @brief 初期化関数
		void Init();

		/// @brief 描画開始処理（カメラ行列更新前）
		void BeginDraw();

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
			const DirectX::SimpleMath::Vector3& _pos,
			const DirectX::SimpleMath::Vector3& _scale = DirectX::SimpleMath::Vector3::One,
			const DirectX::SimpleMath::Vector3& _eularAngles = DirectX::SimpleMath::Vector3::Zero,
			int _startFrame = 0);

		Effekseer::Handle Play(const CreateVfxInfo& _createVfx,
			const DirectX::SimpleMath::Vector3& _pos,
			const DirectX::SimpleMath::Vector3& _eularAngles = DirectX::SimpleMath::Vector3::Zero);

		/// @brief マネジャーを取得
		/// @return エフェクシアマネジャー変数
		const Effekseer::ManagerRef& GetManager() const;
	private:
		DX11EffecseerManager();
		~DX11EffecseerManager();

		/// @brief カメラ行列をセット
		void UpdateCamMatrix();

		/// @brief エフェクシアの行列を作成
		/// @param _dxMtx DIrectXの行列
		/// @param エフェクシアの行列
		void CreateEffekseerMtx(const DirectX::SimpleMath::Matrix& _dxMtx,
			Effekseer::Matrix44& _outMtx);

		/// @brief エフェクシアのVector3を作成
		/// @param _dxMtx DIrectXのVector3
		/// @param エフェクシアのVector3
		Effekseer::Vector3D CreateEffekseerVector3(const DirectX::SimpleMath::Vector3& _dxVec3);
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

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;

		void ImGuiDebug() override;
	};
}