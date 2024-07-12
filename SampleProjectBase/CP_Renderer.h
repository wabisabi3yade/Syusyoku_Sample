#pragma once
#include "Component.h"

class Mesh;

// 描画コンポーネントのベース
class CP_Renderer :
    public Component
{

protected:

    /// @brief メッシュを描画
    /// @param _mesh 描画するメッシュ
    void MeshDraw(const Mesh& _mesh);
public:
    using Component::Component;

    /// @brief 各レンダラーの描画
    virtual void Draw() = 0;
};

