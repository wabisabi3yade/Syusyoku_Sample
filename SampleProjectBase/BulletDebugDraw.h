#pragma once
#include "DXToBullet.h"

namespace HashiTaku
{
    /// @brief DirectX���BulletPhiscs�����蔻��`����s���N���X
    class BulletDebugDraw : public btIDebugDraw
    {
    private:
        /// @brief �`��ݒ�
        int debugMode;

        /// @brief �����蔻��������邩�H
        bool isDisplay;
    public:
        BulletDebugDraw();
        ~BulletDebugDraw() {}

        // BulletPhisics�̃C���^�[�t�F�[�X�֐�
        void drawLine(const btVector3& _from, const btVector3& _to, const btVector3& _color) override;
        void drawContactPoint(const btVector3& _pointOnB, const btVector3& _normalOnB, btScalar _distance, int _lifeTime, const btVector3& _color) override;
        void reportErrorWarning(const char* _warningString) override;
        void draw3dText(const btVector3& _location, const char* _textString) override;
        virtual void setDebugMode(int _debugMode) override;
        virtual int getDebugMode() const override;

        /// @brief �����蔻��������邩�H
        /// @param _isDisplay �\��������t���O
        void SetIsDisplay(bool _isDisplay);

        /// @brief �\�������邩�擾
        /// @return �\�������邩�H
        bool GetIsDisplay() const;
    };
}