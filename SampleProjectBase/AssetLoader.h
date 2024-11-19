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
class SkeletalMesh;
class Material;
class AnimationData;

// ボーン・ツリーノード
class Bone;
class TreeNode;
class AnimationChannel;

// assimp構造体
struct aiScene;
struct aiMesh;
struct aiTexture;
struct aiNode;
struct aiNodeAnim;
struct aiAnimation;

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
	static void MaterialLoad(Mesh_Group* _pMeshgather, const aiScene* pScene, const std::string& _texturedirectory);

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
	/// @param _isFlipY 回転する
	/// @param _isRightHand 右手系か？
	/// @param _isGetScale モデルのスケールを取得するか？
	/// @return ロードしたメッシュ
	static Mesh_Group* ModelLoad(const std::string& _modelPath, float _scale, bool _isFlipY, bool _isRightHand, bool _isGetScale = true);

	/// @brief アニメーションをロードしてアセット管理に追加
	/// @param _animPath アニメーションのパス
	/// @param _boneName 対応ボーン名
	/// @param _isLeftHand 右手系か？
	/// @return アニメーションデータクラス
	static AnimationData* AnimationLoad(const std::string& _animPath, const std::string& _boneName,
		bool _isRightHand);

	/// @brief	エフェクシア専用エフェクトをロードする
	/// @param _pathName パス名
	/// @param _loadScale ロード時のスケール
	/// @return ロードしたエフェクト
	static VisualEffect* VFXLoadForEffekseer(const std::string& _pathName, float _loadScale);

private:	// 便利関数

	/// @brief スタティックかスケルタルを判断して作成する
	/// @param _pScene シーン情報
	/// @param _assetName アセット名
	/// @param _loadScale ロード時のスケール
	/// @param _loadAngles ロード時のオフセット角度
	/// @return 作成したメッシュ群
	static std::unique_ptr<Mesh_Group> CreateMeshGroup(const aiScene* _pScene, const std::string& _assetName, float _loadScale, const DirectX::SimpleMath::Vector3& _loadAngles);

	/// @brief ボーンを生成する
	/// @param _pScene シーン情報
	/// @param _skeletalMesh 情報いれるスケルタルメッシュ
	/// @param _loadScale ロード時のスケール
	/// @param _loadAngles ロード時のオフセット角度
	static void CreateBone(const aiScene* _pScene, SkeletalMesh& _skeletalMesh, float _loadScale, const DirectX::SimpleMath::Vector3& _loadAngles);

	/// @brief ノードを生成する再帰関数
	/// @param _aiChildNode 子ノード
	/// @param _skeletalMesh スケルタルメッシュ
	/// @return 生成したノード
	static std::unique_ptr<TreeNode> CreateNode(const aiNode& _aiChildNode, SkeletalMesh& _skeletalMesh);
	
	/// @brief	頂点とボーン情報を紐づける
	/// @param _pAiMesh aiメッシュ
	/// @param _singleMesh 一つのメッシュ
	/// @param _skeletalMesh　スケルタルメッシュ
	static void LinkVertexToBone(const aiMesh* _pAiMesh, SingleMesh& _singleMesh, SkeletalMesh& _skeletalMesh);

	/// @brief アニメーションノードを作成する
	/// @param _pAiAnimNode assimpのアニメーションノード
	/// @param _animData アニメーションノードを追加するアニメーションデータ
	/// @param _boneList 対応ボーンリスト
	static std::unique_ptr<AnimationChannel> CreateAnimChannel(const aiNodeAnim* _pAiAnimNode, const BoneList& _boneList);

	/// @brief アニメーションの時間を取得する
	/// @param _pAiAnim assimpアニメーション
	/// @return アニメーション時間
	static float GetAnimationTime(const aiAnimation* _pAiAnim);

	/// @brief アニメーションのキー数を取得
	/// @param _pAiAnim assimpアニメーション
	/// @return アニメーションキー数
	static u_int GetAnimationFrameCnt(const aiAnimation* _pAiAnim);

	/// @brief 1キーごとの時間を取得する
	/// @param _pAiAnim assimpアニメーション
	/// @return 1キーごとの時間
	static float GetTimePerKey(const aiAnimation* _pAiAnim);

	/// @brief パスからファイル名を取得する
	/// @param _pathName パス名
	/// @return ファイル名
	static std::string PathToFileName(const std::string& _pathName);

	/// @brief 親パス名を取得
	/// @param _pathName パス名
	/// @return 親パス名
	static std::string GetParentPath(const std::string& _pathName);

	/// @brief 拡張子無しのファイル名を取得する
	/// @param _pathName パス名
	/// @return 拡張しなしファイル名
	static std::string GetPathNameNotExt(const std::string& _pathName);

	/// @brief string→u16stringに変換
	/// @param _string 変換したいstring型文字列
	/// @return u16string文字列
	static std::u16string ConvertToU16String(const std::string& _string);

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
