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
        Num,
        None
    };

protected:
    Type type;  // �^�C�v
    bool isHit; // �������Ă邩�t���O
    // �������ĂȂ��Ƃ��̐F
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;
public:
    using Component::Component;

    Type GetType()const { return type; }    // ��ނ��擾
};

