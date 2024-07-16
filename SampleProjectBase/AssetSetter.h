#pragma once
#include "AssetContacter.h"

// アセット管理
#include "AssetCollection.h"

// アセット管理にアセットをセットクラス
class AssetSetter :
    public AssetContacter
{
    AssetSetter() {}
    ~AssetSetter() {}
public:

    /// @brief アセット管理クラスにセット
    /// @tparam T アセットの型
    /// @param _assetName 名前
    /// @param _pAsse アセットのユニークポインタ
    /// @return アセットのポインタ
    template<HashiTaku::AssetType T> static T* SetAsset(const std::string& _assetName, std::unique_ptr<T> _pAsset);

    /// @brief アセットがあるか確認
    /// @tparam T アセットの型
    /// @param _assetName 名前
    /// @return あるフラグ
    template<HashiTaku::AssetType T> static bool CheckImport(const std::string& _assetName);

private:

    /// @brief アセットに名前をつける
    /// @tparam T アセットの型
    /// @param _assetName アセットの名前
    /// @param _assetPtr アセットのポインタ
    template<class T> static void SetAssetName(const std::string& _assetName, T* _assetPtr);
};

/// @brief アセット管理クラスにセット
/// @tparam T アセットの型
/// @param _assetName 名前
/// @param _pAsse アセットのユニークポインタ
/// @return アセットのポインタ
template<HashiTaku::AssetType T>
inline T* AssetSetter::SetAsset(const std::string& _assetName, std::unique_ptr<T> _pAsset)
{
    if (_assetName == "")
    {
        HASHI_DEBUG_LOG("名前が設定されていません");
        return nullptr;
    }
    else if (_pAsset == nullptr)
    {
        HASHI_DEBUG_LOG(_assetName + "：アセットのポインタがnullです");
        return nullptr;
    }

    // アセットをセット
    T* pReturnAsset = pAssetCollection->SetAsset<T>(_assetName, std::move(_pAsset));
    // アセットに名前をつける
    SetAssetName(_assetName, pReturnAsset);

    return pReturnAsset;
}

template<HashiTaku::AssetType T>
inline bool AssetSetter::CheckImport(const std::string& _assetName)
{
    return pAssetCollection->CheckImport<T>(_assetName);
}

template<class T>
inline static void AssetSetter::SetAssetName(const std::string& _assetName, T* _assetPtr)
{
    Asset_Base* pAssetBase = dynamic_cast<Asset_Base*>(_assetPtr);
    if (pAssetBase == nullptr) return;

    pAssetBase->SetName(_assetName);
}
