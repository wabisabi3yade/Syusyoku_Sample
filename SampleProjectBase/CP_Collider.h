#pragma once
#include "CloneComponent.h"

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

    // ���t���[���E�O�t���[���œ�����������
    std::list<CP_Collider*> hitColliders;
    std::list<CP_Collider*> o_hitColliders; 

    // �������ĂȂ��E�������Ă�Ƃ��̓����蔻��̐F
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;
    
public:
    CP_Collider() : type(Type::Num) {}
    CP_Collider(Type _type);
    virtual ~CP_Collider();

    void Start() override;   

    // ����ɓ��������R���C�_�[�ǉ�
    void SetHitCollider(CP_Collider& _hitCollider) { hitColliders.push_back(&_hitCollider); }

    // ��������Z�b�g
    void ResetColliders() { hitColliders.clear(); } 

    Type GetType()const { return type; }    // ��ނ��擾
public:
    // �F��ς���^�O�E���C���[��ݒ�
    void SetTagColor(const Tag& _tag); 
    void SetLayerColor(const Layer& _layer);
};

