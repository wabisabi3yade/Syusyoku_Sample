#pragma once
#include "Mesh_Base.h"

// �X�P���^�����b�V���N���X
class SkeletalMesh :
    public Mesh_Base
{

public:
    SkeletalMesh() : Mesh_Base(Type::SK) {}
    ~SkeletalMesh() {}
};

