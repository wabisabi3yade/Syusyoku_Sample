#pragma once
#include "DXToBullet.h"

class BulletContactCallBack : public btCollisionWorld::ContactResultCallback
{


public:

    /// @brief �Փ˃R�[���o�b�N
    /// @param cp �Փ˓_�̏��i���W�Ȃǁj
    /// @param colObj0Wrap �Փ˂����I�u�W�F�N�gA���
    /// @param partId0 �I�u�W�F�N�gA�̃T�u�p�[�g
    /// @param index0 �I�u�W�F�N�gA�̃T�u�p�[�g�̒��̃��b�V��ID
    /// @param colObj1Wrap �Փ˂����I�u�W�F�N�gB���
    /// @param partId1 �I�u�W�F�N�gB�̃T�u�p�[�g
    /// @param index1 �I�u�W�F�N�gB�̃T�u�p�[�g�̒��̃��b�V��ID
    /// @return �X�J���[�l
    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
        const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override;
};

