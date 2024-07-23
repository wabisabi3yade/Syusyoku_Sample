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
#include "AssetSetter.h"
#include "AssetGetter.h"

// アセットの前方宣言
class Texture;
class Mesh_Group;
class SingleMesh;
class Material;
class AnimationData;

// ボーン情報
class Bone;

// assimp構造体
struct aiScene;
struct aiMesh;
struct  aiTexture;

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
	static void MaterialLoad(Mesh_Group* _pMeshgather, const aiScene* pScene, std::string texturedirectory);

	/// @brief モデルの内部テクスチャを読み込むクラス
	/// @param _texture 情報を入れるテクスチャの参照
	/// @param _aiTex 内部テクスチャ
	static bool LoadEmbeddedTexture(Texture& _texture, const aiTexture& _aiTex);

	/// @brief テクスチャを作成する
	/// @param _filePath テクスチャパス名
	/// @return テクスチャのユニークポインタ
	static std::unique_ptr<Texture> MakeTexture(const std::string& _filePath);
public:
	
	/// @brief テクスチャをロードしてアセット管理に追加する
	/// @param _filePath テクスチャのパス
	/// @return 成功したかフラグ
	static Texture* TextureLoad(const std::string& _filePath);
	
	/// @brief  モデルをロードしてアセット管理に追加
	/// @param _modelPath モデルのパス名
	/// @param _scale ロードするときのスケール値
	/// @param _isRightHand 左手系か？
	/// @param _isGetScale モデルのスケールを取得するか？
	/// @return ロードしたメッシュ
	static Mesh_Group* ModelLoad(const std::string& _modelPath, float _scale, bool _isLeftHand, 
		bool _isGetScale = true);


	/// @brief アニメーションをロードしてアセット管理に追加
	/// @param _animPath アニメーションのパス
	/// @param _isLeftHand 左手系か？
	/// @return アニメーションデータクラス
	static AnimationData* AnimationLoad(const std::string& _animPath, bool _isLeftHand);

	

private:	// 便利関数

	/// @brief スタティックかスケルタルを判断して作成する
	/// @param _pScene シーン情報
	/// @return 作成したメッシュ群
	static std::unique_ptr<Mesh_Group> CreateMeshGroup(const aiScene* _pScene);

	/// @brief ボーン情報を読み込む
	/// @param _pAiScene シーン情報
	/// @param _skeletalMesh スケルタルメッシュ
	static void CreateBone(const aiScene* _pAiScene, SkeletalMesh& _skeletalMesh);

	/// @brief メッシュからボーン情報を取得
	/// @param _pAiMesh aiメッシュ
	/// @return メッシュのボーン情報
	
	/// @brief メッシュからボーン情報を取得
	/// @param _pAiMesh aiメッシュ
	/// @param _singleMesh 一つメッシュ
	/// @param _boneIdx ボーンインデックス
	/// @return メッシュのボーン情報
	static std::vector<std::unique_ptr<Bone>> CreateBone(const aiMesh* _pAiMesh, SingleMesh& _singleMesh, u_int& _boneIdx);

	/// @brief パスからファイル名を取得する
	/// @param _pathName パス名
	///  @param _isExtexsion 拡張子をつけるか？
	/// @return ファイル名
	static std::string PathToFileName(const std::string& _pathName, bool _isExtension);

	/// @brief 親パス名を取得
	/// @param _pathName パス名
	/// @return 親パス名
	static std::string GetParentPath(const std::string& _pathName);

	/// @brief 最大・最小サイズを更新
	/// @param _vertexPos 頂点座標
	/// @param _max 最大
	/// @param _min 最小
	static void UpdateSize(const DirectX::SimpleMath::Vector3& _vertexPos, DirectX::SimpleMath::Vector3& _max, DirectX::SimpleMath::Vector3& _min);
};

/// @brief アセット管理クラスにアセットを送る
/// @tparam T アセットの型
/// @param _assetName アセットの名前
/// @param _pAsset アセットのユニークポインタ
template<class T>
inline T* AssetLoader::SendAsset(const std::string& _assetName, std::unique_ptr<T> _pAsset)
{
	// 既にインポートされているなら
	if (AssetSetter::CheckImport<T>(_assetName))
	{
		return AssetGetter::GetAsset<T>(_assetName);
	}

	return AssetSetter::SetAsset(_assetName, std::move(_pAsset));
}
