#pragma once

// 描画
class StaticMesh;
class Material;

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

	Geometory() {};
	~Geometory() {};

	// 描画準備
	static void DrawSetup();

	// 描画
	static void Draw(StaticMesh& _staticMesh);
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

private:
	/// @brief マテリアルを作成
	static void MakeMaterial();

	/// @brief 一般的なメッシュを作成
	static void MakeGeometory();

	/// @brief パラメータをリセット
	static void ResetParameter();
};
