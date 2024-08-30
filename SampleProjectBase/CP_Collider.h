#pragma once
#include "CloneComponent.h"

#include <btBulletDynamicsCommon.h>
#include "DXToBullet.h"

class Tag;
class Layer;

// �����蔻���b�N���X
class CP_Collider : public Component
{
public :
    enum Type   // �����蔻��̎��
    {
        Box,    // �{�b�N�X
        Sphere, // ��
        Num
    };

protected:
    Type type;  // �^�C�v

    /// @brief �R���W�����̌`��
    std::unique_ptr<btCollisionShape> pCollisionShape;

    // �������ĂȂ��E�������Ă�Ƃ��̓����蔻��̐F
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;
    
public:
    CP_Collider() : type(Type::Num) {}
    CP_Collider(Type _type);
    CP_Collider(const CP_Collider& _other);
    virtual ~CP_Collider();

    CP_Collider& operator=(const CP_Collider& _other);

    // ��ނ��擾
    Type GetType()const { return type; }   

private:
    void Copy(const CP_Collider& _other);

    /// @brief RigidBody�R���|�[�l���g���쐬����
    void CreateRigidBody();

protected:
    void Start() override;

    /// @brief RigidBpdy�R���|�[�l���g�Ɍ`��𑗂�
    /// @param _shape �`��
    void SendShapeToRb(btCollisionShape& _shape);
};

