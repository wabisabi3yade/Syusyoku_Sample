#pragma once

namespace HashiTaku
{
	// 描画
	class StaticMesh;
	class Material;
	class VertexShader;
	class PixelShader;

	// 当たり判定の描画などを手軽にするクラス(デバッグ用)
	class Geometory
	{
	public:

		/// @brief 図形の種類
		enum class GeoType
		{
			Cube,	// 立方体
			WireCube,	// ワイヤー立方体
			Sphere	// 球
		};

		/// @brief 線描画パラメータ
		struct LineVertex
		{
			DirectX::SimpleMath::Vector3 position;	// 座標
			DirectX::SimpleMath::Color color;	// 色
		};
	private:
		// Material
		static Material* pMaterial;

		/// @brief ワールド座標
		static DirectX::SimpleMath::Vector3 position;

		/// @brief 大きさ
		static DirectX::SimpleMath::Vector3 scale;

		/// @brief 角度
		static DirectX::SimpleMath::Vector3 eularAngle;

		// 色
		static DirectX::SimpleMath::Color color;

		// 基本図形
		static std::vector<StaticMesh*> pGeometory;

		/// @brief 描画する線リスト
		static std::vector<LineVertex> drawLines;

		/// @brief 線描画のシェーダー
		static VertexShader* pLineVs;
		static PixelShader* pLinePs;

		Geometory() {};
		~Geometory() {};
	public:
		static void Init();	// 初期化
		static void Release();

		// 座標を設定
		static void SetPosition(const DirectX::SimpleMath::Vector3& _position)
		{
			position = _position;
		}
		// 回転を設定
		static void SetRotation(const DirectX::SimpleMath::Vector3& _eularAngle)
		{
			eularAngle = _eularAngle;
		}
		// 大きさを設定
		static void SetScale(const DirectX::SimpleMath::Vector3& _scale)
		{
			scale = _scale;
		}
		// 色
		static void SetColor(const DirectX::SimpleMath::Color& _color) { color = _color; }

		static void DrawCube(bool _isWireFrame = false);	// キューブ描画

		static void DrawSphere(bool _isWireFrame = false);	// 球描画

		static void AddLine(const DirectX::SimpleMath::Vector3& _start, const DirectX::SimpleMath::Vector3& _end, const DirectX::SimpleMath::Color& _color);

		/// @brief 線描画
		static void DrawLine();

	private:
		/// @brief マテリアルを作成
		static void MakeMaterial();

		/// @brief 一般的なメッシュを作成
		static void MakeGeometory();

		/// @brief シェーダーを取得
		static void GetShader();

		/// @brief パラメータをリセット
		static void ResetParameter();

		/// @brief 描画準備
		static void DrawSetup();

		/// @brief 描画
		static void Draw(StaticMesh& _staticMesh);
	};
}