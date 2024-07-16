#pragma once
#include "AssetContacter.h"

// �A�Z�b�g�Ǘ�
#include "AssetCollection.h"

// �A�Z�b�g�Ǘ��ɃA�Z�b�g���Z�b�g�N���X
class AssetSetter :
    public AssetContacter
{
    AssetSetter() {}
    ~AssetSetter() {}
public:

    /// @brief �A�Z�b�g�Ǘ��N���X�ɃZ�b�g
    /// @tparam T �A�Z�b�g�̌^
    /// @param _assetName ���O
    /// @param _pAsse �A�Z�b�g�̃��j�[�N�|�C���^
    /// @return �A�Z�b�g�̃|�C���^
    template<HashiTaku::AssetType T> static T* SetAsset(const std::string& _assetName, std::unique_ptr<T> _pAsset);

    /// @brief �A�Z�b�g�����邩�m�F
    /// @tparam T �A�Z�b�g�̌^
    /// @param _assetName ���O
    /// @return ����t���O
    template<HashiTaku::AssetType T> static bool CheckImport(const std::string& _assetName);

private:

    /// @brief �A�Z�b�g�ɖ��O������
    /// @tparam T �A�Z�b�g�̌^
    /// @param _assetName �A�Z�b�g�̖��O
    /// @param _assetPtr �A�Z�b�g�̃|�C���^
    template<class T> static void SetAssetName(const std::string& _assetName, T* _assetPtr);
};

/// @brief �A�Z�b�g�Ǘ��N���X�ɃZ�b�g
/// @tparam T �A�Z�b�g�̌^
/// @param _assetName ���O
/// @param _pAsse �A�Z�b�g�̃��j�[�N�|�C���^
/// @return �A�Z�b�g�̃|�C���^
template<HashiTaku::AssetType T>
inline T* AssetSetter::SetAsset(const std::string& _assetName, std::unique_ptr<T> _pAsset)
{
    if (_assetName == "")
    {
        HASHI_DEBUG_LOG("���O���ݒ肳��Ă��܂���");
        return nullptr;
    }
    else if (_pAsset == nullptr)
    {
        HASHI_DEBUG_LOG(_assetName + "�F�A�Z�b�g�̃|�C���^��null�ł�");
        return nullptr;
    }

    // �A�Z�b�g���Z�b�g
    T* pReturnAsset = pAssetCollection->SetAsset<T>(_assetName, std::move(_pAsset));
    // �A�Z�b�g�ɖ��O������
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
