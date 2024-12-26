#pragma once
#include "Texture.h"
#include "PlaneMesh.h"

#include "ISaveLoad.h"

namespace HashiTaku
{
	class Material;

	// 四角形ポリゴンにテクスチャを貼って表示するクラス
	class Sprite : public ISaveLoad
	{
		/// @brief テクスチャ
		Texture* pTexture;

		/// @brief 四角形ポリゴン
		std::unique_ptr<PlaneMesh> pSquareMesh;

		/// @brief uv座標の始点(左上)
		DXSimp::Vector2 uvStart;

		/// @brief uv座標の終点(右下)
		DXSimp::Vector2 uvEnd;

		/// @brief uvの分割数
		DXSimp::Vector2 uvDivideNum;

		/// @brief テクスチャが設定されているか
		bool isTextureEnable{ false };

		/// @brief コピー関数
		/// @param _other コピーされる対象
		void Copy(const Sprite& _other);
	public:
		Sprite();
		Sprite(const Sprite& _other);
		~Sprite() {};

		Sprite& operator=(const Sprite& _other);

		/// @brief テクスチャをセット
		/// @param _tex テクスチャの参照
		void SetTexture(Texture& _tex);

		/// @brief テクスチャを取得
		/// @return テクスチャの参照
		Texture* GetTexture() const;

		/// @brief 四角形ポリゴンを取得
		/// @return 四角形ポリゴンのconst参照
		const PlaneMesh& GetSquare() const;

		// テクスチャ使用しているか返す
		bool GetIsTexEnable() const;

		/// @brief uv座標を設定
		/// @param _uvStart uv座標始点
		/// @param _uvEnd uv座標終点
		void SetUV(const DXSimp::Vector2& _uvStart, const DXSimp::Vector2& _uvEnd);

		/// @brief セーブする
		/// @param _sceneData セーブデータ
		json Save() override;

		/// @brief ロードする
		/// @param _sceneData ロードするデータ 
		void Load(const json& _data) override;
	};
}