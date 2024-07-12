#pragma once
#include "Mesh_Base.h"

// スケルタルメッシュクラス
class SkeletalMesh :
    public Mesh_Base
{

public:
    SkeletalMesh() : Mesh_Base(Type::SK) {}
    ~SkeletalMesh() {}
};

