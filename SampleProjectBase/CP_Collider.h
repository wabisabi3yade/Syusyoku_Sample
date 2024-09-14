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

    /// @brief ���S���W�Ɗp�x�̃I�t�Z�b�g��ݒ肷�邽�߂̐e�R���W����
    std::unique_ptr<btCompoundShape> pCompound;

    /// @brief �R���W�����̌`��
    std::unique_ptr<btCollisionShape> pCollisionShape;

    /// @brief ���S���W�I�t�Z�b�g
    DirectX::SimpleMath::Vector3 centerOffset;

    /// @brief �p�x�I�t�Z�b�g
    DirectX::SimpleMath::Vector3 angleOffset;

    // �������ĂȂ��E�������Ă�Ƃ��̓����蔻��̐F
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;
    
public:
    CP_Collider() : type(Type::Num) {}
    CP_Collider(Type _type);
    CP_Collider(const CP_Collider& _other);

    CP_Collider& operator=(const CP_Collider& _other);

    void Init() override;
    void OnDestroy() override;

    void SetCenterOffset(const DirectX::SimpleMath::Vector3& _offset);

    void SetAngleOffset(const DirectX::SimpleMath::Vector3& _offset);

    // ��ނ��擾
    Type GetType()const { return type; }   

    void ImGuiSetting() override;

    /// @brief �Z�[�u����
     /// @param _data �Z�[�u�V�[���f�[�^
    virtual nlohmann::json Save() override;
    /// @brief ���[�h����
    /// @param _data ���[�h����V�[���f�[�^ 
    virtual void Load(const nlohmann::json& _data) override;

private:
    void Copy(const CP_Collider& _other);

    /// @brief RigidBody�̌`����Ƃ�
    void RemoveShapeOfRb();

protected:
    void OnEnableTrue() override;
    void OnEnableFalse() override;

    void SetShape();
    virtual void CreateShape() = 0;

    /// @brief RigidBpdy�R���|�[�l���g�Ɍ`��𑗂�
    void SendShapeToRb();
};

