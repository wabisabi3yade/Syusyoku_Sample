#pragma once
#include "DXToBullet.h"

#include "VertexShader.h"
#include "PixelShader.h"

/// @brief DirectX上でBulletPhiscs当たり判定描画を行うクラス
class BulletDebugDraw : public btIDebugDraw
{
private:
    // 描画するシェーダー
    VertexShader* pVxShader;
    PixelShader* pPxShader;

    /// @brief 描画設定
    int debugMode;

public:
    BulletDebugDraw();
    ~BulletDebugDraw() {}

    // BulletPhisicsのインターフェース関数
    void drawLine(const btVector3& _from, const btVector3& _to, const btVector3& _color) override;
    void drawContactPoint(const btVector3& _pointOnB, const btVector3& _normalOnB, btScalar _distance, int _lifeTime, const btVector3& _color) override;
    void reportErrorWarning(const char* _warningString) override;
    void draw3dText(const btVector3& _location, const char* _textString) override;
    virtual void setDebugMode(int _debugMode) override;
    virtual int getDebugMode() const override;
};

