#pragma once
#include "Mesh_Base.h"

// スタティックメッシュ
class StaticMesh :
    public Mesh_Base
{

public:
    StaticMesh() : Mesh_Base(Type::SM) {}
    ~StaticMesh() {}
};

