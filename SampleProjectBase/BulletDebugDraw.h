#pragma once
#include "DXToBullet.h"

namespace HashiTaku
{
    /// @brief DirectX上でBulletPhiscs当たり判定描画を行うクラス
    class BulletDebugDraw : public btIDebugDraw
    {
    private:
        /// @brief 描画設定
        int debugMode;

        /// @brief 当たり判定可視化するか？
        bool isDisplay;
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

        /// @brief 当たり判定可視化するか？
        /// @param _isDisplay 表示させるフラグ
        void SetIsDisplay(bool _isDisplay);

        /// @brief 表示させるか取得
        /// @return 表示させるか？
        bool GetIsDisplay() const;
    };
}