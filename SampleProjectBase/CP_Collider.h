#pragma once
#include "Component.h"

#include <btBulletDynamicsCommon.h>
#include "DXToBullet.h"

class Tag;
class Layer;

// �����蔻���b�N���X
class CP_Collider : public Component
{
public :
    enum ShapeType   // �`��̎��
    {
        Box,    // �{�b�N�X
        Sphere, // ��
        Capsule,    // �J�v�Z��
        Num
    };

private:
    /// @brief ���Ɍ`��͍쐬���ꂽ���H
    bool isCreateCompound;

protected:
    ShapeType type;  // �^�C�v

    /// @brief ���S���W�Ɗp�x�̃I�t�Z�b�g��ݒ肷�邽�߂̐e�R���W����
    std::unique_ptr<btCompoundShape> pCompound;

    /// @brief �R���W�����̌`��
    std::unique_ptr<btCollisionShape> pCollisionShape;

    /// @brief ���S���W�I�t�Z�b�g
    DirectX::SimpleMath::Vector3 centerOffset;

    /// @brief �p�x�I�t�Z�b�g
    DirectX::SimpleMath::Vector3 angleOffset;
    
public:
    CP_Collider() : type(ShapeType::Num), isCreateCompound(false) {}
    CP_Collider(ShapeType _type);
    virtual ~CP_Collider() {}
    CP_Collider(const CP_Collider& _other);

    CP_Collider& operator=(const CP_Collider& _other);

    // �R���|�[�l���g���ʊ֐�
    void Init() override;
    void OnDestroy() override;

    /// @brief �R���W�����̒��S���W�I�t�Z�b�g���Z�b�g
    /// @param _offset �I�t�Z�b�g���W
    void SetCenterOffset(const DirectX::SimpleMath::Vector3& _offset);

    /// @brief �R���W�����̊p�x�I�t�Z�b�g���Z�b�g
    /// @param _offset �I�t�Z�b�g�p�x
    void SetAngleOffset(const DirectX::SimpleMath::Vector3& _offset);

    /// @brief �Փˌ`��擾
    /// @return �Փˌ`��
    btCollisionShape& GetColliderShape();

    // ��ނ��擾
    ShapeType GetType()const;

    bool GetIsCreateCompound() const;

    void ImGuiSetting() override;

    virtual nlohmann::json Save() override;
    virtual void Load(const nlohmann::json& _data) override;
private:
    void Copy(const CP_Collider& _other);

    /// @brief RigidBody�̌`����Ƃ�
    void RemoveShapeFromRb();

protected:
    // �R���|�[�l���g���ʊ֐�
    void OnEnableTrue() override;
    void OnEnableFalse() override;
    void OnChangeScale() override;

    /// @brief �`����쐬�������A�R���p�E���h�ɒǉ�
    void RecreateShape();

    /// @brief �e�R���W�����`����쐬����
    virtual void CreateShape() = 0;

    /// @brief RigidBpdy�R���|�[�l���g�Ɍ`��𑗂�
    void SendShapeToRb();

    /// @brief �R���p�E���h����`����폜
    void RemoveFromCompound();

    /// @brief �R���p�E���h�Ɍ`���ǉ�
    void AddToCompound();
};

