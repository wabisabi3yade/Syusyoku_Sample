/* --------------------------------------------------------------------------
Assimp読込の処理構造はHAL大阪の科目：GM31のサンプルを参考にして作られています

2024/07/08
HAL大阪　
橋本拓実
----------------------------------------------------------------------------*/

#pragma once
#include "AssetContacter.h"

// アセット
#include "AssetCollection.h"

// アセットの前方宣言
class Texture;
class Mesh_Base;
class Material;

// アセットをロードするクラス(staticクラス)
class AssetLoader : private AssetContacter
{
	AssetLoader() {};
	~AssetLoader() {};

	/// @brief アセット管理クラスにアセットを送る
	/// @tparam T アセットの型
	/// @param _assetName アセットの名前
	/// @param _pAsset アセットのユニークポインタ
	/// @return アセットのポインタ
	template<class T> static T* SendAsset(const std::string& _assetName, std::unique_ptr<T> _pAsset);

	/// @brief マテリアル読込
	/// @param _pMeshgather メッシュ
	/// @param pScene シーン
	/// @param texturedirectory 読み込むモデルまでのパス名
	static void MaterialLoad(Mesh_Base* _pMeshgather, const aiScene* pScene, std::string texturedirectory);

	/// @brief モデルの内部テクスチャを読み込むクラス
	/// @param _texture 情報を入れるテクスチャの参照
	/// @param Data 先頭アドレス
	/// @param len テクスチャサイズ（メモリにある場合、幅がサイズ）
	static bool TextureMemoryLoad(Texture& _texture , const unsigned char* Data, int len);

	/// @brief テクスチャを作成する
	/// @param _filePath テクスチャパス名
	/// @return テクスチャのユニークポインタ
	static std::unique_ptr<Texture> MakeTexture(const std::string& _filePath);
public:
	
	/// @brief テクスチャをロードしてアセット管理に追加する
	/// @param _filePath テクスチャのパス
	/// @return 成功したかフラグ
	static Texture* TextureLoad(const std::string& _filePath);

	/// @brief モデルをロードしてアセット管理に追加
	/// @param _modelPathPath モデルのパス名
	/// @param _texturePath テクスチャのパス名
	/// @return ロードしたモデルのポインタ
	static Mesh_Base* ModelLoad(const std::string& _modelPathPath, const std::string& _texturePath);

private:	// 便利関数

	/// @brief パスからファイル名を取得する
	/// @param _pathName パス名
	/// @return ファイル名
	static std::string PathToFileName(const std::string& _pathName);
};

/// @brief アセット管理クラスにアセットを送る
/// @tparam T アセットの型
/// @param _assetName アセットの名前
/// @param _pAsset アセットのユニークポインタ
template<class T>
inline T* AssetLoader::SendAsset(const std::string& _assetName, std::unique_ptr<T> _pAsset)
{
	return pAssetCollection->SetAsset<T>(_assetName, std::move(_pAsset));
}
