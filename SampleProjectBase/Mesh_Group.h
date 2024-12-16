#pragma once
#include "AssetPath_Base.h"

// 描画関連
#include "SingleMesh.h"
#include "Material.h"
#include "Texture.h"

namespace HashiTaku
{
	/// @brief メッシュ群（スケルタル・スタティックメッシュ）の基底クラス
	class Mesh_Group : public AssetPath_Base
	{
	public:

		/// @brief メッシュの種類
		enum class MeshType
		{
			SM,	// スタティックメッシュ
			SK,	// スケルタルメッシュ
			None	// 設定なし
		};
	private:
		/// @brief メッシュ配列
		std::vector<std::unique_ptr<SingleMesh>> pMeshes;

		/// @brief マテリアル配列
		std::vector<Material*> materials;

		/// @brief 中心座標
		DirectX::SimpleMath::Vector3 centerPos;

		/// @brief モデルサイズ
		DirectX::SimpleMath::Vector3 size;

		/// @brief ロード時、スケール倍率
		float loadScale;

		/// @brief ロード時オフセット角度
		DirectX::SimpleMath::Vector3 loadOffsetAngles;

		/// @brief ロード時Y軸反転したか？
		bool isFlipY;

		/// @brief メッシュの種類
		MeshType meshType;

		/// @brief サイズは取得するか？
		bool isGetSize;

		/// @brief 右手系のモデルか？
		bool isRightHand;
	public:
		Mesh_Group() : meshType(MeshType::None), loadScale(1.0f), isFlipY(false), isGetSize(false), isRightHand(false) {}

		/// @brief コンストラクタ
		/// @param _meshType メッシュの種類
		Mesh_Group(MeshType _meshType) : meshType(_meshType), loadScale(1.0f), isFlipY(false), isGetSize(false), isRightHand(false) {}
		virtual ~Mesh_Group() {}

		/// @brief メッシュを追加
		/// @param _pMesh メッシュのユニークポインタ
		/// @param _materialID マテリアルインデックス
		void AddMesh(std::unique_ptr<SingleMesh> _pMesh);

		/// @brief マテリアルを追加
		/// @param _pMaterial セットするマテリアル
		void AddMaterial(Material* _pMaterial);

		/// @brief メッシュを取得
		/// @param _meshIdx 何個目
		/// @return メッシュ
		SingleMesh* GetMesh(u_int _meshIdx) const;

		/// @brief メッシュの数取得
		/// @return メッシュの数
		u_int GetMeshNum();

		/// @brief マテリアル取得mesh
		/// @param _materialIdx マテリアルID
		/// @return マテリアルのポインタ
		Material* GetMaterial(u_int _materialIdx);

		/// @brief 頂点シェーダー取得
		/// @return 頂点シェーダー
		VertexShader* GetVertexShader();

		/// @brief マテリアルの数取得
		/// @return マテリアルの数
		u_int GetMaterialNum();

		/// @brief 中心座標を取得
		/// @return 中心座標
		DirectX::SimpleMath::Vector3 GetCenterPosition() const;

		/// @brief モデルのサイズを取得
		/// @return サイズ
		DirectX::SimpleMath::Vector3 GetSize() const;

		// ロード時のスケール倍率を取得する
		float GetLoadOffsetScale() const;

		/// ロード時のオフセット角度を取得する
		DirectX::SimpleMath::Vector3 GetLoadOffsetAngles() const;

		/// @brief メッシュ群の種類を取得
		/// @return 種類
		MeshType GetType() const;

		/// @brief モデルの中心座標をセット
		/// @param _size 中心座標
		void SetCenterPosition(const DirectX::SimpleMath::Vector3& _centerPos);

		/// @brief サイズをセット
		/// @param _size サイズ
		void SetSize(const DirectX::SimpleMath::Vector3& _size);

		/// @brief スケール倍率をセット
		/// @param _scaleTimes スケール倍率
		void SetLoadOffsetScale(float _scaleTimes);

		/// @brief Y軸反転したかをセット
		/// @param  _isLoadFlipY Y軸反転したか？
		void SetLoadFlipY(bool _isLoadFlipY);

		// 右手系かどうかセット
		void SetIsRightHand(bool _isRightHand);

		// サイズを取得するかセット
		void SetIsGetSize(bool _isGetSize);

		// マテリアルの頂点・ピクセルシェーダーをセット
		void SetVertexShader(const std::string& _vsName);
		void SetPixelShader(const std::string& _psName);

		/// @brief セーブする
		/// @param _sceneData セーブデータ
		nlohmann::json Save() override;

		/// @brief ロードする
		/// @param _sceneData ロードするデータ 
		void Load(const nlohmann::json& _data) override;
	};
}