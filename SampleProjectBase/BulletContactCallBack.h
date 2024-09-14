#pragma once
#include "DXToBullet.h"

class BulletContactCallBack : public btCollisionWorld::ContactResultCallback
{


public:

    /// @brief 衝突コールバック
    /// @param cp 衝突点の情報（座標など）
    /// @param colObj0Wrap 衝突したオブジェクトA情報
    /// @param partId0 オブジェクトAのサブパート
    /// @param index0 オブジェクトAのサブパートの中のメッシュID
    /// @param colObj1Wrap 衝突したオブジェクトB情報
    /// @param partId1 オブジェクトBのサブパート
    /// @param index1 オブジェクトBのサブパートの中のメッシュID
    /// @return スカラー値
    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
        const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override;
};

