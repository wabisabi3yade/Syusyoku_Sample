#pragma once
#include "Component.h"
#include "PlaneMesh.h"
#include "Sprite.h"
#include "SpriteDrawer.h"

namespace HashiTaku
{
	class Texture;
	class Material;
	class CP_Camera;

	/// @brief スカイボックスを描画する
	class CP_SkyBox : public Component
	{
	public:
		/// @brief ボックスの面の場所
		enum class FaceKind
		{
			X_Right,
			X_Left,
			Y_Top,
			Y_Bottom,
			Z_Front,
			Z_Back,
			MaxNum
		};

	private:
		/// @brief スカイボックスの面
		std::unique_ptr<Sprite> pFaces[static_cast<int>(FaceKind::MaxNum)];

		/// @brief スプライト描画
		std::unique_ptr<SpriteDrawer> pSpriteDrawer;

		/// @brief 使用するマテリアル
		Material* pMaterial;

		/// @brief スカイボックスのサイズ
		float size;
	public:
		CP_SkyBox();
		~CP_SkyBox() {}
		CP_SkyBox(const CP_SkyBox& _other);

		CP_SkyBox& operator=(const CP_SkyBox& _other);

		void Init() override;
		void Draw() override;
		void ImGuiDebug() override;

		/// @brief 指定した面のテクスチャを貼る
		/// @param _pTex テクスチャ
		/// @param _setFace 指定した面
		void SetTexture(Texture& _pTex, FaceKind _setFace);

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		nlohmann::json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief マテリアルのセットアップ
		void MaterialSetting();

		/// @brief 描画できるかチェック
		/// @return 描画できるか？
		bool IsCanDraw();

		/// @brief 1面を描画する
		/// @param _faceNum 描画する面
		void FaceDraw(u_int _faceNum);

		/// @brief 面の情報を取得する
		/// @param _facePlace 面の位置
		/// @param _outPos 座標
		/// @param _outScale スケール
		/// @param _outAngles 角度
		void FaceTransform(FaceKind _facePlace, DirectX::SimpleMath::Vector3& _outPos, DirectX::SimpleMath::Vector3& _outScale, DirectX::SimpleMath::Vector3& _outAngles);

		void Copy(const CP_SkyBox& _other);

		/// @brief テクスチャをセーブする
		/// @param _data 保存するデータ先
		/// @param _kind 面の種類
		void SaveTexture(nlohmann::json& _data, FaceKind _kind);

		void LoadTexture(const nlohmann::json& _data, FaceKind _kind);
	};
}