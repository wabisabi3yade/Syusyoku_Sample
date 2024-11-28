#pragma once
#include "Singleton_Base.h"

class InSceneSystemManager;

/// @brief DX11環境で初期化や解放などを行うクラス
class DX11EffecseerManager : public Singleton_Base<DX11EffecseerManager>
{
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
	/// @param _pos 座標
	/// @param _scale スケール
	/// @return 再生しているハンドル
	Effekseer::Handle Play(
		const Effekseer::EffectRef& _effect,
		const DirectX::SimpleMath::Vector3& _pos,
		const DirectX::SimpleMath::Vector3& _scale);

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

