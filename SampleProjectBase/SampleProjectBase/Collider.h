#pragma once
#include "Component.h"

// �����蔻���b�N���X
class Collider :
    public Component
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

    std::vector<Collider*> hitColliders;    // ���t���[���œ�����������
    std::vector<Collider*> o_hitColliders;  // �O�t���[���œ�����������

    //bool isHit{false}; // �������Ă邩�t���O

    // �������ĂȂ��E�������Ă�Ƃ��̓����蔻��̐F
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;

    Collider() = delete;
    virtual ~Collider();
public:
    using Component::Component;
    void Init()override;    // ��������������(Collider�p�������N���X�͐�΂ɌĂԂ悤�ɂ���)

    /* void SetIsHit(bool _isHit) { isHit = _isHit; }*/

    // ���̓����蔻��ɓ��������R���C�_�[��z��ɒǉ�����
    void SetHitCollider(Collider& _hitCollider) { hitColliders.push_back(&_hitCollider); }
    void ResetColliders() { hitColliders.clear(); } // �������Ă锻������Z�b�g����

    Type GetType()const { return type; }    // ��ނ��擾
};

