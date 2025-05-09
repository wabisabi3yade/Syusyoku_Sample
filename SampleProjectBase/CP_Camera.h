#pragma once
#include "Component.h"

namespace HashiTaku
{
	// ゲーム内カメラクラス
	class CP_Camera : public Component
	{
		/// @brief カメラの上ベクトル
		DXSimp::Vector3 camUp{ Vec3::Up };

		/// @brief カメラの上ベクトル
		DXSimp::Vector3 target{ Vec3::Forward };

		/// @brief 視野角
		float fov;

		/// @brief 描画距離
		float distance;

		/// @brief 平行投影にするか？
		bool isOrthographic{ false };

		// ビューポートの番号
		u_int viewPortSlot;
	public:
		CP_Camera();
		virtual ~CP_Camera() {};

		virtual void Init() override;
		virtual void OnDestroy() override;

		/// @brief ビュー変換行列
		virtual void UpdateViewMatrix();

		/// @brief 透視投影に変更
		virtual void SetPerspective();

		/// @brief 平行投影に変更
		virtual void SetOrthographic();

		// fovセット
		virtual void SetFov(float _setFov);

		// ビューポート番号をセット
		virtual void SetViewportSlot(u_int _slot);

		/// @brief カメラの上ベクトル
		virtual const DXSimp::Vector3& GetCamUp() const;

		/// @brief カメラの座標
		virtual const DXSimp::Vector3& GetEyePos() const;

		/// @brief カメラの注視点
		virtual const DXSimp::Vector3& GetTarget() const;

		/// @brief 視野角を取得(deg)
		/// @return 視野角
		virtual float GetFov() const;

		void ImGuiDebug() override;

		/// @brief セーブする
		/// @param _sceneData セーブデータ
		json Save();

		/// @brief ロードする
		/// @param _sceneData ロードするデータ 
		void Load(const json& _data);

	protected:
		/// @brief 投影行列を更新する
		virtual void UpdateProjection();

		/// @brief 透視投影行列を更新
		virtual void UpdatePerspective();

		/// @brief 平行投影をセットする
		virtual void UpdateOrthographic();
	};

	/// @brief CP_CameraのNullオブジェクト
	class CP_CameraNull : public CP_Camera
	{
	public:
		CP_CameraNull() {}
		~CP_CameraNull() {}

		void Init() override {}

		void LateUpdate() override {}

		void Draw() override {}

		void ImGuiDebug() override {}

		/// @brief ビュー変換行列
		void UpdateViewMatrix() {}

		/// @brief 透視投影に変更
		void SetPerspective() {}

		/// @brief 平行投影に変更
		void SetOrthographic() {}

		// fovセット
		void SetFov(float _setFov) {}

		// ビューポート番号をセット
		void SetViewportSlot(u_int _slot) {}

		/// @brief カメラの上ベクトル
		const DXSimp::Vector3& GetCamUp() const override { return Vec3::Up; }

		/// @brief カメラの座標
		const DXSimp::Vector3& GetEyePos() const override
		{
			return DXSimp::Vector3::Zero;
		}

		/// @brief カメラの注視点
		const DXSimp::Vector3& GetTarget() const override
		{
			return Vec3::Forward;
		};

		float GetFov() const override { return 0.0f; }

		/// @brief セーブする
		/// @param _sceneData セーブデータ
		json Save() { return json(); }

		/// @brief ロードする
		/// @param _sceneData ロードするデータ 
		void Load(const json& _data) {}

	private:
		/// @brief 投影行列を更新する
		virtual void UpdateProjection() {};

		/// @brief 透視投影行列を更新
		virtual void UpdatePerspective() {};

		/// @brief 平行投影をセットする
		virtual void UpdateOrthographic() {};
	};
}