#pragma once
#include "Mesh_Base.h"

// �X�^�e�B�b�N���b�V��
class StaticMesh :
    public Mesh_Base
{

public:
    StaticMesh() : Mesh_Base(Type::SM) {}
    ~StaticMesh() {}
};

