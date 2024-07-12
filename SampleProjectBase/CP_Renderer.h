#pragma once
#include "Component.h"

class Mesh;

// �`��R���|�[�l���g�̃x�[�X
class CP_Renderer :
    public Component
{

protected:

    /// @brief ���b�V����`��
    /// @param _mesh �`�悷�郁�b�V��
    void MeshDraw(const Mesh& _mesh);
public:
    using Component::Component;

    /// @brief �e�����_���[�̕`��
    virtual void Draw() = 0;
};

