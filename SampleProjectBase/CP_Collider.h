#pragma once
#include "Component.h"

class Tag;
class Layer;

// �����蔻���b�N���X
class CP_Collider :
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

   // ���t���[���E�O�t���[���œ�����������
    std::list<CP_Collider*> hitColliders;
    std::list<CP_Collider*> o_hitColliders; 

    // �������ĂȂ��E�������Ă�Ƃ��̓����蔻��̐F
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;
 
    virtual ~CP_Collider();
public:
    using Component::Component;

    void Init()override;   
    // ����ɓ��������R���C�_�[�ǉ�
    void SetHitCollider(CP_Collider& _hitCollider) { hitColliders.push_back(&_hitCollider); }
    // ��������Z�b�g
    void ResetColliders() { hitColliders.clear(); } 

    Type GetType()const { return type; }    // ��ނ��擾

private:

#ifdef EDIT
    // ����ɓ�����ƐF��ς���
    std::list<Tag> colorTags;   
    std::list<Layer> colorLayers;
#endif // EDIT

public:
    // �F��ς���^�O�E���C���[��ݒ�
    void SetTagColor(const Tag& _tag); 
    void SetLayerColor(const Layer& _layer);
};

