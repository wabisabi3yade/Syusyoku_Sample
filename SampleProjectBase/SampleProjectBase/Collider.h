#pragma once
#include "Component.h"

class Tag;
class Layer;

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

   // ���t���[���E�O�t���[���œ�����������
    std::list<Collider*> hitColliders;
    std::list<Collider*> o_hitColliders; 

    // �������ĂȂ��E�������Ă�Ƃ��̓����蔻��̐F
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;

    Collider() = delete;    
    virtual ~Collider();
public:
    using Component::Component;
    Collider& operator=(const Collider& _other);

    void Init()override;   
    // ����ɓ��������R���C�_�[�ǉ�
    void SetHitCollider(Collider& _hitCollider) { hitColliders.push_back(&_hitCollider); }
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

