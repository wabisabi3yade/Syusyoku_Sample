#pragma once
#include "CloneComponent.h"

// ���b�V��
class SingleMesh;

// �`��R���|�[�l���g�̃x�[�X
class CP_Renderer : public Component
{
protected:

    /// @brief ���b�V����`��
    /// @param _mesh �`�悷�郁�b�V��
    void DrawMesh(const SingleMesh& _mesh);
public:
    CP_Renderer() {}
    ~CP_Renderer() {}

    /// @brief �e�����_���[�̕`��
    virtual void Draw() {};
};

